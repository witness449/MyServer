#include "mydatabase.h"
#include <QFile>

int MyDatabase::count=0;

MyDatabase::MyDatabase(){}

void MyDatabase::createConnection(){
    QFile file ("dbconfig.txt");

    file.open(QIODevice::ReadOnly);

    QString databaseName;
    QString hostName;
    QString appUser;
    QString appPassword;

    QTextStream in(&file);

    databaseName=in.readLine();
    hostName=in.readLine();
    appUser=in.readLine();
    appPassword=in.readLine();

    file.close();

    count++;
    QString conn(count);
    myDB= QSqlDatabase::addDatabase("QODBC", conn);
    myDB.setDatabaseName(databaseName);
    myDB.setHostName(hostName);
    myDB.setPort(-1);
    bool b=myDB.open(appUser, appPassword);
    qDebug()<<"Database opened: "<<b;
}

void MyDatabase::closeConnection(){
    myDB.close();
    qDebug()<<"Database is closed";
}

void MyDatabase::createTable(){
    if(myDB.isValid()){
    /*QSqlQuery query(myDB);
    QString create ="CREATE TABLE Users (Id INTEGER  PRIMARY KEY IDENTITY (1,1), Login  VARCHAR(50) NOT NULL, Password VARCHAR(50) NOT NULL, DisplayName VARCHAR (50))
";
    qDebug()<<"Create table status: "<<query.exec(create);
    qDebug()<<myDB.tables();*/

    QSqlQuery query5(myDB);
    QString create5 ="CREATE TABLE TestMessagess1 (id INTEGER  PRIMARY KEY IDENTITY (1,1), text  VARCHAR (255))";
    qDebug()<<"Create table status: "<<query5.exec(create5);
    qDebug()<<myDB.tables();


    /*QSqlQuery query3(myDB);
    QString create3 ="CREATE TABLE Roomss (id INTEGER  PRIMARY KEY IDENTITY(1,1), text  VARCHAR(20))";
    qDebug()<<"Create table status: "<<query3.exec(create3);
    qDebug()<<myDB.tables();

    QSqlQuery query4(myDB);
    QString create4 ="CREATE TABLE RoomssClientss (id INTEGER  PRIMARY KEY IDENTITY (1,1), Client  VARCHAR(20), Room VARCHAR(20), Access_token VARCHAR(50))";
    qDebug()<<"Create table status: "<<query4.exec(create4);
    qDebug()<<myDB.tables();*/
    }
    else{
        qDebug()<<"db is not valid";
    }
}

void MyDatabase:: dropTable(){
    if(myDB.isValid()){
    /*QSqlQuery query(myDB);
    QString drop="DROP TABLE Clients";
    query.exec(drop);
    qDebug()<<myDB.tables()<<"Should be only TestMessagess table";*/

    QSqlQuery query2(myDB);
    QString drop2="DROP TABLE TestMessagess1";
    query2.exec(drop2);
    qDebug()<<myDB.tables()<<"Should be no tables";

    /*QSqlQuery query3(myDB);
    QString drop3="DROP TABLE TestMessagess2";
    query3.exec(drop3);
    qDebug()<<myDB.tables()<<"Should be no tables";

    QSqlQuery query4(myDB);
    QString drop4="DROP TABLE TestMessagess3";
    query4.exec(drop4);
    qDebug()<<myDB.tables()<<"Should be no tables";

    QSqlQuery query5(myDB);
    QString drop5="DROP TABLE Roomss";
    query5.exec(drop5);
    qDebug()<<myDB.tables()<<"Should be no tables";

    QSqlQuery query6(myDB);
    QString drop6="DROP TABLE RoomssClientss";
    query6.exec(drop6);
    qDebug()<<myDB.tables()<<"Should be no tables";*/
    }
    else{
        qDebug()<<"db is not valid";
    }
}

void MyDatabase::printTable(){
    if (myDB.isValid()){
    QSqlQuery query(myDB);
    QString select="SELECT * FROM Users";
    qDebug()<<"Select query status: "<<query.exec(select);
    QSqlRecord rec =query.record();

    int id;
    QString login;
    QString password;

    while (query.next()){
        qDebug()<<"id: "<<query.value(rec.indexOf("Id")).toInt()
        <<"login: "<<query.value(rec.indexOf("Login")).toString()
        <<"password: "<<query.value(rec.indexOf("Password")).toString();
        }

    std::vector<QString> rooms;

    QSqlQuery query3(myDB);
    QString select3="SELECT * FROM Rooms";
    query3.exec(select3);
    QSqlRecord rec3=query3.record();

    while (query3.next()){
        qDebug()<<"RoomId: "<<query3.value(rec3.indexOf("Id")).toInt()
        <<"text: "<<query3.value(rec3.indexOf("Name")).toString();
        rooms.push_back(query3.value(rec3.indexOf("Name")).toString());
        }

    QSqlQuery query4(myDB);
    QString select4="SELECT * FROM RoomssUsers";
    query4.exec(select4);
    QSqlRecord rec4=query4.record();

    while (query4.next()){
        qDebug()<<"RoomUserId: "<<query4.value(rec4.indexOf("Id")).toInt()
        <<"User: "<<query4.value(rec4.indexOf("IdUser")).toInt()
         <<"Room: "<<query4.value(rec4.indexOf("IdRoom")).toInt()
         <<"Token: "<<query4.value(rec4.indexOf("AccessToken")).toString();
    }

    for(auto& room:rooms){
        qDebug()<<"Room: "<<room;
        QSqlQuery query2(myDB);
        QString select2="SELECT * FROM Events"+room;
        qDebug()<<"Select query status: "<<query2.exec(select2);
        QSqlRecord rec2 =query2.record();
        QString text;

        while (query2.next()){
            qDebug()<<"Id: "<<query2.value(rec2.indexOf("Id")).toInt()
                      <<"Content: "<<query2.value(rec2.indexOf("Content")).toString();
        }
    }
    }
    else
        qDebug()<<"db is not valid";
}

