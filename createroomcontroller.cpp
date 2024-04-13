#include "createroomcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>
#include "myrequestmapper.h"


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

    if (request.getMethod()!="POST"){
           response.setStatus(404, "Not_found");
           return;
    }

    response.setHeader("Request-type", "Create_room");
    QString type=object["preset"].toString();
    QString creatorLogin=object["creator_id"].toString();
    QString contactLogin=object["invite"].toString();

    if (type!="private_chat"){
        response.setStatus(400, "Unknown");
        return;
    }

    pM->lock();
    User whoBanned;
    whoBanned=pMdb->selectUser(creatorLogin);
    int idWhoBanned=whoBanned.id.toInt();
    User whoBan=pMdb->selectUser(contactLogin);
    int idWhoBan=whoBan.id.toInt();
    bool banned=pMdb->checkIfBan(idWhoBan, idWhoBanned);
    bool banned2=pMdb->checkIfBan(idWhoBanned, idWhoBan);
    pM->unlock();

    if (banned||banned2)
    {
        response.setStatus(403, "Forbidden");
        return;
    }

    int id=0;
    pM->lock();
    pMdb->selectRoomByLogins(creatorLogin, contactLogin, id);
    pM->unlock();

    if (id!=0)
    {
        Room r;
        r.id=id;
        r.isActive=1;

        pM->lock();
        bool res=pMdb->updateRoom(r);
        pM->unlock();
        if (res)response.setStatus(200, "OK");
        else response.setStatus(400, "Unknown");
        return;
    }
    else
    {
        pM->lock();
        bool res=pMdb->findUser(contactLogin);
        pM->unlock();

        if (res){
            pM->lock();
            bool insertRoomRes=pMdb->insertRoom(creatorLogin+"AND"+contactLogin);
            int roomID=pMdb->selectRoom();
            bool insertUser1Res=pMdb->insertUserRoom(contactLogin, roomID, RandomGenerator());
            bool insertUser2Res=pMdb->insertUserRoom(creatorLogin, roomID, RandomGenerator());

            pM->unlock();

            QList<QJsonObject> roomsListCreator= pMdb->selectRooms(creatorLogin);
            QList<QJsonObject> roomsListClient= pMdb->selectRooms(contactLogin);

            QString creatorToken=MyRequestMapper::makeAccessToken(creatorLogin, roomsListCreator);
            QString userToken=MyRequestMapper::makeAccessToken(contactLogin, roomsListClient);

            User creator;
            creator.login=creatorLogin;
            creator.accessToken=creatorToken;

            User user;
            user.accessToken=userToken;
            user.login=contactLogin;

            pM->lock();
            pMdb->updateUser(creator);
            pMdb->updateUser(user);
            pM->unlock();

            if (insertRoomRes&insertUser1Res&insertUser2Res)response.setStatus(200, "OK");
            else response.setStatus(400, "Unknown");
         }
    }
}
