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
    QString clientLogin=object["clientLogin"].toString();

    pM->lock();
    bool res=pMdb->findClient(clientLogin);
    pM->unlock();

    if (res){
        pMdb->insertRoom("t");
        pM->lock();
        std::string tmp = std::to_string(pMdb->selectRoom());
        char const *roomID = tmp.c_str();
        pMdb->insertClientRoom(clientLogin, roomID);
        pMdb->insertClientRoom(creatorLogin, roomID);
        response.setStatus(200, "OK");
        pM->unlock();
    }
    else{
        response.setStatus(403, "Forbidden");
    }
}
