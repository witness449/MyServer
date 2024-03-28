#include "clientstate.h"

ClientState::ClientState(QMutex* pm, QString login)
{
    pM=pm;
    Login=login;
}


QString ClientState::GetToken()
{
    return AccessToken;
}

QMap<int, bool> ClientState::GetActiveRooms()
{
    return Rooms;
}
QMap<int, int> ClientState::GetLastEvents()
{
    return Events;
}


void ClientState::SetToken(QString token)
{
    AccessToken=token;
}

void ClientState::SetRooms(MyDatabase *pMDB)
{
    pM->lock();
    Rooms=pMDB->selectRoomsForState(Login);
    pM->unlock();
}

void ClientState::SetLastEvents(MyDatabase *pMDB)
{
    pM->lock();
    Events=pMDB->selectTopMessages(Login);
    pM->unlock();
}

ClientState& ClientState::operator=(const ClientState& cs)
{
    AccessToken=cs.AccessToken;
    Rooms=cs.Rooms;
    Events=cs.Events;

    return *this;

}

QJsonObject ClientState::ToJson()
{
    QJsonObject objectRes;
    objectRes["AccessToken"]=AccessToken;
    QJsonArray arRooms;
    QJsonArray arEvents;

    for (auto i=Rooms.begin(); i!=Rooms.end(); i++)
    {
        QJsonObject object;
        object["Id"]=i.key();
        object["IsActive"]=i.value();
        arRooms.append(object);
    }

    for (auto i=Events.begin(); i!=Events.end(); i++)
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

void ClientState::SetRoomsFromJson(QJsonArray roomsArr)
{
    for (auto el:roomsArr)
     {
        Rooms.insert(el.toObject()["Id"].toInt(), el.toObject()["IsActive"].toBool());
     }
}

void ClientState::SetLastEventsFromJson(QJsonArray eventsArr)
{
    for (auto el:eventsArr)
     {
        Events.insert(el.toObject()["IdRoom"].toInt(), el.toObject()["IdEvent"].toInt());
     }
}

bool ClientState::compareEvents(ClientState& other, int& eventId, int& roomId)
{
    for (auto i=Events.begin(); i!=Events.end(); i++)
    {
        auto it=other.Events.find(i.key());
        if (it==other.Events.end())
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

bool ClientState::compareRooms(ClientState& other, int& roomId)
{
    for(auto i=Rooms.begin(); i!=Rooms.end(); i++)
    {
        auto it=other.Rooms.find(i.key());
        if (it==other.Rooms.end())
        {
            roomId=i.key();
            return false;
        }
    }
    roomId=-1;
    return true;
}
