#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSQLError>
#include <QSqlRecord>

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
    bool clientInsert(QString, QString); //Внесение пользователя
    QString selectClient (QString);
    bool selectMessage(int, QString, QString&);//Выбор сообщения
    void insertTestMessages(QString);
    void insertMessage(QString, QString);//Внесение сообщения
    void insertRoom(QString roomID); //Внесение чата
    void insertClientRoom(QString client, QString room, QString access_token=""); //Соотнесение пользователя и чата
    QList<QString> selectRooms(QString client); //Получение писка чата по пользователю

};

#endif // MYDATABASE_H
