#include "Model/player.h"

Player::Player(const QString name, const QString birthday, const QString country) {
    _db = &SqliteConnector::instance();
    _name = name;
    _birthday = birthday;
    _country = country;
    qDebug() << name;
    if (isPlayerUnknown()) {
        addPlayerToDatabase();
    }
}

Player::Player(const int id){
    _db = &SqliteConnector::instance();
    QString sqlPrepare = R"(
SELECT *
FROM player_list
WHERE id = ?;
    )";
    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(id));
    QList<QVariant> player = _db->sqlQuery(sqlPrepare, sqlParameters)[0];
    _name = player[1].toString();
    _birthday= player[2].toString();
    _country = player[3].toString();
}

/// To add a Player to the SqLite Database.
/*
 * this method add a player to the SqLite Database. In the Database the id was increment up automatic
 * and the default value for the column is_available is 0.
 */
void Player::addPlayerToDatabase() {

    QString sqlPrepare = R"(
INSERT INTO player_list (name, birthday, country)
VALUES (?, ?, ?
)";
    QList<QString> sqlParameters;
    sqlParameters.append(_name);
    sqlParameters.append(_birthday);
    sqlParameters.append(_country);

    _db->sqlQuery(sqlPrepare, sqlParameters);
}


bool Player::isPlayerUnknown() {
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
