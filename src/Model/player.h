#ifndef PLAYER_H
#define PLAYER_H
#include "qstring.h"


class player
{
public:
    player(const QString playerfirstname,
               const QString playername,
               const QString country,
               const QString birthday);
    player(const int id);

    QString getName(const int id);

    void addPlayerToDatabank();
    bool isPlayerUnknown();

private:
    int _playerid;
    QString _playerfirstname;
    QString _playername;
    QString _country;
    QString _birthday;
};

#endif // PLAYER_H
