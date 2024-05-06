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
    QString authToken=request.getHeader("access_token");
    QString login;
    QMap<int, QString> roomsTokens;

    roomsTokens=MyRequestMapper::tokenParse(authToken, login);
    QString tmpToken;

    pM->lock();
    tmpToken=pMdb->selectUser(login).accessToken;
    pM->unlock();

    if (tmpToken==authToken)
    {
        QString text;
        bool b=true;

        QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
        QJsonObject object=doc.object();

        ClientState csClient (pM, login);
        csClient.setToken(authToken);
        csClient.setRoomsFromJson(object["RoomsState"].toArray());
        csClient.setLastEventsFromJson(object["EventsState"].toArray());

        int ROOMid;
        int thisId=0;

        int newRoomId;
        int banRoomId;

        bool newEvent=true;

        while(b){
            int lastId=-1;
            ClientState csServer (pM, login);
            csServer.setToken(authToken);
            csServer.setLastEvents(pMdb);
            csServer.setRooms(pMdb);

            b=csServer.compareEvents(csClient, lastId, ROOMid);
            if (!b){
                pM->lock();
                //b=pMdb->selectMessage(id, roomId, text);
                pMdb->selectSyncMessage(ROOMid, lastId, thisId, text); //ЗАПИСАТЬ ТЕКУЩИЙ ID
                pM->unlock();
                qDebug()<<text;
            }
            if (b){
                b=csServer.compareRooms(csClient, newRoomId, banRoomId);
                if(!b){
                    newEvent=false;
                }
            }
            if (b){
                std::chrono::milliseconds ms(30);
                std::this_thread::sleep_for(ms);
            }
        }


        QJsonObject jsonObject;

        if(newEvent){
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
            int count=1;
            QJsonArray RoomsArray;

            for (auto i = roomsList.cbegin(), end = roomsList.cend(); i != end; ++i)
            {
                QString str_count=QString::number(count);
                QJsonObject roomObject;
                roomObject["id"]=(*i)["Id"];
                roomObject["login"]=(*i)["Login"].toString();
                RoomsArray.append(roomObject);
                count++;
            }

            //Проверить изменение токена для забанненного
            QString authToken=MyRequestMapper::makeAccessToken(login, roomsList);
            QString roomName;

            pM->lock();
            User u;
            u.login=login;
            u.accessToken=authToken;
            pMdb->updateUser(u);
            roomName=pMdb->selectContact(login, newRoomId).login;
            pM->unlock();

            jsonObject["type"]=1;
            if (newRoomId){
                jsonObject["idRoom"]=newRoomId;
                jsonObject["banRoom"]="0";
            }
            if (banRoomId){
                jsonObject["banRoom"]=banRoomId;
                jsonObject["idRoom"]="0";
            }

            jsonObject["RoomName"]=roomName;
            jsonObject["access_token"]=authToken;

        }


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
