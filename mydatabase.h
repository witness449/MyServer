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
    void insertMessage(Message);//Внесение сообщения

    void insertRoom(QString roomID); //Внесение чата
    void insertUserRoom(QString user, int room, QString access_token=""); //Соотнесение пользователя и чата
    void updateRoom(Room r);



    //Получение выборок
    QList<QJsonObject> selectRooms(QString user); //Получение писка чата по пользователю
     int selectRoom(); //Возвращает ID последней созданной комнаиты
    bool findUser(QString userLogin);
    QString selectContact(QString client, int roomId);
    QMap <int, bool> selectRoomsForState(QString login);
    QMap<int, int> selectTopMessages(QString login);
    void selectSyncMessage(int idRoom, int lastId, int& thisId, QString& text);
    void selectRoomByLogins(QString login1, QString login2, int&);
    bool userExists(QString login);


};

#endif // MYDATABASE_H
