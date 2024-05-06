#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

using namespace stefanfrings;

namespace Ui {
class MainWindow;
}

//Данный класс сохранен на случай развития сервера и добавления интерфейса администратора
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //Указатель на объект взаимодействия с БД
    MyDatabase* pMyDB;
    //Обеспечение синхронизации
    QMutex m;
    //Отладночный таймер
    QTimer* testMessageInsertTimer;


private slots:

};

#endif // MAINWINDOW_H
