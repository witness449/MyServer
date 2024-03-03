#include "synccontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <thread>

SyncController::SyncController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void SyncController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    QString authToken=request.getHeader("Auth_token");
    if (authToken.mid(0, 16)=="pass_from_server")
    {
        QString roomId=authToken.mid(17, 1);
        QString str_id=request.getHeader("Id");
        QString login=request.getHeader("Login");
        int id=str_id.toInt();
        QString text;
        bool b=false;
        while(!b){
            pM->lock();
            b=pMdb->selectMessage(id, roomId, text);
            pM->unlock();
            if (!b){
                std::chrono::milliseconds ms(30);
                std::this_thread::sleep_for(ms);
                }
            }

        pM->lock();
        QList<QJsonObject> roomsList= pMdb->selectRooms(login);
        pM->unlock();
        QJsonObject jsonObject;
        jsonObject["Authorization_token"]="pass_from_server";
        QString authToken=jsonObject["Authorization_token"].toString();
        int count=1;
        QJsonArray RoomsArray;

        for (auto i = roomsList.cbegin(), end = roomsList.cend(); i != end; ++i)
        {
            QString str_count=QString::number(count);
            jsonObject[str_count+"Room"]=(*i)["Id"].toString()+" "+(*i)["Login"].toString();
            authToken+="_"+(*i)["Id"].toString()+" "+(*i)["AccessToken"].toString();
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

        jsonObject["Rooms"]=RoomsArray;
        jsonObject["Authorization_token"]=authToken;

        //QJsonObject jsonObject;
        jsonObject["one"]=text;
        response.setStatus(200, "OK");
        QJsonDocument document=QJsonDocument(jsonObject);
        QByteArray byte_array = document.toJson();
        response.setHeader("Content-Type", "application/json");
        response.write(byte_array, true);
      }
    else{
        response.setStatus(401, "Unauthorized");
    }
}
