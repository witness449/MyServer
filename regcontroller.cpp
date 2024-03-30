#include "regcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>

RegController::RegController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void RegController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    if (request.getMethod()=="POST")
    {
        //regController.service(request, response);
        response.setHeader("Request-type", "Registration");
        QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
        QJsonObject object=doc.object();

        QString login=object["Login"].toString();
        QString password=object["Password"].toString();

        User u;
        u.Login=login;
        u.Password=password;

        pM->lock();
        bool exists=pMdb->userExists(login);
        pM->unlock();

        if (!exists)
        {
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
        else
        {
            response.setStatus(400, "User in use");
        }


        //pMdb->printTable();
    }
    else{
        response.setStatus(404, "Unknown");
    }
}

