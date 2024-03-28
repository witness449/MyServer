#ifndef USER_H
#define USER_H

#include <QString>

struct User
{
    QString Login;
    QString Password;
    QString DisplayName;
    QString AccessToken;
};

#endif // USER_H
