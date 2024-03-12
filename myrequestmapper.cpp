#include "myrequestmapper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <thread>

MyRequestMapper::MyRequestMapper(QObject *parent, MyDatabase* pMDB, QMutex* pm) :
    HttpRequestHandler(parent)
{
    //pMdb=pMDB;
    pMdb=new MyDatabase();
    pMdb->createConnection();
    //pMdb->dropTable();
    //pMdb->createTable();
    pM=new QMutex;

}

void MyRequestMapper::service(HttpRequest &request, HttpResponse &response){
    QByteArray path=request.getPath();
    if (path=="/" ) {
        myController.service(request, response);
    }
    else if(path=="/reg"){
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
    else if(path=="/stop"){

    }
    else if (path=="/create_room"){
        createRoomController.service(request, response, pMdb, pM);
    }
    else {
        response.setStatus(404, "Not found");
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
