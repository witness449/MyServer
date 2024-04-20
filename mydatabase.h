#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSQLError>
#include <QSqlRecord>
#include <QMultiMap>
#include <QJsonObject>
#include "user.h"
#include "message.h"
#include "room.h"

class MyDatabase
{
private:
    QSqlDatabase myDB;
    static int count;
public:
    MyDatabase();
    //Работа с соединением
    void createConnection();
    void closeConnection();

    //Работа с таблицами
    void createTable();
    void dropTable();
    void printTable();

    //Работа с данными
    //Работа с пользователями
    User selectUser (QString login);
    //QString selectAccessToken(QString login);
    bool insertUser(User); //Внесение пользователя
    bool updateUser(User u);

    bool selectMessage(Message & m, QString roomId);//Выбор сообщения
    bool insertMessage(Message);//Внесение сообщения

    Room selectRoom(int roomID);
    bool insertRoom(QString roomID); //Внесение чата
    bool insertUserRoom(QString user, int room, QString access_token=""); //Соотнесение пользователя и чата
    bool updateRoom(Room r);

    bool insertBlackList(int idWhoBan, int idWhoBanned);
    bool deleteBlackList(int idWhoBan, int idWhoBanned);



    //Получение выборок
    QList<QJsonObject> const selectRooms(QString user); //Получение писка чата по пользователю
    int const selectRoom(); //Возвращает ID последней созданной комнаиты
    bool const findUser(QString userLogin);
    User const selectContact(QString client, int roomId);
    QMap <int, bool> const selectRoomsForState(QString login);
    QMap<int, int> const selectTopMessages(QString login);
    void selectSyncMessage(int idRoom, int lastId, int& thisId, QString& text);
    void selectRoomByLogins(QString login1, QString login2, int&);
    bool const userExists(QString login);
    bool const checkIfBan(int idWhoBan, int idWhoBanned);
    bool const checkAccess(QString senderLogin, QString accessToken);



};



#endif // MYDATABASE_H
