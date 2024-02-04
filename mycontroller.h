#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"

using namespace stefanfrings;

//Заготовка класса контроллера
class MyController : public HttpRequestHandler
{
    Q_OBJECT
public:
    MyController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response);

signals:

public slots:

};

#endif // MYCONTROLLER_H
