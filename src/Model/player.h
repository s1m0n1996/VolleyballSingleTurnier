#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QDebug>
#include "Model/sqliteConnector.h"

class Player {
public:
    Player(const QString name,
           const QString birthday,
           const QString country);

    Player(const int id);

    QString getName(const int id);

    void addPlayerToDatabase();

    bool isPlayerUnknown();

private:
    SqliteConnector* _db;
    int _id;
    QString _name;
    QString _birthday;
    QString _country;

};

#endif // PLAYER_H
