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

        pM->lock();
        bool res=pMdb->clientInsert(login, password);
        pM->unlock();

        if (res){
            response.setStatus(200, "OK");
        }
        else{
            response.setStatus(403, "Forbidden");
        }
        //pMdb->printTable();
    }
    else{
        response.setStatus(403, "Forbidden");
    }
}

