#include "synccontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <thread>
#include "myrequestmapper.h"
#include "clientstate.h"

SyncController::SyncController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void SyncController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    QString authToken=request.getHeader("Auth_token");
    QString login;
    QMap<int, QString> roomsTokens;

    roomsTokens=MyRequestMapper::TokenParse(authToken, login);
    QString tmpToken;

    pM->lock();
    tmpToken=pMdb->selectUser(login).AccessToken;
    pM->unlock();

    if (tmpToken==authToken)
    {
        QString roomId=authToken.mid(17, 1);
        QString str_id=request.getHeader("Id");
        //QString login=request.getHeader("Login");
        int id=str_id.toInt();
        QString text;
        bool b=true;

        QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
        QJsonObject object=doc.object();

        ClientState csClient (pM, login);
        csClient.SetToken(authToken);
        csClient.SetRoomsFromJson(object["RoomsState"].toArray());
        csClient.SetLastEventsFromJson(object["EventsState"].toArray());


           int ROOMid;
            int thisId=0;

            int newRoomId;

            bool newEvent=true;



        while(b){


            int lastId=-1;

        ClientState csServer (pM, login);
        csServer.SetToken(authToken);
        csServer.SetLastEvents(pMdb);
        csServer.SetRooms(pMdb);




        b=csServer.compareEvents(csClient, lastId, ROOMid);
        if (!b)
        {
        pM->lock();
       //b=pMdb->selectMessage(id, roomId, text);
        pMdb->selectSyncMessage(ROOMid, lastId, thisId, text); //ЗАПИСАТЬ ТЕКУЩИЙ ID
        pM->unlock();
        qDebug()<<text;
        }
        if (b)
        {
            b=csServer.compareRooms(csClient, newRoomId);
            if(!b)
            {
                newEvent=false;
            }
        }





        if (b){
            std::chrono::milliseconds ms(30);
            std::this_thread::sleep_for(ms);
            }

        }

        /*pM->lock();
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
        QString authToken=MyRequestMapper::makeAccessToken(login, roomsList);*/

        /*for(auto& el:roomsList)
        {
           QString str_count=QString::number(count);
           jsonObject[str_count+"Room"]=el;
           authToken+="_"+el;
           count++;
        }*/

        /*jsonObject["Rooms"]=RoomsArray;
        jsonObject["Authorization_token"]=authToken;*/

        QJsonObject jsonObject;

        if(newEvent)
        {
        jsonObject["type"]=0;
        jsonObject["Id"]=thisId;
        jsonObject["IdRoom"]=ROOMid;
        jsonObject["Content"]=text;
        }

        else
        {
            pM->lock();
            QList<QJsonObject> roomsList= pMdb->selectRooms(login);
            pM->unlock();
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

            QString authToken=MyRequestMapper::makeAccessToken(login, roomsList);
            QString roomName;

            pM->lock();
            User u;
            u.Login=login;
            u.AccessToken=authToken;
            pMdb->updateUser(u);
            roomName=pMdb->selectContact(login, newRoomId);
            pM->unlock();

            jsonObject["type"]=1;
            jsonObject["idRoom"]=newRoomId;
            jsonObject["RoomName"]=roomName;
            jsonObject["Authorization_token"]=authToken;

        }


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
