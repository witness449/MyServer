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
        if (object["session"].toString()==sessionId){
            pM->lock();
            QString passwordFromDb=pMdb->selectUser(login).password;
            pM->unlock();
            if (password==passwordFromDb){
                pM->lock();
                QList<QJsonObject> roomsList= pMdb->selectRooms(login);
                pM->unlock();
                QJsonObject jsonObject;
                QJsonArray RoomsArray;

                for (auto i = roomsList.cbegin(), end = roomsList.cend(); i != end; ++i){
                    QJsonObject roomObject;
                    roomObject["id"]=(*i)["Id"];
                    roomObject["login"]=(*i)["Login"].toString();
                    RoomsArray.append(roomObject);
                }

                //QString authToken=MyRequestMapper::makeAccessToken(login, roomsList);
                pM->lock();
                QString authToken=pMdb->selectUser(login).accessToken;
                pM->unlock();

                jsonObject["rooms"]=RoomsArray;
                jsonObject["access_token"]=authToken;
                response.setStatus(200, "OK");

                QJsonDocument document=QJsonDocument(jsonObject);
                QByteArray byte_array = document.toJson();
                response.setHeader("Content-Type", "application/json");
                response.write(byte_array, true);
            }
            else{
                response.setStatus(403, "Forbidden");
            }

        }
        else{
            response.setStatus(403, "Forbidden");
        }
    }
    else{
        response.setStatus(400, "Unknown");
    }
}

