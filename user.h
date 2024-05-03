#ifndef USER_H
#define USER_H

#include <QString>

struct User
{
    QString id;
    QString login;
    QString password;
    QString displayName;
    QString accessToken;
};

#endif // USER_H
