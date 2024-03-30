#include "authcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMultiMap>
#include <QDebug>
#include <QJsonArray>
#include "myrequestmapper.h"

AuthController::AuthController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void AuthController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    sessionStore->getSession(request,response,true);
    QString sessionIdTmp=QString(sessionStore->getSession(request,response,true).getId());

    if (request.getMethod()=="GET")
    {
        response.setHeader("Request-type", "AuthentificationStage1");

        response.setStatus(401, "Unauthorized");
        QJsonObject jsonObject;
        jsonObject["type"]="m.login.password";
        jsonObject["identifier"]="<login>";
        jsonObject["password"]="<password>";

        sessionId=sessionIdTmp;

        jsonObject["session"]=sessionId;

        QJsonDocument document=QJsonDocument(jsonObject);
        QByteArray byte_array = document.toJson();
        response.setHeader("Content-Type", "application/json");
        response.write(byte_array, true);

    }


    else if (request.getMethod()=="POST")
    {
    response.setHeader("Request-type", "AuthentificationStage2");
    QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
    QJsonObject object=doc.object();

    QString login=object["identifier"].toString();
    QString password=object["password"].toString();
    QString sessId=object["session"].toString();

    if (object["session"].toString()==sessionId)
    {
    pM->lock();
    QString passwordFromDb=pMdb->selectUser(login).Password;
    pM->unlock();

    if (password==passwordFromDb)
    {
        qDebug()<<"authorized";

        pM->lock();
        QList<QJsonObject> roomsList= pMdb->selectRooms(login);
        pM->unlock();
        QJsonObject jsonObject;
        //jsonObject["Authorization_token"]="pass_from_server";
        //QString authToken=jsonObject["Authorization_token"].toString();
        int count=1;
        QJsonArray RoomsArray;

        for (auto i = roomsList.cbegin(), end = roomsList.cend(); i != end; ++i)
        {
            QString str_count=QString::number(count);
            jsonObject[str_count+"Room"]=(*i)["Id"].toString()+" "+(*i)["Login"].toString();
            //authToken+="_"+(*i)["Id"].toString()+" "+(*i)["AccessToken"].toString();
            QJsonObject roomObject;
            roomObject["id"]=(*i)["Id"];
            roomObject["login"]=(*i)["Login"].toString();
            RoomsArray.append(roomObject);
            count++;
        }

        /*for(auto& el:roomsList)
        {
           QString str_count=QString::number(count);
           jsonObject[str_count+"Room"]=el;
           authToken+="_"+el;
           count++;
        }*/

        //QString authToken=MyRequestMapper::makeAccessToken(login, roomsList);

        pM->lock();
        QString authToken=pMdb->selectUser(login).AccessToken;
        pM->unlock();

        jsonObject["Rooms"]=RoomsArray;
        jsonObject["Authorization_token"]=authToken;
        response.setStatus(200, "OK");
        QJsonDocument document=QJsonDocument(jsonObject);
        QByteArray byte_array = document.toJson();
        response.setHeader("Content-Type", "application/json");
        response.write(byte_array, true);
    }
    }
    else
    {
        response.setStatus(403, "Forbidden");
    }
    }
    else
    {
        response.setStatus(400, "Unknown");
    }
}

