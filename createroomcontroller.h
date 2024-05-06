#ifndef CREATEROOMCONTROLLER_H
#define CREATEROOMCONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
#include "mydatabase.h"
#include <QMutex>

using namespace stefanfrings;

//Контроллер вызываемый при запросе на создание чата
class CreateRoomController :  public HttpRequestHandler
{
    Q_OBJECT
public:
    CreateRoomController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response, MyDatabase * pMdb, QMutex* pm);

signals:

public slots:

};

#endif // CREATEROOMCONTROLLER_H
