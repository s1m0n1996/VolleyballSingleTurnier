#include "Model/player.h"

Player::Player(const QString firstName, const QString lastName, const QString country, const QString birthday) {
    _db = &SqliteConnector::instance();
    _firstName = firstName;
    _lastName = lastName;
    _country = country;
    _birthday = birthday;
    if (isPlayerUnknown()) {
        addPlayerToDatabase();
    }
}

Player::Player(const int id){
    _db = &SqliteConnector::instance();
}

/// To add a Player to the SqLite Database.
/*
 * this method add a player to the SqLite Database. In the Database the id was increment up automatic
 * and the default value for the column is_available is 0.
 */
void Player::addPlayerToDatabase() {

    QString sqlPrepare = "INSERT INTO player_list (name, birthday, country) VALUES (?, ?, ?)";
    QList<QString> sqlParameters;
    sqlParameters.append(_firstName + ", " + _lastName); // TODO in 2 spalten aufteilen
    sqlParameters.append(_birthday);
    sqlParameters.append(_country);

    _db->sqlQuery(sqlPrepare, sqlParameters);
}


bool Player::isPlayerUnknown() {
    return true;
}
