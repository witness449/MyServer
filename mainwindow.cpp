#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :QMainWindow()
    //QMainWindow(parent),ui(new Ui::MainWindow)
{
    //ui->setupUi(this);

     //Запуск библиотечных функций для работы веб сервера (запуск сервера)
    QSettings* sessionSettings=new QSettings("D:/HttpQt/MyServer/MyServer/MyServer/etc/webapp1.ini", QSettings::IniFormat,this);
    sessionSettings->beginGroup("sessions");
    sessionStore=new stefanfrings::HttpSessionStore(sessionSettings,this);
    QSettings* listenerSettings=new QSettings("D:/HttpQt/MyServer/MyServer/MyServer/etc/webapp1.ini",QSettings::IniFormat,this);
    listenerSettings->beginGroup("listener");
    //new HttpListener(listenerSettings,new MyRequestMapper(this, pMyDB, &m),this);
   }

MainWindow::~MainWindow()
{

    delete pMyDB;
    //QSqlDatabase::removeDatabase("MyServerDB connection");
    //delete ui;
}

