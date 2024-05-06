#ifndef BANCONTROLLER_H
#define BANCONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
#include "mydatabase.h"
#include <QMutex>

using namespace stefanfrings;

//Контроллер вызываемый при запросе на внесение в черный список
class BanController :  public HttpRequestHandler
{
    Q_OBJECT
public:
    BanController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response, MyDatabase * pMdb, QMutex* pm);

signals:

public slots:

};

#endif // BANCONTROLLER_H
