#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //pMyDB=new MyDatabase();
    //pMyDB->createConnection();
    //pMyDB->dropTable();
    //pMyDB->createTable();
    //pMyDB->clientInsert("Alice", "12345");
    //pMyDB->printTable();

    //Запуск библиотечных функций для работы веб сервера (запуск сервера)
    QSettings* sessionSettings=new QSettings("D:/HttpQt/MyServer/MyServer/MyServer/etc/webapp1.ini", QSettings::IniFormat,this);
    sessionSettings->beginGroup("sessions");
    HttpSessionStore* sessionStore=new stefanfrings::HttpSessionStore(sessionSettings,this);
    QSettings* listenerSettings=new QSettings("D:/HttpQt/MyServer/MyServer/MyServer/etc/webapp1.ini",QSettings::IniFormat,this);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings,new MyRequestMapper(this, pMyDB, &m),this);

    /*testMessageInsertTimer=new QTimer(this);
    testMessageInsertTimer->setInterval(10000);
    connect(testMessageInsertTimer, SIGNAL(timeout()), this, SLOT(dbInsertTestMessages()));
    testMessageInsertTimer->start();*/
    //pMyDB->insertTestMessages("1");
    //pMyDB->insertTestMessages("2");
    //pMyDB->insertTestMessages("3");
    //pMyDB->insertRoom("1");
    //pMyDB->insertRoom("2");
    //pMyDB->insertRoom("3");
    //pMyDB->insertUserRoom("Alice", "1");
    //pMyDB->insertUserRoom("Alice", "2");
    //pMyDB->insertUserRoom("Bob", "1");
    //pMyDB->insertUserRoom("Eva", "2");
}

MainWindow::~MainWindow()
{
    //pMyDB->dropTable();
    //pMyDB->closeConnection();
    delete pMyDB;
    //QSqlDatabase::removeDatabase("MyServerDB connection");
    delete ui;
}

