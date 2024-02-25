#include "createroomcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>

CreateRoomController::CreateRoomController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void CreateRoomController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
    QJsonObject object=doc.object();

    QString creatorLogin=object["creatorLogin"].toString();
    QString userLogin=object["clientLogin"].toString();

    pM->lock();
    bool res=pMdb->findUser(userLogin);
    pM->unlock();

    if (res){
        pMdb->insertRoom("t");
        pM->lock();
        //std::string tmp = std::to_string(pMdb->selectRoom());
        //char const *roomID = tmp.c_str();
        int roomID=pMdb->selectRoom();
        pMdb->insertUserRoom(userLogin, roomID, "1234");
        pMdb->insertUserRoom(creatorLogin, roomID, "1234");

        QString str_roomID=QString::number(roomID);
        pMdb->createMessageTable(str_roomID);

        response.setStatus(200, "OK");
        pM->unlock();
    }
    else{
        response.setStatus(403, "Forbidden");
    }
}
