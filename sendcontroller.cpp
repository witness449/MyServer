#include "sendcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>

SendController::SendController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void SendController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
    QJsonObject object=doc.object();

    if (request.getMethod()!="PUT"){
           response.setStatus(404, "Not_found");
           return;
    }

    response.setHeader("Request-type", "Send");
    Message m;
    m.content=object["body"].toString();
    m.idRoom=object["room_id"].toString();
    m.senderLogin=object["sender_id"].toString();
    QString accessToken=object["access_token"].toString();

    Room r=pMdb->selectRoom(m.idRoom.toInt());
    if (r.isActive){
        bool permission=pMdb->checkAccess(m.senderLogin, accessToken);
        if (permission){
            pM->lock();
            bool res=pMdb->insertMessage(m);
            pM->unlock();
            if (res) response.setStatus(200, "Ok");
            else response.setStatus(400, "Unknown");
        }
        else{
            response.setStatus(403, "Forbidden");
        }
    }
    else{
        response.setStatus(403, "Forbidden");
    }
}
