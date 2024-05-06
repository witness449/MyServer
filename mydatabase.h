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
    //Работа с соединением
    void createConnection();
    //Работа с таблицами
    void createTable();
    void dropTable();
    void printTable();
public:
    MyDatabase();
    //Работа с данными
    //Работа с пользователями
    User selectUser (QString login);
    bool insertUser(User); //Внесение пользователя
    bool updateUser(User u);

    bool selectMessage(Message & m, QString roomId);//Выбор сообщения
    bool insertMessage(Message);//Внесение сообщения

    Room selectRoom(int roomID); //Выбор чата
    bool insertRoom(QString roomID); //Внесение чата
    bool insertUserRoom(QString user, int room, QString access_token=""); //Соотнесение пользователя и чата
    bool updateRoom(Room r); //Обновление чата

    bool insertBlackList(int idWhoBan, int idWhoBanned);//Внесение записи в таблицу BlackList
    bool deleteBlackList(int idWhoBan, int idWhoBanned);//Удаление записи из таблицы BlackList

    //Получение выборок
    QList<QJsonObject> const selectRooms(QString user); //Получение cписка чатов по идентификатору пользователя
    int const selectRoom(); //Получение ID последней созданной комнаиты
    bool const findUser(QString userLogin); //Получение признака того, что пользователь существует
    User const selectContact(QString client, int roomId); //Получение пользователя по идентифкатору чата и идентифкатору его собеседника
    QMap <int, bool> const selectRoomsForState(QString login); //Формирования данных об активных чатах пользовтаеля
    QMap<int, int> const selectTopMessages(QString login); //Формирование данных о последних сообщениях в чатах определенного пользователя
    void selectSyncMessage(int idRoom, int lastId, int& thisId, QString& text); //Запись идентифкатор сообщения и текста, по идентификатору предыдущего сообщения и идентифкатору чата
    void selectRoomByLogins(QString login1, QString login2, int&);//Получение идентификатора комнаты по идентификаторам собеседников
    bool const checkIfBan(int idWhoBan, int idWhoBanned); //Проверка того, внесен ли пользователь в черный сисок или нет
    bool const checkAccess(QString senderLogin, QString accessToken); //Проверка права доступа пользователя отправлять сообщение другому пользователю

};



#endif // MYDATABASE_H
