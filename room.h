#ifndef ROOM_H
#define ROOM_H

#include <QString>

//Структура инкапсулирующая информацию о чате
struct Room
{
    QString name;
    int id;
    int isActive=0;
};

#endif // ROOM_H
