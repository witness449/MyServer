#include "leavecontroller.h"
#include "room.h"
#include <QJsonDocument>
#include <QJsonObject>

LeaveController::LeaveController(QObject *parent) :
    HttpRequestHandler(parent)
{
}


void LeaveController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
    QJsonObject object=doc.object();

    if (request.getMethod()!="POST"){
           response.setStatus(404, "Not_found");
           return;
    }

    response.setHeader("Request-type", "Leave");

    QString creatorLogin=object["creator_id"].toString();
    int idRoom=object["room_id"].toString().toInt();
    QString accessToken=object["access_token"].toString();
    bool permission=pMdb->checkAccess(creatorLogin, accessToken);

    if(permission){
        Room r;
        r.id=idRoom;
        r.isActive=0;

        pM->lock();
        bool res=pMdb->updateRoom(r);
        pM->unlock();

        if (res)response.setStatus(200, "OK");
        else response.setStatus(400, "Unknown");
    }
    else{
        response.setStatus(403, "Forbidden");
    }
 }
