#include "bancontroller.h"
#include "room.h"
#include <QJsonDocument>
#include <QJsonObject>

BanController::BanController(QObject *parent) :
    HttpRequestHandler(parent)
{
}


void BanController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
    QJsonObject object=doc.object();

    QString creatorLogin=object["creatorLogin"].toString();
    int idRoom=object["idRoom"].toString().toInt();
    Room r;
    r.Id=idRoom;
    r.isActive=0;

    pM->lock();
    pMdb->updateRoom(r);
    pM->unlock();

    /*if (res){
        pMdb->insertRoom(creatorLogin+"AND"+userLogin);
        pM->lock();
        //std::string tmp = std::to_string(pMdb->selectRoom());
        //char const *roomID = tmp.c_str();
        int roomID=pMdb->selectRoom();
        pMdb->insertUserRoom(userLogin, roomID, RandomGenerator());
        pMdb->insertUserRoom(creatorLogin, roomID, RandomGenerator());

        QString str_roomID=QString::number(roomID);

        //pMdb->createMessageTable(str_roomID);

        pM->unlock();
        QList<QJsonObject> roomsListCreator= pMdb->selectRooms(creatorLogin);
        QList<QJsonObject> roomsListClient= pMdb->selectRooms(userLogin);

        QString creatorToken=MyRequestMapper::makeAccessToken(creatorLogin, roomsListCreator);
        QString userToken=MyRequestMapper::makeAccessToken(userLogin, roomsListClient);

        User creator;
        creator.Login=creatorLogin;
        creator.AccessToken=creatorToken;
        User user;
        user.AccessToken=userToken;
        user.Login=userLogin;

        pM->lock();
        pMdb->updateUser(creator);
        pMdb->updateUser(user);
        pM->unlock();


        response.setStatus(200, "OK");
    }
    else{
        response.setStatus(403, "Forbidden");
    }*/
}
