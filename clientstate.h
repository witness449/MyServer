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
    QString AccessToken;
    QMap <int, bool> Rooms;
    QMap<int, int> Events;
    QString Login;
    QMutex* pM;

public:
    ClientState(QMutex*, QString login);
    QString GetToken();
    QMap<int, bool> GetActiveRooms();
    QMap<int, int> GetLastEvents();

    void SetToken(QString token);
    void SetRooms(MyDatabase* pMDB);
    void SetLastEvents(MyDatabase* pMDB);

    void SetRoomsFromJson(QJsonArray roomsArr);
    void SetLastEventsFromJson(QJsonArray eventsArr);

    ClientState& operator=(const ClientState& cs);
    QJsonObject ToJson();

    bool compareEvents(ClientState& other, int&, int&);
    bool compareRooms(ClientState& other, int&);

};
#endif // CLIENTSTATE_H
