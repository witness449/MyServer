#include "mycontroller.h"

MyController::MyController(QObject *parent) :
    HttpRequestHandler(parent)
{
}

void MyController::service(HttpRequest &request, HttpResponse &response) {
    response.setHeader("Content-Type", "application/json");
    response.write("{ \"id\": 1, \"title\": \"Hello World!\"}");
}
