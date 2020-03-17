
#include "Model/playerManagement.h"


PlayerManagement::PlayerManagement(void)
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();
    refreshDatabasePlayerTable();
    refreshNextGamePlayerTable();
    refreshDeletedPlayerTable();
}

/*!
 * \brief Gebe alle Spieler zurück
 *
 * \param[in]onlyAvailable default: true. gebe nur verfügbare Spieler zurück
 *
 * \return Liste von Spieler objekten
 *
 * Es werden alle Spieler verfügbaren aus der Datenbank geladen
 * \example die player_list Tabelle hat folgende Einträge:
 *
 * id   name    birthday    country     is_available
 * 1	Klaus	2010-12-15	DE	        1
 * 2	Hilde	2000-12-15	DE	        1
 * 3	Peter	2020-12-15	DE	        1
 *
 * es wird eine Liste von 3 Spielern zurückgegeben
 */
QList<Player> PlayerManagement::getAllStoredPlayers(bool onlyAvailable)
{
    QString sqlPrepare = R"(
SELECT id
FROM player_list
WHERE is_available >= :isAvailable
ORDER BY name;
    )";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":isAvailable", onlyAvailable);

    QList<QList<QVariant>> playerIds = _db->sqlQuery(sqlQuery);

    QList<Player> storedPlayers;

    for (const QVariant playerId : playerIds)
    {
        storedPlayers.append(playerId.toList()[0].toInt());
    }

    return storedPlayers;
}

/*!
 * \brief Füge einen Spieler dem aktuellen Turnier hinzu
 *
 * \param[in] addPlayer Der Spieler der hinzugefügt werden soll.
 *
 * Es können maximal 64 Spieler hinzugefügt werden. Danach ist die Methode nicht mehr aufrufbar.
 */
void PlayerManagement::addPlayerForNewGame(const Player& addPlayer)
{
    if (countSelectedPlayersForNewGame() > 64)
    {
        return;
    }

    // check if the player exists in the new tournament
    QString sqlPrepare = R"(
SELECT count(*)
FROM tournament_players_list
WHERE player_id = :playerId
  AND sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":playerId", addPlayer.getId());
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    if (0 < _db->sqlQuery(sqlQuery)[0][0].toInt())
    {
        return;
    }


    sqlPrepare = R"(
INSERT INTO tournament_players_list (player_id, sport_type_id, game_mode_id, tournament_id)
VALUES (:playerId, :sportTypeId, :gameModeId, :tournamentId)
)";

    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":playerId", addPlayer.getId());
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    _db->sqlQuery(sqlQuery);
    refreshNextGamePlayerTable();

    emit valueChanged();
}

/*!
 * \brief Lösche einen Spieler vom neuen Spiel
 *
 * \param[in] deletePlayer the player from the list that would be added to the actual turnier gameboard
 *
 * This method is the undo function for addPlayerForNewGame
 */
void PlayerManagement::deletePlayerForNewGame(const Player& deletePlayer)
{
    QString sqlPrepare = R"(
DELETE
FROM tournament_players_list
WHERE player_id =
      (SELECT id FROM player_list WHERE name = :name AND birthday = :birthday AND country = :country)
  AND sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":name", deletePlayer.getName());
    sqlQuery.bindValue(":birthday", deletePlayer.getBirthday());
    sqlQuery.bindValue(":country", deletePlayer.getCountry());
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    _db->sqlQuery(sqlQuery);
    refreshNextGamePlayerTable();
    refreshDeletedPlayerTable();
    emit valueChanged();
}

/*!
 * \brief Zähle alle spieler im aktuellen/Neuen Spiel
 *
 * \return anzahl der Spieler
 */
int PlayerManagement::countSelectedPlayersForNewGame(void)
{
    QString sqlPrepare = R"(
SELECT Count(*)
FROM tournament_players_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    QList<QList<QVariant>> data = _db->sqlQuery(sqlQuery);

    if (data.length() <= 0)
    {
        return 0;
    }

    return data[0][0].toInt();
}

/*!
 * \brief Zähle Spieler die noch benötigt werden, um das Spiel zu beginnen
 *
 * \return anzahl fehlemnder Spieler
 *
 * Zähle die Spieler die noch benötigt werden, um auf eine 2^n kompatibilität zu kommen.
 *
 * \example:
 * bei 3 Spielern im aktuellen Spiel ist die nächste 2^n kompatibilität 4 also wird 1 fehlender Spieler zurückgegeben.
 * bei 4 Spielern im aktuellen Spiel wird eine 0 zurückgegeben da es 2^n kompatibilität ist.
 * bei 5 Spielern im aktuellen Spiel ist die nächste 2^n kompatibilität 8 also werden 3 fehlender Spieler zurückgegeben.
 */
