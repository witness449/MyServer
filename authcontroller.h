#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
#include "httpsession.h"
#include "httpsessionstore.h"
#include "mydatabase.h"
#include <QMutex>
#include "global.h"

using namespace stefanfrings;

//Контроллер вызываемый при запросе на авторизацию
class AuthController: public HttpRequestHandler
{
     Q_OBJECT
public:
    AuthController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response, MyDatabase * pMdb, QMutex* pm);
private:
    QString sessionId;

signals:

public slots:

};

#endif // AUTHCONTROLLER_H
