#ifndef USER_H
#define USER_H

#include <QString>

//Структура инкапсулирующая информацию о чате
struct User
{
    QString id;
    QString login;
    QString password;
    QString displayName;
    QString accessToken;
};

#endif // USER_H
