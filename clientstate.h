#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H

#include <QString>
#include <QMap>
#include "mydatabase.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>


//Класс, икапсулирующий состояние клиента
class ClientState
{
private:
    //Токен доступа
    QString accessToken;
    //Структура с информацией об активных комнатах
    QMap <int, bool> rooms;
    //Структура с информацией об идентифкаторах последних сообщений в определенных чатах
    QMap<int, int> events;
    //Идентфикатор клиента
    QString login;
    QMutex* pM;

public:
    ClientState(QMutex*, QString login);
    //Метод получения доекна авторизации
    QString getToken();
    //Метод получения информации об активных комнатах
    QMap<int, bool> const getActiveRooms();
    //Метод получения информации об идентифкаторах последних сообщений в определенных чатах
    QMap<int, int> const getLastEvents();

    //Метод установки токена доступа
    void setToken(QString token);
    //Метод установки информации об активных комнатах из базы данных
    void setRooms(MyDatabase* pMDB);
    //метод устновки информации о последних сообщениях из базы данных
    void setLastEvents(MyDatabase* pMDB);
    //Метод установки информации об активных комнатах из JSON файла
    void setRoomsFromJson(QJsonArray roomsArr);
    //Метод устновки информации о последних сообщениях из JSON файла
    void setLastEventsFromJson(QJsonArray eventsArr);

    ClientState& operator=(const ClientState& cs);
    QJsonObject toJson();

    //Метод сравнения двух объектов типа CLientState по данным о последних сообщениях
    bool compareEvents(const ClientState& other, int&, int&);
    //Метод сравнения двух объектов типа CLientState по данным об активных чатах
    bool compareRooms(const ClientState& other, int&, int&);

};
#endif // CLIENTSTATE_H
