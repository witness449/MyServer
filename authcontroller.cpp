#include "authcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMultiMap>
#include <QDebug>

AuthController::AuthController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void AuthController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    HttpSession session=sessionStore->getSession(request,response,true);

    QMap<QByteArray,QVariant> sessDetails=session.getAll();
    for (auto el:sessDetails)
    {
        qDebug()<<el;
    }


    response.setHeader("Request-type", "Authorization");
    QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
    QJsonObject object=doc.object();

    QString login=object["Login"].toString();
    QString password=object["Password"].toString();
    pM->lock();
    QString passwordFromDb=pMdb->selectUser(login);
    pM->unlock();

    if (password==passwordFromDb)
    {
        qDebug()<<"authorized";

        pM->lock();
        QList<QJsonObject> roomsList= pMdb->selectRooms(login);
        pM->unlock();
        QJsonObject jsonObject;
        jsonObject["Authorization_token"]="pass_from_server";
        QString authToken=jsonObject["Authorization_token"].toString();
        int count=1;

        for (auto i = roomsList.cbegin(), end = roomsList.cend(); i != end; ++i)
        {
            QString str_count=QString::number(count);
            jsonObject[str_count+"Room"]=(*i)["Id"].toString()+(*i)["Login"].toString();
            authToken+="_"+(*i)["Id"].toString()+(*i)["AccessToken"].toString();
            count++;
        }

        /*for(auto& el:roomsList)
        {
           QString str_count=QString::number(count);
           jsonObject[str_count+"Room"]=el;
           authToken+="_"+el;
           count++;
        }*/

        jsonObject["Authorization_token"]=authToken;
        response.setStatus(200, "OK");
        QJsonDocument document=QJsonDocument(jsonObject);
        QByteArray byte_array = document.toJson();
        response.setHeader("Content-Type", "application/json");
        response.write(byte_array, true);
    }
    else
    {
        response.setStatus(401, "Unauthorized");
    }
}

