#ifndef MYREQUESTMAPPER_H
#define MYREQUESTMAPPER_H

#include <QObject>
#include "httprequesthandler.h"
#include "mycontroller.h"
#include "regcontroller.h"
#include "authcontroller.h"
#include "synccontroller.h"
#include "sendcontroller.h"
#include "mydatabase.h"
#include <QMutex>
#include <QTcpSocket>


using namespace stefanfrings;

//Организация выбора контроллера в зависимости от конечной точки запроса
class MyRequestMapper : public HttpRequestHandler
{
    Q_OBJECT
private:
    RegController regController; //Контроллер по конечной точке /reg
    AuthController authController; //Контроллер по конечной точке /auth
    SyncController syncController; //Контроллер по конечной точке /sync
    SendController sendController; //Контроллер по конечной точке /send
    MyController myController;

    MyDatabase* pMdb;
    QMutex* pM;


public:
    explicit MyRequestMapper(QObject *parent = 0, MyDatabase* pMDB=NULL, QMutex* pm=NULL);
    void service(HttpRequest& request, HttpResponse& response);
    ~MyRequestMapper();

signals:

public slots:

};

#endif // MYREQUESTMAPPER_H