bool MyDatabase::userInsert(QString login, QString password){
    QString selectExists="SELECT COUNT(1) FROM Users WHERE Login='"+login+"'";
    QSqlQuery query(myDB);
    query.prepare(selectExists);
    query.exec();
    query.next();
    bool res1;
    bool res2;
    res1=query.value(0).toBool();
    if(!res1){
        if (myDB.isValid()){
            QString insert ="INSERT INTO Users(Login, Password) VALUES ('"+login+"','"+password+"')";
            query.prepare(insert);
            query.exec(insert);
            query.prepare(selectExists);
            query.exec();
            query.next();
            res2=query.value(0).toBool();
        }
     }
    if (!res1&&res2){return true;}
    else {return false;}
}

QString MyDatabase::selectUser(QString user)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString selectUser="SELECT * FROM Users WHERE Login='"+user+"'";
        qDebug()<<"Select query status: "<<query.exec(selectUser);
        query.next();
        QSqlRecord rec =query.record();
        QString password=query.value(rec.indexOf("password")).toString();
        qDebug()<<"password: "<<password;
        return password;
    }
}

void MyDatabase::insertTestMessages(QString str)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString insert ="INSERT INTO TestMessagess"+str+"(text) VALUES ('test message')";
        bool res=query.exec(insert);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
        this->printTable();
    }
}

bool MyDatabase::selectMessage(int messageId, QString roomId, QString& text)
{
    QSqlQuery query(myDB);
    QString message_id=QString::number(messageId);
    QString selectMessage="SELECT * FROM Events"+roomId+" WHERE Id="+message_id;
    query.exec(selectMessage);
    QSqlRecord rec =query.record();
    bool res;
    while (query.next()){
        qDebug()<<"Id: "<<query.value(rec.indexOf("Id")).toInt()
        <<"Content: "<<query.value(rec.indexOf("Content")).toString();
        text=query.value(rec.indexOf("Content")).toString();
        }
    QString selectExists="SELECT COUNT(1) FROM Events"+roomId+" WHERE Id='"+message_id+"'";
    query.prepare(selectExists);
    query.exec();
    query.next();
    res=query.value(0).toBool();
    qDebug()<<message_id;
    return res;
}

void MyDatabase::insertMessage(QString message, QString str)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString insert ="INSERT INTO Events"+str+" (Content, IdRoom) VALUES ('"+message+"', "+str+")";
        bool res=query.exec(insert);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
        this->printTable();
    }
}

void MyDatabase::insertRoom(QString text)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString insert ="INSERT INTO Rooms(Name) VALUES ('"+text+"')";
        bool res=query.exec(insert);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
        this->printTable();
    }
}

void MyDatabase::insertUserRoom(QString User, int Room, QString access_token)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString insert ="INSERT INTO RoomssUsers (IdUser, IdRoom, AccessToken) VALUES ((SELECT Id FROM Users WHERE Login='"+User+"'), "+Room+",'"+access_token+"')";
        bool res=query.exec(insert);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
     }
}

QList<QString> MyDatabase::selectRooms(QString user)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString selectRooms="SELECT * FROM RoomssUsers WHERE IdUser =(SELECT Id FROM Users WHERE Login='"+user+"')";
        bool res=query.exec(selectRooms);
        QSqlRecord rec =query.record();

        QList<QString> RoomsList;

        while (query.next()){
            RoomsList.append(QString::number(query.value(rec.indexOf("IdRoom")).toInt()));
            }

        return RoomsList;
    }
}


bool MyDatabase:: findUser(QString userLogin)
{
    QSqlQuery query(myDB);
    QString selectExists="SELECT COUNT(1) FROM Users WHERE Login='"+userLogin+"'";
    query.prepare(selectExists);
    query.exec();
    query.next();
    bool res=query.value(0).toBool();
    return res;
}

int MyDatabase::selectRoom()
{
    QSqlQuery query(myDB);
    QString selectTop="SELECT TOP (1) Id FROM Rooms ORDER BY Id DESC";
    query.prepare(selectTop);
    query.exec();
    query.next();
    int res=query.value(0).toInt();
    return res;
}

void MyDatabase::createMessageTable(QString roomID)
{
    QSqlQuery query(myDB);
    QString create="CREATE TABLE Events"+roomID+" (Id INTEGER  PRIMARY KEY IDENTITY (1,1), IdRoom INTEGER NOT NULL, Content NVARCHAR(255) NOT NULL, IdSender INTEGER, TimeStamp DATETIME)";
    //QString create ="CREATE TABLE TestMessagess"+roomID+" (id INTEGER  PRIMARY KEY IDENTITY (1,1), text  VARCHAR (255))";
    query.exec(create);
}
