#include "clientstate.h"

ClientState::ClientState(QMutex* pm, QString login)
{
    pM=pm;
    this->login=login;
}


QString ClientState::getToken()
{
    return accessToken;
}

QMap<int, bool> ClientState::getActiveRooms()
{
    return rooms;
}
QMap<int, int> ClientState::getLastEvents()
{
    return events;
}


void ClientState::setToken(QString token)
{
    accessToken=token;
}

void ClientState::setRooms(MyDatabase *pMDB)
{
    pM->lock();
    rooms=pMDB->selectRoomsForState(login);
    pM->unlock();
}

void ClientState::setLastEvents(MyDatabase *pMDB)
{
    pM->lock();
    events=pMDB->selectTopMessages(login);
    pM->unlock();
}

ClientState& ClientState::operator=(const ClientState& cs)
{
    accessToken=cs.accessToken;
    rooms=cs.rooms;
    events=cs.events;

    return *this;

}

QJsonObject ClientState::toJson()
{
    QJsonObject objectRes;
    objectRes["AccessToken"]=accessToken;
    QJsonArray arRooms;
    QJsonArray arEvents;

    for (auto i=rooms.begin(); i!=rooms.end(); i++)
    {
        QJsonObject object;
        object["Id"]=i.key();
        object["IsActive"]=i.value();
        arRooms.append(object);
    }

    for (auto i=events.begin(); i!=events.end(); i++)
    {
        QJsonObject object;
        object["IdRoom"]=i.key();
        object["IdEvent"]=i.value();
        arEvents.append(object);
    }

    objectRes["RoomsState"]=arRooms;
    objectRes["EventsState"]=arEvents;

    return objectRes;
}

void ClientState::setRoomsFromJson(QJsonArray roomsArr)
{
    for (auto el:roomsArr)
     {
        rooms.insert(el.toObject()["Id"].toInt(), el.toObject()["IsActive"].toBool());
     }
}

void ClientState::setLastEventsFromJson(QJsonArray eventsArr)
{
    for (auto el:eventsArr)
     {
        events.insert(el.toObject()["IdRoom"].toInt(), el.toObject()["IdEvent"].toInt());
     }
}

bool ClientState::compareEvents(ClientState& other, int& eventId, int& roomId)
{
    for (auto i=events.begin(); i!=events.end(); i++)
    {
        auto it=other.events.find(i.key());
        if (it==other.events.end())
        {
            roomId=i.key(); //id пустой комнаты
            return false;
        }
        else if (*it==*i)
        {
            continue;
        }
        else if (*it!=*i)
        {
            eventId=*it;
            roomId=i.key();
            return false;
        }

    }
    eventId=-1;
    roomId=-1;
    return true;
}

bool ClientState::compareRooms(ClientState& other, int& roomId, int& roomBanId)
{
    for(auto i=rooms.begin(); i!=rooms.end(); i++)
    {
        auto it=other.rooms.find(i.key());
        if (it==other.rooms.end())
        {
            roomBanId=0;
            roomId=i.key();
            return false;
        }
    }

    for(auto i=other.rooms.begin(); i!=other.rooms.end(); i++)
    {
        auto it=rooms.find(i.key());
        if (it==rooms.end())
        {
            roomId=0;
            roomBanId=i.key();
            qDebug()<<"BAN"<<roomBanId;
            return false;
        }
    }


    roomId=-1;
    return true;
}
