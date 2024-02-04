#ifndef SYNCCONTROLLER_H
#define SYNCCONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
#include "mydatabase.h"
#include <QMutex>

using namespace stefanfrings;

//Контроллер вызываемый при запросе на синхронизацию
class SyncController: public HttpRequestHandler
{
    Q_OBJECT
public:
    SyncController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response, MyDatabase * pMdb, QMutex* pm);

signals:

public slots:

};

#endif // SYNCCONTROLLER_H
