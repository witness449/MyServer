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
    sendcontroller.cpp \
    createroomcontroller.cpp \
    global.cpp \
    clientstate.cpp \
    unbancontroller.cpp \
    leavecontroller.cpp \
    bancontroller.cpp


HEADERS  += mainwindow.h \
    mycontroller.h \
    myrequestmapper.h \
    mydatabase.h \
    regcontroller.h \
    authcontroller.h \
    synccontroller.h \
    sendcontroller.h \
    createroomcontroller.h \
    global.h \
    clientstate.h \
    user.h \
    message.h \
    room.h \
    unbancontroller.h \
    leavecontroller.h \
    bancontroller.h



FORMS    += mainwindow.ui

include(../MyServer/QtWebApp/httpserver/httpserver.pri)

CONFIG +=C++11

win32:CONFIG(release, debug|release): LIBS +=-LD:/tools/OpenSSL/vc-win32/lib/ -llibcrypto

INCLUDEPATH += D:/tools/OpenSSL/vc-win32/include
DEPENDPATH += D:/tools/OpenSSL/vc-win32/include

win32:CONFIG(release, debug|release): LIBS +=-LD:/tools/OpenSSL/vc-win32/lib/ -llibssl

INCLUDEPATH += D:/tools/OpenSSL/vc-win32/include
DEPENDPATH += D:/tools/OpenSSL/vc-win32/include
