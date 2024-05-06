#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

//Структура с информацией о сообщении
struct Message
{
    QString senderLogin;
    QString idRoom;
    QString content;
    int messageId;
};

#endif // MESSAGE_H
