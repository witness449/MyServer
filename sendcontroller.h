#ifndef SENDCONTROLLER_H
#define SENDCONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
#include "mydatabase.h"
#include <QMutex>

using namespace stefanfrings;

//Контроллер вызываемый при поступлении сообщения
class SendController:public HttpRequestHandler
{
    Q_OBJECT
public:
    SendController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response, MyDatabase * pMdb, QMutex* pm);

signals:

public slots:

};

#endif // SENDCONTROLLER_H
