#include <QSqlQuery>

#include "Model/player.h"

Player::Player(const QString name, const QDate birthday, const QString country)
{
    if (!birthday.isValid())
    {
        qWarning() << "cant load or create Player because the birthday is invalid";
        return;
    }
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
WHERE id = :id;
    )";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":id", _id);

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQuery);

    if (rawData.length() <= 0)
    {
        qWarning() << "no player with id: " << id << " found";
        return;
    }
    QList<QVariant> player = rawData[0];
    _name = player[1].toString();
    _birthday = player[2].toDate();
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
VALUES (:name, :birthday, :country);
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":name", _name);
    sqlQuery.bindValue(":birthday", _birthday);
    sqlQuery.bindValue(":country", _country);

    _db->sqlQuery(sqlQuery);
}


bool Player::_isPlayerUnknown()
{
    QString sqlPrepare = R"(
SELECT *
FROM player_list
WHERE name = :name
AND birthday = :birthday
AND country = :country
    )";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":name", _name);
    sqlQuery.bindValue(":birthday", _birthday);
    sqlQuery.bindValue(":country", _country);

    return _db->sqlQuery(sqlQuery).isEmpty();

}

int Player::_getPlayerIdFromDatabase()
{
    QString sqlPrepare = R"(
SELECT id
FROM player_list
WHERE name = :name
AND birthday = :birthday
AND country = :country
    )";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":name", _name);
    sqlQuery.bindValue(":birthday", _birthday);
    sqlQuery.bindValue(":country", _country);

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQuery);
    if (rawData.isEmpty())
    {
        qWarning() << "can't get the id from the player because the player does not exists.";
        return -1;
    }
    return rawData[0][0].toInt();
}

/*!
 * \brief Speichere ein bild zu einem Spieler
 *
 * \param[in] picture das bild, welches dem Spieler hinzugefügt werden soll
 *
 * Diese Methode lädt ein Bild in die Datenbank, und weist dieses Bild dann dem Spieler zu.
 */
void Player::savePicture(const QByteArray& picture)
{
    // step1: save picture in database
    QString sqlPrepare = R"(
INSERT INTO player_pictures_list (picture)
VALUES (:picture)
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":picture", picture);

    _db->sqlQuery(sqlQuery);
    sqlQuery.clear();

    // step2: get the id from the picture
    QString sqlQueryString = R"(
SELECT id
FROM player_pictures_list
ORDER BY id DESC
LIMIT 1;
)";
    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQueryString);
    if (rawData.isEmpty())
    {
        return;
    }

    int lastPictureId = rawData[0][0].toInt();

    // step3: set the picture id for the player
    sqlPrepare = R"(
UPDATE player_list
SET picture_id = :pictureId
WHERE id = :playerId
)";
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":pictureId", lastPictureId);
    sqlQuery.bindValue(":playerId", _id);

    _db->sqlQuery(sqlQuery);
}

/*!
 * \brief Lade ein bild zu einem Spieler
 *
 * \return das Bild im QByteArray Objekt
 *
 * Diese Methode lädt ein Bild aud der Datenbank
 */
QByteArray Player::loadPicture(void)
{
    QString sqlPrepare = R"(
SELECT picture
FROM player_list
         INNER JOIN player_pictures_list ppl ON player_list.picture_id = ppl.id
WHERE player_list.id = :playerId
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":playerId", _id);

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQuery);

    if (rawData.isEmpty())
    {
        // TODO: default Bild laden???
        return QByteArray();
    }

    QByteArray picture = rawData[0][0].toByteArray();

    return picture;
}

