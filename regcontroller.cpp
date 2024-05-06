#include "regcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>

RegController::RegController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void RegController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM){
    if (request.getMethod()!="POST"){
           response.setStatus(404, "Not_found");
           return;
    }

    response.setHeader("Request-type", "Registration");
    QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
    QJsonObject object=doc.object();

    if (object["type"].toString()!="m.login.password"){
        response.setStatus(400, "Unknown");
        return;
    }

    QString login=object["identifier"].toString();
    QString password=object["password"].toString();

    if (login.length()>10||password.length()>10){
        response.setStatus(400, "Invalid_user_name");
        return;
    }

    User u;
    u.login=login;
    u.password=password;

    pM->lock();
    bool exists=pMdb->findUser(login);
    pM->unlock();

    if (!exists){
        pM->lock();
        bool res=pMdb->insertUser(u);
        pM->unlock();

        if (res){
            response.setStatus(200, "OK");
        }
        else{
            response.setStatus(403, "Unimposible");
        }
   }
   else{
        response.setStatus(400, "User_in_use");
   }
}



