#include "sendcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>

SendController::SendController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void SendController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    QJsonDocument doc=QJsonDocument::fromJson(request.getBody());
    QJsonObject object=doc.object();
    Message m;
    m.content=object["message"].toString();
    m.idRoom=object["room_id"].toString();
    m.senderLogin=object["login"].toString();
    pM->lock();
    pMdb->insertMessage(m);
    pM->unlock();
}
