#include "mainwindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QSettings>
#include "httplistener.h"
#include "httprequesthandler.h"
#include "httpsessionstore.h"
#include "myrequestmapper.h"
#include "mydatabase.h"
#include "global.h"
#include <QTimer>
#include <QMutex>
#include <QCoreApplication>

using namespace stefanfrings;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //MainWindow w;
    //w.show();

    //Указатель на объект взаимодействия с БД
    MyDatabase* pMyDB;
    //Обеспечение синхронизации
    QMutex m;



    QSettings* sessionSettings=new QSettings(path, QSettings::IniFormat);
    sessionSettings->beginGroup("sessions");
    sessionStore=new stefanfrings::HttpSessionStore(sessionSettings);
    QSettings* listenerSettings=new QSettings(path,QSettings::IniFormat);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings,new MyRequestMapper(pMyDB, &m));

    return a.exec();
}
