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
    QString message=object["message"].toString();
    QString roomId=object["room_id"].toString();
    pM->lock();
    pMdb->insertMessage(message, roomId);
    pM->unlock();
}
