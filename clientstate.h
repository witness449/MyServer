#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H

#include <QString>
#include <QMap>
#include "mydatabase.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>

class ClientState
{
private:
    QString accessToken;
    QMap <int, bool> rooms;
    QMap<int, int> events;
    QString login;
    QMutex* pM;

public:
    ClientState(QMutex*, QString login);
    QString getToken();
    QMap<int, bool> getActiveRooms();
    QMap<int, int> getLastEvents();

    void setToken(QString token);
    void setRooms(MyDatabase* pMDB);
    void setLastEvents(MyDatabase* pMDB);

    void setRoomsFromJson(QJsonArray roomsArr);
    void setLastEventsFromJson(QJsonArray eventsArr);

    ClientState& operator=(const ClientState& cs);
    QJsonObject toJson();

    bool compareEvents(ClientState& other, int&, int&);
    bool compareRooms(ClientState& other, int&, int&);

};
#endif // CLIENTSTATE_H
