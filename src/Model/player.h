#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QDebug>
#include "Model/sqliteConnector.h"

class Player {
public:
    Player(const QString playerfirstname,
           const QString playername,
           const QString country,
           const QString birthday);

    Player(const int id);

    QString getName(const int id);

    void addPlayerToDatabase();

    bool isPlayerUnknown();

private:
    SqliteConnector* _db;
    QString _firstName;
    QString _lastName;
    QString _country;
    QString _birthday;
};

#endif // PLAYER_H
