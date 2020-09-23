#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "Model/sqliteConnector.h"

class GameManagement;

/*!
 * \file player.h
 * \author Simon Flörke
 * \brief Spieler Klasse, welche einen Spieler repräsentiert
 *
 * Es werden verschiedene Methoden für einen Spieler bereitgestellt.
 *
*/
class Player
{
public:
    Player(const QString name,
           const QDate birthday,
           const QString country,
           const QString gender);

    Player(const int id);

    int getId(void) const { return _id; }
    QString getName(void) const { return _name; }
    QDate getBirthday(void) const { return _birthday; }
    QString getCountry(void) const { return _country; }
    QString getGender(void) const { return _gender; }
    const int getPlayedGames() const;
    QByteArray loadPicture(void);
    void savePicture(const QByteArray& picture);

private:
    SqliteConnector* _db;
    GameManagement* _gameManagement;
    int _id;
    QString _name;
    QDate _birthday;
    QString _country;
    QString _gender;
    int _playedGames;

    int _getPlayerIdFromDatabase(void);
    void _addPlayerToDatabase(void);
    bool _isPlayerUnknown(void);
};

#endif // PLAYER_H
