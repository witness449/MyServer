#-------------------------------------------------
#
# Project created by QtCreator 2023-10-18T06:08:25
#
#-------------------------------------------------

QT       += core gui
QT+=sql
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mycontroller.cpp \
    myrequestmapper.cpp \
    mydatabase.cpp \
    regcontroller.cpp \
    authcontroller.cpp \
    synccontroller.cpp \
    sendcontroller.cpp

HEADERS  += mainwindow.h \
    mycontroller.h \
    myrequestmapper.h \
    mydatabase.h \
    regcontroller.h \
    authcontroller.h \
    synccontroller.h \
    sendcontroller.h

FORMS    += mainwindow.ui

include(../MyServer/QtWebApp/httpserver/httpserver.pri)

CONFIG +=C++11
