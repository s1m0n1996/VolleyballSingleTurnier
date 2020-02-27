#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QDebug>
#include "Model/sqliteConnector.h"

class Player
{
public:
    Player();
    Player(const QString name,
           const QDate birthday,
           const QString country);

    Player(const int id);

    int getId() const { return _id; }
    QString getName() const { return _name; }
    QDate getBirthday() const { return _birthday; }
    QString getCountry() const { return _country; }
    QByteArray loadPicture(void);
    void savePicture(const QByteArray& picture);

private:
    SqliteConnector* _db;
    int _id;
    QString _name;
    QDate _birthday;
    QString _country;

    int _getPlayerIdFromDatabase();
    void _addPlayerToDatabase();
    bool _isPlayerUnknown();
};

#endif // PLAYER_H
