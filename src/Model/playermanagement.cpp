
#include "Model/playermanagement.h"


PlayerManagement::PlayerManagement()
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();
    refreshDatabasePlayerTable();
    refreshNextGamePlayerTable();
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
WHERE is_available >= ?
ORDER BY name;
    )";

    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(onlyAvailable));

    QList<QList<QVariant>> playerIds = _db->sqlQuery(sqlPrepare, sqlParameters);

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
VALUES ((SELECT id FROM player_list WHERE name = ? AND birthday = ? AND country = ?), ?, ?, ?)
)";

    QList<QVariant> sqlParameters;
    sqlParameters.append(addPlayer.getName());
    sqlParameters.append(addPlayer.getBirthday());
    sqlParameters.append(addPlayer.getCountry());
    sqlParameters.append(_gameManagement->getSportTypeId());
    sqlParameters.append(_gameManagement->getGameModeId());
    sqlParameters.append(_gameManagement->getTournamentId());

    qDebug() << _gameManagement->getSportTypeId() << _gameManagement->getGameModeId() << _gameManagement->getTournamentId();

    _db->sqlQuery(sqlPrepare, sqlParameters);
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
      (SELECT id FROM player_list WHERE name = ? AND birthday = ? AND country = ?)
  AND sport_type_id = ?
  AND game_mode_id = ?
  AND tournament_id = ?
)";

    QList<QVariant> sqlParameters;
    sqlParameters.append(dropPlayer.getName());
    sqlParameters.append(dropPlayer.getBirthday());
    sqlParameters.append(dropPlayer.getCountry());
    sqlParameters.append(_gameManagement->getSportTypeId());
    sqlParameters.append(_gameManagement->getGameModeId());
    sqlParameters.append(_gameManagement->getTournamentId());

    _db->sqlQuery(sqlPrepare, sqlParameters);
    refreshNextGamePlayerTable();
    emit valueChanged();
}

/*!
 * \brief count the players that are selected for the new game
 *
 * \return number of players
 */
int PlayerManagement::countSelectedPlayersForNewGame()
{
    QString sqlQuery = R"(
SELECT Count(*)
FROM tournament_players_list
WHERE sport_type_id = ?
  AND game_mode_id = ?
  AND tournament_id = ?;)";

    QList<QVariant> sqlParameters;
    sqlParameters.append(_gameManagement->getSportTypeId());
    sqlParameters.append(_gameManagement->getGameModeId());
    sqlParameters.append(_gameManagement->getTournamentId());

    QList<QList<QVariant>> data = _db->sqlQuery(sqlQuery, sqlParameters);

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
}

/*!
 * \brief create or refresh the player tablefor the next game from the database
 *
 * this method refresh the model data from the table. After a call the same data from the database are in the model.
 */
void PlayerManagement::refreshNextGamePlayerTable()
{
    QSqlQuery query;

    QString sqlPrepare = R"(
SELECT pl.name, pl.birthday, pl.country
FROM tournament_players_list
         INNER JOIN player_list pl ON tournament_players_list.player_id = pl.id
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
)";

    query.prepare(sqlPrepare);

    query.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    query.bindValue(":gameModeId", _gameManagement->getGameModeId());
    query.bindValue(":tournamentId", _gameManagement->getTournamentId());
    // TODO: wieder prepare system verwenden
    //qDebug() << "isValid: " << query.isValid();
    //_nextGamePlayerTableModel->setQuery(query);

    QString sqlQuery;
    sqlQuery = R"(
SELECT pl.name, pl.birthday, pl.country
FROM tournament_players_list
         INNER JOIN player_list pl ON tournament_players_list.player_id = pl.id
WHERE sport_type_id = )";
    sqlQuery += QString::number(_gameManagement->getSportTypeId());
    sqlQuery += " AND game_mode_id = ";
    sqlQuery += QString::number(_gameManagement->getGameModeId());
    sqlQuery += " AND tournament_id = ";
    sqlQuery += QString::number(_gameManagement->getTournamentId());


    _nextGamePlayerTableModel->setQuery(sqlQuery);


}
