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
    bool userInsert(QString, QString); //Внесение пользователя
    QString selectUser (QString);
    bool selectMessage(int, QString, QString&);//Выбор сообщения
    void insertTestMessages(QString);
    void insertMessage(QString, QString);//Внесение сообщения
    void insertRoom(QString roomID); //Внесение чата
    void insertUserRoom(QString user, int room, QString access_token=""); //Соотнесение пользователя и чата
    QList<QString> selectRooms(QString user); //Получение писка чата по пользователю
    bool findUser(QString userLogin);
    int selectRoom();
    void createMessageTable(QString roomID);

};

#endif // MYDATABASE_H
