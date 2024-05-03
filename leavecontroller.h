#ifndef LEAVECONTROLLER_H
#define LEAVECONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
#include "mydatabase.h"
#include <QMutex>

using namespace stefanfrings;

class LeaveController :  public HttpRequestHandler
{
    Q_OBJECT
public:
    LeaveController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response, MyDatabase * pMdb, QMutex* pm);


signals:

public slots:

};

#endif // BANCONTROLLER_H
