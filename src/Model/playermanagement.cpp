
#include "Model/playermanagement.h"


PlayerManagement::PlayerManagement()
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();
    refreshDatabasePlayerTable();
    refreshNextGamePlayerTable();
    refreshDeletedPlayerTable();
}

/*!
 * \brief get all players
 *
 * \param[in]onlyAvailable default: true. get only players who is the available flag true.
 *
 * \return a list of player objects
 *
 * This method returned all players that are stored in the database.
 * For Example the player_list table from the database have this entries:
 *
 * id   name    birthday    country     is_available
 * 1	Klaus	2010-12-15	DE	        1
 * 2	Hilde	2000-12-15	DE	        1
 * 3	Peter	2020-12-15	DE	        1
 *
 * you get a list of 3 Player objects
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
 * \brief add a player to the actual turnier gameboard
 *
 * \param[in] addPlayer the player that would be added to the actual turnier gameboard
 */
void PlayerManagement::addPlayerForNewGame(const Player addPlayer)
{
    QString sqlPrepare = R"(
INSERT INTO tournament_players_list (player_id, sport_type_id, game_mode_id, tournament_id)
VALUES ((SELECT id FROM player_list WHERE name = :name AND birthday = :birthday AND country = :country), :sportTypeId, :gameModeId, :tournamentId)
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":name", addPlayer.getName());
    sqlQuery.bindValue(":birthday", addPlayer.getBirthday());
    sqlQuery.bindValue(":country", addPlayer.getCountry());
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    _db->sqlQuery(sqlQuery);
    refreshNextGamePlayerTable();

    emit valueChanged();
}

/*!
 * \brief add a player to the actual turnier gameboard
 *
 * \param[in] addPlayer the player that would be added to the actual turnier gameboard
 */
void PlayerManagement::addPlayerForNewGame(const QList<Player> addPlayer)
{
    for (Player player : addPlayer)
    {
        addPlayerForNewGame(player);
    }
}

/*!
 * \brief drop a player to the actual turnier gameboard
 *
 * \param[in] dropPlayer the player from the list that would be added to the actual turnier gameboard
 *
 * This method is the undo function for addPlayerForNewGame
 */
void PlayerManagement::dropPlayerForNewGame(const Player dropPlayer)
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
    sqlQuery.bindValue(":name", dropPlayer.getName());
    sqlQuery.bindValue(":birthday", dropPlayer.getBirthday());
    sqlQuery.bindValue(":country", dropPlayer.getCountry());
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    _db->sqlQuery(sqlQuery);
    refreshNextGamePlayerTable();
    refreshDeletedPlayerTable();
    emit valueChanged();
}

/*!
 * \brief count the players that are selected for the new game
 *
 * \return number of players
 */
int PlayerManagement::countSelectedPlayersForNewGame()
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
 * \brief count the players they are missing to start the game
 *
 * \return missing players of the list
 *
 * Count the players they are missing to the next 2^n compatible.
 *
 * Examples:
 * if you have 3 players the next 2^n compatible is 4 and the method returned 1.
 * if you have 4 players it is 2^n compatible and the method returned 0.
 * if you have 5 players the next 2^n compatible is 8 and the method returned 3.
 */
int PlayerManagement::countMissingPlayersForNewGame()
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
 * \brief create or refresh the player table from the database
 *
 * this method refresh the model data from the table. After a call the same data from the database are in the model.
 */
void PlayerManagement::refreshDatabasePlayerTable()
{
    _databasePlayerTable->setQuery("SELECT name, birthday, country FROM player_list where is_available >= 1");

    _databasePlayerTable->setHeaderData(0, Qt::Horizontal, tr("Name"));
    _databasePlayerTable->setHeaderData(1, Qt::Horizontal, tr("Geburtstag"));
    _databasePlayerTable->setHeaderData(2, Qt::Horizontal, tr("Land"));
}

/*!
 * \brief create or refresh the player tablefor the next game from the database
 *
 * this method refresh the model data from the table. After a call the same data from the database are in the model.
 */
void PlayerManagement::refreshNextGamePlayerTable()
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
    // TODO: wieder prepare system verwenden
    //qDebug() << "isValid: " << sqlQuery.isValid();
    //_nextGamePlayerTableModel->setQuery(sqlQuery);

    QString sqlQueryString;
    sqlQueryString = R"(
SELECT pl.name, pl.birthday, pl.country
FROM tournament_players_list
         INNER JOIN player_list pl ON tournament_players_list.player_id = pl.id
WHERE sport_type_id = )";
    sqlQueryString += QString::number(_gameManagement->getSportTypeId());
    sqlQueryString += " AND game_mode_id = ";
    sqlQueryString += QString::number(_gameManagement->getGameModeId());
    sqlQueryString += " AND tournament_id = ";
    sqlQueryString += QString::number(_gameManagement->getTournamentId());

    _nextGamePlayerTableModel->setQuery(sqlQueryString);

    _nextGamePlayerTableModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    _nextGamePlayerTableModel->setHeaderData(1, Qt::Horizontal, tr("Geburtstag"));
    _nextGamePlayerTableModel->setHeaderData(2, Qt::Horizontal, tr("Land"));
}

void PlayerManagement::refreshDeletedPlayerTable()
{
    _deletedPlayerTableModel->setQuery("SELECT name, birthday, country FROM player_list where is_available = 0");

    _deletedPlayerTableModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    _deletedPlayerTableModel->setHeaderData(1, Qt::Horizontal, tr("Geburtstag"));
    _deletedPlayerTableModel->setHeaderData(2, Qt::Horizontal, tr("Land"));
}

/*!
 * \brief drop a player from the database
 *
 * \param[in] player object that would be dropped
 *
 * This method drop a player from the database virtual.
 * So the Player would not delete from the database really. Only the is_available flag was set to false.
 * The reason that the player can not delete really from the database is that the foreign keys needs the players.
 */
void PlayerManagement::dropPlayerFromDatabase(Player dropPlayer)
{

    QString sqlPrepare = R"(
UPDATE player_list
SET is_available = 0
WHERE id = :id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":id", dropPlayer.getId());

    _db->sqlQuery(sqlQuery);

    refreshDatabasePlayerTable();
    refreshDeletedPlayerTable();
}

void PlayerManagement::restorePlayerFromDatabase(const Player restoredPlayer)
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


QList<Player> PlayerManagement::getPlayersForNextGame()
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
