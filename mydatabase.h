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
    bool userInsert(QString, QString); //Внесение пользователя
    QString selectUser (QString); //GetPassword
    bool selectMessage(int, QString, QString&);//Выбор сообщения
    void insertTestMessages(QString);
    void insertMessage(QString, QString, QString);//Внесение сообщения
    void insertRoom(QString roomID); //Внесение чата
    void insertUserRoom(QString user, int room, QString access_token=""); //Соотнесение пользователя и чата
    QList<QJsonObject> selectRooms(QString user); //Получение писка чата по пользователю
    bool findUser(QString userLogin);
    int selectRoom(); //Возвращает ID последней созданной комнаиты
    void createMessageTable(QString roomID);


};

#endif // MYDATABASE_H
