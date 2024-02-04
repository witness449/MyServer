#ifndef REGCONTROLLER_H
#define REGCONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
#include "mydatabase.h"
#include <QMutex>

using namespace stefanfrings;

//Контроллер вызываемый при запросе на регистрацию
class RegController: public HttpRequestHandler
{
    Q_OBJECT
public:
    RegController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response, MyDatabase * pMdb, QMutex* pm);

signals:

public slots:
};

#endif // REGCONTROLLER_H
