#include <QSqlQuery>

#include "Model/player.h"

/*!
 * \brief Es wird ein Spieler erstellt
 *
 * \param[in] name Spielername
 * \param[in] birthday Geburtstag
 * \param[in] country Land
 *
 * Es wird ein Spielerobjekt erstellt.
 * Beim erstellen des Objektes wird der Spieler automatisch in die Datenbank eingetragen, sofern dieser nicht schon
 * vorhanden ist.
 *
 * Wenn der Spieler bereits vorhanden ist wird die Membervariable _id aus der Datenbank geladen.
 *
 */
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

/*!
 * \brief Es wird ein Spieler Geladen
 *
 * \param[in] id Spieler Id
 *
 * Es wird ein Spielerobjekt erstellt.
 * Der Die Spieler Id muss bereits in der Datenbank eingetragen sein.
 * Es werden alle zugehörigen Daten aus der Datenbank in die Membervariablen geschrieben dazu gehören:
 * - name
 * - geburtstag
 * - Land
 */
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
        qWarning() << "No player with id: " << id << " found";
        return;
    }
    QList<QVariant> player = rawData[0];
    _name = player[1].toString();
    _birthday = player[2].toDate();
    _country = player[3].toString();
}

/*!
 * \brief Der Spieler wird in der Datenbank gespeichert.
 *
 * Es muss vorher beispielsweise mit der Methode _isPlayerUnknown kontrolliert werden,
 * ob es den Spieler bereits gibt, da die Methode nur ausgeführt werden darf, wenn der Spieler noch nicht
 * in der Datenbank vorhanden ist.
 */
void Player::_addPlayerToDatabase(void)
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

/*!
 * \brief Kontrolliere ob der Spieler bereits in der Datenbank vorhanden ist.
 *
 * \return true wenn es den Spieler noch nicht gibt und false wenn der Spieler bereits vorhanden ist
 *
 */
bool Player::_isPlayerUnknown(void)
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

/*!
 * \brief Gebe die id des Spielers zurück
 *
 * Anhand der einzigartigen Daten Name, Geburtstag und Land wird die zugehörige is des Spielers aus der
 * Datenbank geladen.
 * Bei Aufruf der Methode muss der Spieler bereits in der Datenbank vorhanden sein.
 */
int Player::_getPlayerIdFromDatabase(void)
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
        qWarning() << "Can't get the id from the player because the player does not exists.";
        return -1;
    }
    return rawData[0][0].toInt();
}

/*!
 * \brief Speichere ein Bild zu einem Spieler
 *
 * \param[in] picture das Bild, welches dem Spieler hinzugefügt werden soll
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
    sqlQuery.prepare(sqlQueryString);
    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQuery);
    if (rawData.isEmpty())
    {
        return;
    }

    int lastPictureId = rawData[0][0].toInt();

    sqlQuery.clear();

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
 * \brief Lade ein Bild zu einem Spieler
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
        return QByteArray();
    }

    QByteArray picture = rawData[0][0].toByteArray();

    return picture;
}
