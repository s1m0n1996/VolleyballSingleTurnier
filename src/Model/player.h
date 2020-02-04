#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QDebug>
#include "Model/sqliteConnector.h"

class Player
{
public:
    Player(const QString name,
           const QString birthday,
           const QString country);

    Player(const int id);

    const int getId() const { return _id; };
    const QString getName() const { return _name; };
    const QString getBirthday() const { return _birthday; };
    const QString getCountry() const { return _country; };

private:
    SqliteConnector* _db;
    int _id;
    QString _name;
    QString _birthday;
    QString _country;

    const int _getPlayerIdFromDatabase();
    const void _addPlayerToDatabase();
    const bool _isPlayerUnknown();
};

#endif // PLAYER_H
