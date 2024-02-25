#include "createroomcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>


QString RandomGenerator()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    int randInt;
    QString myString;
    for(int i=0; i<5; i++)
    {
    randInt = qrand()%('Z'-'A'+1)+'A';
    myString.append(randInt);
    }
    return myString;
}


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
        pMdb->insertRoom(creatorLogin+"AND"+userLogin);
        pM->lock();
        //std::string tmp = std::to_string(pMdb->selectRoom());
        //char const *roomID = tmp.c_str();
        int roomID=pMdb->selectRoom();
        pMdb->insertUserRoom(userLogin, roomID, RandomGenerator());
        pMdb->insertUserRoom(creatorLogin, roomID, RandomGenerator());

        QString str_roomID=QString::number(roomID);
        pMdb->createMessageTable(str_roomID);

        response.setStatus(200, "OK");
        pM->unlock();
    }
    else{
        response.setStatus(403, "Forbidden");
    }
}
