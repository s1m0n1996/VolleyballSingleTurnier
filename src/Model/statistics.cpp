#include "Model/statistics.h"

Statistics::Statistics()
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();
}

/*!
 * \brief Gibt den Average von einem Spieler zurück
 * \param[in] einen Player
 * \return double
 *
 * Gibt den Average eines Spielers für das aktuelle Spiel aus
 */
double Statistics::getAverageOfPlayerInCurrentGame(const Player& player)
{
    Game game;
    QString sqlPrepare = R"(
SELECT AVG(value * value_type_id) AS average
FROM leg_history_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
  AND game_board_id = :gameId
  AND player_id = :playerId
    )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":gameId", game.getGameId());
    sqlQuery.bindValue(":playerId", player.getId());

    return _db->sqlQuery(sqlQuery)[0][0].toDouble();
}


/*!
 * \brief Gibt den Average von einem Spieler zurück
 * \param[in] einen Player
 * \return double
 *
 * Gibt den Average eines Spielers aus
 */
double Statistics::getAverageOfPlayerEver(const Player& player)
{
    QString sqlPrepare = R"(
SELECT AVG(value * value_type_id) AS average
FROM leg_history_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND player_id = :playerId
    )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":playerId", player.getId());

    return _db->sqlQuery(sqlQuery)[0][0].toDouble();
}

/*!
 * \brief Gibt den Average von einem Spieler zurück
 * \param[in] einen Player
 * \param[in] LegID
 * \return double
 *
 * Gibt den Average eines Spielers aus dem als Parameter übergebenen Leg aus
 */
double Statistics::getAverageOfPlayerInCurrentLeg(const Player& player, const int legId)
{
    Game game;
    QString sqlPrepare = R"(
SELECT AVG(value * value_type_id) AS average
FROM leg_history_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
  AND game_board_id = :gameId
  AND leg_id = :legId
  AND player_id = :playerId
    )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":gameId", game.getGameId());
    sqlQuery.bindValue(":legId", legId);
    sqlQuery.bindValue(":playerId", player.getId());

    return _db->sqlQuery(sqlQuery)[0][0].toDouble();
}

/*!
 * \brief Gibt die Anzahl von gewonnenen Spielen eines Spielers zurück
 * \param[in] einen Player
 * \return int
 *
 */
int Statistics::getWonGamesOfPlayer(const Player& player)
{
    QString sqlPrepare = R"(
                         SELECT *
                         FROM game_board_list
                         WHERE winner_id = :winnerId;
    )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":winnerId", player.getId());
    QList<QList<QVariant>> list = _db->sqlQuery(sqlQuery);
    return list.size();
}

/*!
 * \brief Gibt die Anzahl von 180er Würfen eines Spielers in einem eingegebenen Spiel zurück
 * \param[in] einen Player
 * \param[in] die GameID des gefragten Spiels
 * \return int
 *
 */
int Statistics::getCountOfHundretEightyInGame(const Player& player, const int gameId)
{
    QString sqlPrepare = R"(
                         SELECT *
                         FROM leg_history_list
                         WHERE sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                           AND game_board_id = :gameId
                           AND player_id = :playerId
                           AND value_type_id = :valueTypeId
                           AND value = :value
    )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":gameId", gameId);
    sqlQuery.bindValue(":playerId", player.getId());
    sqlQuery.bindValue(":valueTypeId", 3);
    sqlQuery.bindValue(":value", 20);
    QList<QList<QVariant>> list = _db->sqlQuery(sqlQuery);
    return list.size();
}

/*!
 * \brief Gibt die Anzahl von 180er Würfen eines Spielers aus dem aktuellen Turniers zurück
 * \param[in] einen Player
 * \return int
 *
 */
int Statistics::getCountOfHundretEightyInTournament(const Player& player)
{
    QString sqlPrepare = R"(
                         SELECT *
                         FROM leg_history_list
                         WHERE sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                           AND player_id = :playerId
                           AND value_type_id = :valueTypeId
                           AND value = :value
    )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":playerId", player.getId());
    sqlQuery.bindValue(":valueTypeId", 3);
    sqlQuery.bindValue(":value", 20);
    QList<QList<QVariant>> list = _db->sqlQuery(sqlQuery);
    return list.size();
}