int PlayerManagement::countMissingPlayersForNewGame(void)
{
    int nActualPlayers = countSelectedPlayersForNewGame();

    // the calculation not works with 0 because the log(<1) is negative.
    if (0 == nActualPlayers)
    {
        return 2;
    }
    // the calculation not works with 1 because the log(1) = 0
    if (1 == nActualPlayers)
    {
        return 1;
    }

    double actualExponent = log(nActualPlayers) / log(2);
    if ((actualExponent - static_cast<int>(actualExponent)) == 0)
    {
        // the exponent is direct number, so the list is complete
        return 0;
    }
    int nextExponent = static_cast<int>(actualExponent) + 1;
    return static_cast<int>(pow(2, nextExponent)) - nActualPlayers;

}

/*!
 * \brief Aktualisiere Tabelle mit allen Spielern
 *
 * Es wird die Tabelle im Model aktualisiert, welche alle Spieler darstellt.
 */
void PlayerManagement::refreshDatabasePlayerTable(void)
{
    _databasePlayerTable->setQuery("SELECT name, birthday, country FROM player_list where is_available >= 1");

    _databasePlayerTable->setHeaderData(0, Qt::Horizontal, tr("Name"));
    _databasePlayerTable->setHeaderData(1, Qt::Horizontal, tr("Geburtstag"));
    _databasePlayerTable->setHeaderData(2, Qt::Horizontal, tr("Land"));
}

/*!
 * \brief Aktualisiere Tabelle mit Spielern für das nächste Spiel
 *
 * Es wird die Tabelle im Model aktualisiert, welche alle Spieler des neuen Spiels darstellt
 */
void PlayerManagement::refreshNextGamePlayerTable(void)
{
    QSqlQuery sqlQuery;

    QString sqlPrepare = R"(
SELECT pl.name, pl.birthday, pl.country
FROM tournament_players_list
         INNER JOIN player_list pl ON tournament_players_list.player_id = pl.id
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
)";

    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    sqlQuery.exec();
    _nextGamePlayerTableModel->setQuery(sqlQuery);

    _nextGamePlayerTableModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    _nextGamePlayerTableModel->setHeaderData(1, Qt::Horizontal, tr("Geburtstag"));
    _nextGamePlayerTableModel->setHeaderData(2, Qt::Horizontal, tr("Land"));
}

/*!
 * \brief Aktualisiere Tabelle mit gelöschten Spielern
 *
 * Es wird die Tabelle im Model aktualisiert, welche alle gelöschten Spieler darstellt.
 */
void PlayerManagement::refreshDeletedPlayerTable(void)
{
    _deletedPlayerTableModel->setQuery("SELECT name, birthday, country FROM player_list where is_available = 0");

    _deletedPlayerTableModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    _deletedPlayerTableModel->setHeaderData(1, Qt::Horizontal, tr("Geburtstag"));
    _deletedPlayerTableModel->setHeaderData(2, Qt::Horizontal, tr("Land"));
}

/*!
 * \brief Lösche einen Spieler aus der Datenbank
 *
 * \param[in] deletePlayer Spieler objekt, welches gelöscht werden soll.
 *
 * Der Spieler wird nicht wirklich aus der Datenbank gelöscht.
 * Es wird nur das is_available flag auf false bzw. null gesetzt, dass der spieler nichtmehr geladen wird.
 */
void PlayerManagement::deletePlayerFromDatabase(Player& deletePlayer)
{

    QString sqlPrepare = R"(
UPDATE player_list
SET is_available = 0
WHERE id = :id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":id", deletePlayer.getId());

    _db->sqlQuery(sqlQuery);

    refreshDatabasePlayerTable();
    refreshDeletedPlayerTable();
}

/*!
 * \brief Stelle gelöschten Spieler wieder her
 *
 * \param[in] restoredPlayer Spieler objekt, welches gelöscht werden soll.
 *
 * Der Spieler wird nicht wirklich aus der Datenbank gelöscht.
 * Es wird nur das is_available flag auf false bzw. null gesetzt, dass der spieler nichtmehr geladen wird.
 */
void PlayerManagement::restorePlayerFromDatabase(const Player& restoredPlayer)
{
    QString sqlPrepare = R"(
UPDATE player_list
SET is_available = 1
WHERE id = :id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":id", restoredPlayer.getId());

    _db->sqlQuery(sqlQuery);

    refreshDatabasePlayerTable();
    refreshDeletedPlayerTable();
}

/*!
 * \brief Gebe alle Spieler des aktuellen Turnier zurück
 *
 * \return Liste von Spieler objekten
 *
 * Es werden alle Spieler im aktuellen Turnier zurückgegeben
 */
QList<Player> PlayerManagement::getPlayersForNextGame(void)
{

    QString sqlPrepare = R"(
SELECT player_id
FROM tournament_players_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    QList<QList<QVariant>> playersIdsFromDatabase = _db->sqlQuery(sqlQuery);

    QList<Player> players;
    for (QList<QVariant>& playerId : playersIdsFromDatabase)
    {
        players.append(Player(playerId[0].toInt()));
    }

    return players;
}
