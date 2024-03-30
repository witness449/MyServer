#ifndef UNBANCONTROLLER_H
#define UNBANCONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
#include "mydatabase.h"
#include <QMutex>

using namespace stefanfrings;

class UnbanController :  public HttpRequestHandler
{
    Q_OBJECT
public:
    UnbanController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response, MyDatabase * pMdb, QMutex* pm);


signals:

public slots:

};

#endif // UNBANCONTROLLER_H
