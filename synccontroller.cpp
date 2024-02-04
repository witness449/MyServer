#include "synccontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <thread>

SyncController::SyncController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void SyncController::service(HttpRequest &request, HttpResponse &response, MyDatabase* pMdb, QMutex* pM) {
    QString authToken=request.getHeader("Auth_token");
    if (authToken.mid(0, 16)=="pass_from_server")
    {
        QString roomId=authToken.mid(17, 1);
        QString str_id=request.getHeader("Id");
        int id=str_id.toInt();
        QString text;
        bool b=false;
        while(!b){
            pM->lock();
            b=pMdb->selectMessage(id, roomId, text);
            pM->unlock();
            if (!b){
                std::chrono::milliseconds ms(30);
                std::this_thread::sleep_for(ms);
                }
            }
        QJsonObject jsonObject;
        jsonObject["one"]=text;
        response.setStatus(200, "OK");
        QJsonDocument document=QJsonDocument(jsonObject);
        QByteArray byte_array = document.toJson();
        response.setHeader("Content-Type", "application/json");
        response.write(byte_array, true);
      }
    else{
        response.setStatus(401, "Unauthorized");
    }
}
