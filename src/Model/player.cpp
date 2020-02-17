#include "Model/player.h"

Player::Player(const QString name, const QString birthday, const QString country)
{
    _db = &SqliteConnector::instance();
    _name = name;
    _birthday = birthday;
    _country = country;
    if (_isPlayerUnknown())
    {
        _addPlayerToDatabase();
    }
    _id = _getPlayerIdFromDatabase();
}

Player::Player(const int id)
{
    _db = &SqliteConnector::instance();
    _id = id;
    QString sqlPrepare = R"(
SELECT *
FROM player_list
WHERE id = ?;
    )";
    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(_id));
    QList<QVariant> player = _db->sqlQuery(sqlPrepare, sqlParameters)[0];
    _name = player[1].toString();
    _birthday = player[2].toString();
    _country = player[3].toString();
}

/// To add a Player to the SqLite Database.
/*
 * this method add a player to the SqLite Database. In the Database the id was increment up automatic
 * and the default value for the column is_available is 0.
 */
void Player::_addPlayerToDatabase()
{

    QString sqlPrepare = R"(
INSERT INTO player_list (name, birthday, country)
VALUES (?, ?, ?);
)";
    QList<QString> sqlParameters;
    sqlParameters.append(_name);
    sqlParameters.append(_birthday);
    sqlParameters.append(_country);
    _db->sqlQuery(sqlPrepare, sqlParameters);
}


bool Player::_isPlayerUnknown()
{
    QString sqlPrepare = R"(
SELECT *
FROM player_list
WHERE name = ?
AND birthday = ?
AND country = ?;
    )";
    QList<QString> sqlParameters;
    sqlParameters.append(_name);
    sqlParameters.append(_birthday);
    sqlParameters.append(_country);
    return _db->sqlQuery(sqlPrepare, sqlParameters).isEmpty();

}

int Player::_getPlayerIdFromDatabase()
{
    QString sqlPrepare = R"(
SELECT id
FROM player_list
WHERE name = ?
AND birthday = ?
AND country = ?;
    )";
    QList<QString> sqlParameters;
    sqlParameters.append(_name);
    sqlParameters.append(_birthday);
    sqlParameters.append(_country);

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlPrepare, sqlParameters);
    if (rawData.isEmpty())
    {
        qWarning() << "can't get the id from the player because the player does not exists.";
        return -1;
    }
    return rawData[0][0].toInt();
}
