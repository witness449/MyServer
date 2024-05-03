#include "myrequestmapper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <thread>

MyRequestMapper::MyRequestMapper(MyDatabase* pMDB, QMutex* pm, QObject *parent) :
    HttpRequestHandler(parent)
{
    //pMdb=pMDB;
    pMdb=new MyDatabase();
    //pMdb->createConnection();
    //pMdb->dropTable();
    //pMdb->createTable();
    pM=new QMutex;

}

void MyRequestMapper::service(HttpRequest &request, HttpResponse &response){
    QByteArray path=request.getPath();
    if (path=="/" ) {
        myController.service(request, response);
    }
    else if(path=="/register"){
        regController.service(request, response, pMdb, pM);
    }
    else if(path=="/auth"){
        authController.service(request, response, pMdb, pM);
    }
    else if(path=="/sync"){
        syncController.service(request, response, pMdb, pM);
    }
    else if(path=="/send"){
        sendController.service(request, response, pMdb, pM);
    }
    else if(path=="/leave"){
        leaveController.service(request, response, pMdb, pM);
    }
    else if(path=="/unban"){
        unbanController.service(request, response, pMdb, pM);
    }
    else if (path=="/create_room"){
        createRoomController.service(request, response, pMdb, pM);
    }
    else if (path=="/ban"){
        banController.service(request, response, pMdb, pM);
    }
    else {
        response.setStatus(404, "Not_found");
    }
}

MyRequestMapper::~MyRequestMapper()
{
    pMdb->closeConnection();
}

QString MyRequestMapper::makeAccessToken(QString login, QList<QJsonObject> roomsList)
{
    QString authToken=login;
    for (auto i = roomsList.cbegin(), end = roomsList.cend(); i != end; ++i)
    {
        authToken+=" "+(*i)["Id"].toString()+" "+(*i)["AccessToken"].toString();
    }
    return authToken;
}

QMap<int, QString> MyRequestMapper::tokenParse(QString& accessToken, QString &login)
{
    QStringList pieces=accessToken.split(" ");
    login=pieces[0];
    QMap<int, QString> roomsTokens;
    for (int i=1; i<pieces.size(); i+=2)
    {
         roomsTokens.insert(pieces[i].toInt(), pieces[i+1]);
    }
    return roomsTokens;
}
