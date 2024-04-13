#include "unbancontroller.h"
#include "room.h"
#include <QJsonDocument>
#include <QJsonObject>

UnbanController::UnbanController(QObject *parent) :
    HttpRequestHandler(parent)
{
}


void UnbanController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
    QJsonObject object=doc.object();

    if (request.getMethod()!="POST"){
           response.setStatus(404, "Not_found");
           return;
    }

    response.setHeader("Request-type", "Unban");

    QString creatorLogin=object["admin_id"].toString();
    QString contactLogin=object["user_id"].toString();

    pM->lock();
    User whoBanned;
    whoBanned=pMdb->selectUser(contactLogin);
    int idWhoBanned=whoBanned.id.toInt();
    User whoBan=pMdb->selectUser(creatorLogin);
    int idWhoBan=whoBan.id.toInt();
    bool res=pMdb->deleteBlackList(idWhoBan, idWhoBanned);
    pM->unlock();

    if(res){
        response.setStatus(200, "Ok");
    }
    else{
        response.setStatus(400, "Unknown");
    }
}
