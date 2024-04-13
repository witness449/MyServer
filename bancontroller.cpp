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

    if (request.getMethod()!="POST"){
           response.setStatus(404, "Not_found");
           return;
    }

    response.setHeader("Request-type", "Ban");

    QJsonObject object=doc.object();
    QString creatorLogin=object["admin_id"].toString();
    int idRoom=object["room_id"].toString().toInt();
    QString accessToken=object["access_token"].toString();

    bool permission=pMdb->checkAccess(creatorLogin, accessToken);

    if (permission){
        Room r;
        r.id=idRoom;
        r.isActive=0;
        pM->lock();
        bool resUpd=pMdb->updateRoom(r);
        User whoBanned;
        whoBanned=pMdb->selectContact(creatorLogin, idRoom);
        int idWhoBanned=whoBanned.id.toInt();
        User whoBan=pMdb->selectUser(creatorLogin);
        int idWhoBan=whoBan.id.toInt();
        bool resIns=pMdb->insertBlackList(idWhoBan, idWhoBanned);
        pM->unlock();
        if (resUpd&resIns){
            response.setStatus(200, "Ok");
        }
        else{
            response.setStatus(400, "Unknown");
        }
    }
    else{
        response.setStatus(403, "Forbidden");
    }




}
