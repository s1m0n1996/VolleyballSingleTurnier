#include "playerstatistics.h"

PlayerStatistics::PlayerStatistics()
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();
}

/*!
 * \brief Gibt alle Spieler mit der relativen anzahl gewonnener Spiele zurück
 *
 * \return als key steht der Spielername und als value die relative anzahl der gewinnenen Spiele in einer QMap
 *
 * Es wird ein Prozektwert zurückgegeben (0-1), der sich berechnet aus:
 * anzahl der Gewonnenen Spieler / Anzahl gespielete Spiele
 */
QMap<QString, double> PlayerStatistics::getWinningStatistic(void)
{
    QString sqlPrepare = R"(
SELECT name, sum(winning_games) AS winning_games, sum(played_games) AS played_games
FROM (
         SELECT player_id, 0 AS winning_games, sum(count) AS played_games
         FROM (

-- count the played games for player_a
                  SELECT player_a_id AS player_id, count(*) AS count
                  FROM game_board_list
                  WHERE sport_type_id = :sportTypeId
                    AND game_mode_id = :gameModeId
                    AND player_a_id NOT NULL
                    AND player_b_id NOT NULL
                    AND winner_id NOT NULL
                  GROUP BY player_a_id

                  UNION

-- count the played games for player_b
                  SELECT player_b_id AS player_id, count(*) AS count
                  FROM game_board_list
                  WHERE sport_type_id = :sportTypeId
                    AND game_mode_id = :gameModeId
                    AND player_a_id NOT NULL
                    AND player_b_id NOT NULL
                    AND winner_id NOT NULL
                  GROUP BY player_b_id
              )
         GROUP BY player_id
         UNION

-- count the winning games
         SELECT winner_id AS player_id, count(*) AS winning_games, 0 AS played_games
         FROM game_board_list
         WHERE sport_type_id = :sportTypeId
           AND game_mode_id = :gameModeId
           AND player_a_id NOT NULL
           AND player_b_id NOT NULL
           AND winner_id NOT NULL
         GROUP BY winner_id
     )
         INNER JOIN player_list pl ON player_id = pl.id
GROUP BY player_id
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());

    QList<QList<QVariant>> playerWinLoseGames = _db->sqlQuery(sqlQuery);

    QMap<QString, double> winningGamesStatistic;

    for (QList<QVariant>& player : playerWinLoseGames)
    {
        winningGamesStatistic[player[0].toString()] = player[1].toDouble() / player[2].toDouble();
    }

    return winningGamesStatistic;
}

/*!
 * \brief Gibt gewonnene und verlorene Spiele eines Spielers zurück
 *
 * \param[in] player spieler, von dem die gewonnenen und verlorenen Spiele zurückgegeben werden sollen.
 *
 * \return als key stehen die Möglichkeiten ("Gewonnen", "Verloren") und als value die Anzahl der Spiele in einer QMap
 *
 * Es werden die Gewonnenen und die Verlorenen Spiele gezählt
 */
QMap<QString, double> PlayerStatistics::getWinningStatistic(const Player* player)
{
    QString sqlPrepare = R"(
SELECT sum(win) AS win, sum(lost) AS lost
FROM (
-- select win games
      SELECT count(*) AS win, 0 AS lost
      FROM game_board_list
      WHERE sport_type_id = :sportTypeId
        AND game_mode_id = :gameModeId
        AND player_a_id NOT NULL
        AND player_b_id NOT NULL
        AND winner_id = :playerId

      UNION

-- select lost games
      SELECT 0 AS win, count(*) AS lost
      FROM game_board_list
      WHERE sport_type_id = :sportTypeId
        AND game_mode_id = :gameModeId
        AND (player_a_id = :playerId OR player_b_id = :playerId)
        AND winner_id IS NOT :playerId
    )
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":playerId", player->getId());

    QMap<QString, double> winLoseStatistic;

    for (QList<QVariant>& category: _db->sqlQuery(sqlQuery))
    {
        winLoseStatistic["Gewonnen"] = category[0].toDouble();
        winLoseStatistic["Verloren"] = category[1].toDouble();
    }

    return winLoseStatistic;
}

/*!
 * \brief Gibt alle Spieler mit der relativen anzahl gewonnener Spiele des ausgewählten Turniers zurück
 *
 * \param[in] tournamentId id des Turniers, von dem die Daten angezeigt werden sollen
 *
 * \return als key steht der Spielername und als value die relative anzahl der gewinnenen Spiele in einer QMap
 *
 * Es wird ein Prozektwert zurückgegeben (0-1), der sich berechnet aus:
 * anzahl der Gewonnenen Spieler / Anzahl gespielete Spiele
 */
QMap<QString, double> PlayerStatistics::getWinningStatistic(const int tournamentId)
{
    QString sqlPrepare = R"(
SELECT name, sum(winning_games) AS winning_games, sum(played_games) AS played_games
FROM (
         SELECT player_id, 0 AS winning_games, sum(count) AS played_games
         FROM (

-- count the played games for player_a
                  SELECT player_a_id AS player_id, count(*) AS count
                  FROM game_board_list
                  WHERE sport_type_id = :sportTypeId
                    AND game_mode_id = :gameModeId
                    AND tournament_id = :tournamentId
                    AND player_a_id NOT NULL
                    AND player_b_id NOT NULL
                    AND winner_id NOT NULL
                  GROUP BY player_a_id

                  UNION

-- count the played games for player_b
                  SELECT player_b_id AS player_id, count(*) AS count
                  FROM game_board_list
                  WHERE sport_type_id = :sportTypeId
                    AND game_mode_id = :gameModeId
                    AND tournament_id = :tournamentId
                    AND player_a_id NOT NULL
                    AND player_b_id NOT NULL
                    AND winner_id NOT NULL
                  GROUP BY player_b_id
              )
         GROUP BY player_id
         UNION

-- count the winning games
         SELECT winner_id AS player_id, count(*) AS winning_games, 0 AS played_games
         FROM game_board_list
         WHERE sport_type_id = :sportTypeId
           AND game_mode_id = :gameModeId
           AND tournament_id = :tournamentId
           AND player_a_id NOT NULL
           AND player_b_id NOT NULL
           AND winner_id NOT NULL
         GROUP BY winner_id
     )
         INNER JOIN player_list pl ON player_id = pl.id
GROUP BY player_id
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", tournamentId);

    QList<QList<QVariant>> playerWinLoseGames = _db->sqlQuery(sqlQuery);

    QMap<QString, double> winningGamesStatistic;

    for (QList<QVariant>& player : playerWinLoseGames)
    {
        winningGamesStatistic[player[0].toString()] = player[1].toDouble() / player[2].toDouble();
    }

    return winningGamesStatistic;
}

/*!
 * \brief Gibt gewonnene und verlorene Spiele eines Spielers für das angegebene Turnier zurück
 *
 * \param[in] player spieler, von dem die gewonnenen und verlorenen Spiele zurückgegeben werden sollen.
 * \param[in] tournamentId id des Turniers, von dem die Daten angezeigt werden sollen
 *
 * \return als key stehen die Möglichkeiten ("Gewonnen", "Verloren") und als value die Anzahl der Spiele in einer QMap
 *
 * Es werden die Gewonnenen und die Verlorenen Spiele gezählt
 */
QMap<QString, double> PlayerStatistics::getWinningStatistic(const Player* player, const int tournamentId)
{
    QString sqlPrepare = R"(
SELECT sum(win) AS win, sum(lost) AS lost
FROM (
-- select win games
      SELECT count(*) AS win, 0 AS lost
      FROM game_board_list
      WHERE sport_type_id = :sportTypeId
        AND game_mode_id = :gameModeId
        AND player_a_id NOT NULL
        AND player_b_id NOT NULL
        AND winner_id = :playerId
        AND tournament_id = :tournamentId

      UNION

-- select lost games
      SELECT 0 AS win, count(*) AS lost
      FROM game_board_list
      WHERE sport_type_id = :sportTypeId
        AND game_mode_id = :gameModeId
        AND (player_a_id = :playerId OR player_b_id = :playerId)
        AND winner_id IS NOT :playerId
        AND tournament_id = :tournamentId
    )
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":playerId", player->getId());
    sqlQuery.bindValue(":tournamentId", tournamentId);

    QMap<QString, double> winLoseStatistic;

    for (QList<QVariant>& category: _db->sqlQuery(sqlQuery))
    {
        winLoseStatistic["Gewonnen"] = category[0].toDouble();
        winLoseStatistic["Verloren"] = category[1].toDouble();
    }

    return winLoseStatistic;
}

/*!
 * \brief Gibt alle geworfenen Punkte mit Zeitstempel zurück
 *
 * \return alle geworfenen Würfe mit zeitstempel
 *
 */
QMap<int, QMap<int, double>> PlayerStatistics::getAverageHistory(void)
{
    QString sqlPrepare = R"(
SELECT player_id
FROM leg_history_list
GROUP BY player_id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);

    QList<int> playerIds;
    for (QList<QVariant>& playerId : _db->sqlQuery(sqlQuery))
    {
        playerIds.append(playerId[0].toInt());
    }


    sqlPrepare = R"(
SELECT AVG(value_type_id * value) AS points
FROM leg_history_list
WHERE player_id = :playerId
GROUP BY sport_type_id, game_mode_id, game_board_id, leg_id
ORDER BY game_board_id, leg_id, id
)";

    QMap<int, QMap<int, double>> playerStatistic;

    for (int playerId : playerIds)
    {
        sqlQuery.clear();
        sqlQuery.prepare(sqlPrepare);
        sqlQuery.bindValue(":playerId", playerId);

        QMap<int, double> throwHistory;

        int nLeg = 0;
        for (QList<QVariant>& oneThrow : _db->sqlQuery(sqlQuery))
        {
            throwHistory[nLeg] = oneThrow[0].toDouble();
            nLeg++;
        }

        playerStatistic[playerId] = throwHistory;
    }

    return playerStatistic;
}

/*!
 * \brief Gibt alle geworfenen Punkte mit Zeitstempel zurück
 *
 * \param[in] tournamentId id des Turniers, von dem die Daten angezeigt werden sollen
 *
 * \return alle geworfenen Würfe mit zeitstempel
 *
 */
QMap<int, QMap<int, double>> PlayerStatistics::getAverageHistory(const int tournamentId)
{
    QString sqlPrepare = R"(
SELECT player_id
FROM leg_history_list
where tournament_id = :tournamentId
GROUP BY player_id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":tournamentId", tournamentId);

    QList<int> playerIds;
    for (QList<QVariant>& playerId : _db->sqlQuery(sqlQuery))
    {
        playerIds.append(playerId[0].toInt());
    }


    sqlPrepare = R"(
SELECT AVG(value_type_id * value) AS points
FROM leg_history_list
WHERE player_id = :playerId
  AND tournament_id = :tournamentId
GROUP BY sport_type_id, game_mode_id, game_board_id, leg_id
ORDER BY game_board_id, leg_id, id
)";

    QMap<int, QMap<int, double>> playerStatistic;

    for (int playerId : playerIds)
    {
        sqlQuery.clear();
        sqlQuery.prepare(sqlPrepare);
        sqlQuery.bindValue(":playerId", playerId);
        sqlQuery.bindValue(":tournamentId", tournamentId);

        QMap<int, double> throwHistory;

        int nLeg = 0;
        for (QList<QVariant>& oneThrow : _db->sqlQuery(sqlQuery))
        {
            throwHistory[nLeg] = oneThrow[0].toDouble();
            nLeg++;
        }

        playerStatistic[playerId] = throwHistory;
    }

    return playerStatistic;
}

/*!
 * \brief Gibt alle geworfenen Punkte mit Zeitstempel zurück
 *
 * \param[in] player spieler, von dem die Statistik angezeigt werden soll.
 *
 * \return alle geworfenen Würfe mit zeitstempel
 *
 */
QMap<int, QMap<int, double>> PlayerStatistics::getAverageHistory(const Player* player)
{
    QString sqlPrepare = R"(
SELECT AVG(value_type_id * value) AS points
FROM leg_history_list
WHERE player_id = :playerId
GROUP BY sport_type_id, game_mode_id, game_board_id, leg_id
ORDER BY game_board_id, leg_id, id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":playerId", player->getId());

    QMap<int, QMap<int, double>> playerStatistic;
    QMap<int, double> throwHistory;

    int nLeg = 0;
    for (QList<QVariant>& oneThrow : _db->sqlQuery(sqlQuery))
    {
        throwHistory[nLeg] = oneThrow[0].toDouble();
        nLeg++;
    }

    playerStatistic[player->getId()] = throwHistory;
    return playerStatistic;
}

/*!
 * \brief Gibt alle geworfenen Punkte mit Zeitstempel zurück
 *
 * \param[in] player Spieler, von dem die Statistik angezeigt werden soll.
 * \param[in] tournamentId id des Turniers, von dem die Daten angezeigt werden sollen
 *
 * \return alle geworfenen Würfe mit zeitstempel
 *
 */
QMap<int, QMap<int, double>> PlayerStatistics::getAverageHistory(const Player* player, const int tournamentId)
{
    QString sqlPrepare = R"(
SELECT AVG(value_type_id * value) AS points
FROM leg_history_list
WHERE player_id = :playerId
  AND tournament_id = :tournamentId
GROUP BY sport_type_id, game_mode_id, game_board_id, leg_id
ORDER BY game_board_id, leg_id, id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":playerId", player->getId());
    sqlQuery.bindValue(":tournamentId", tournamentId);

    QMap<int, QMap<int, double>> playerStatistic;
    QMap<int, double> throwHistory;

    int nLeg = 0;
    for (QList<QVariant>& oneThrow : _db->sqlQuery(sqlQuery))
    {
        throwHistory[nLeg] = oneThrow[0].toDouble();
        nLeg++;
    }

    playerStatistic[player->getId()] = throwHistory;
    return playerStatistic;
}

/*!
 * \brief Konvetiere die sqlQuery
 *
 * \param[in] sqlQuery sqlQuery mit den entsprechenden Daten
 *
 * \return Die 10 am meisten Getroffenen Felder
 *
 */
QMap<QString, double> PlayerStatistics::_convert10MostHittingFields(QSqlQuery& sqlQuery)
{
    QMap<QString, double> mostHittingFields;
    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQuery);

    QList<QString> multiplicatorName = {"Miss", "S", "D", "T"};
    for (QList<QVariant> field : rawData)
    {
        if (field[0] == 0) // miss
        {
            QString fieldName = multiplicatorName[field[0].toInt()];
            mostHittingFields[fieldName] = field[2].toDouble();
            continue;
        }

        QString fieldName = multiplicatorName[field[0].toInt()] + "-" + field[1].toString();
        mostHittingFields[fieldName] = field[2].toDouble();
    }

    return mostHittingFields;
}

/*!
 * \brief Gibt die 10 am meisten getroffene Felder
 *
 * \return Die 10 am meisten Getroffenen Felder
 *
 */
QMap<QString, double> PlayerStatistics::get10MostHittingFields()
{
    QString sqlPrepare = R"(
SELECT value_type_id, value, count(*) AS count
FROM leg_history_list
GROUP BY value_type_id, value
ORDER BY count DESC
LIMIT :limit
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":limit", _countOfMostHittingFields);

    return _convert10MostHittingFields(sqlQuery);
}

/*!
 * \brief Gibt die 10 am meisten getroffene Felder
 *
 * \param[in] player Spieler, von dem die Statistik angezeigt werden soll.
 *
 * \return Die 10 am meisten Getroffenen Felder
 *
 */
QMap<QString, double> PlayerStatistics::get10MostHittingFields(const Player* player)
{
    QString sqlPrepare = R"(
SELECT value_type_id, value, count(*) AS count
FROM leg_history_list
WHERE player_id = :playerId
GROUP BY value_type_id, value
ORDER BY count DESC
LIMIT :limit
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":limit", _countOfMostHittingFields);
    sqlQuery.bindValue(":playerId", player->getId());

    return _convert10MostHittingFields(sqlQuery);
}

/*!
 * \brief Gibt die 10 am meisten getroffene Felder
 *
 * \param[in] tournamentId id des Turniers, von dem die Daten angezeigt werden sollen
 *
 * \return Die 10 am meisten Getroffenen Felder
 *
 */
QMap<QString, double> PlayerStatistics::get10MostHittingFields(const int tournamentId)
{
    QString sqlPrepare = R"(
SELECT value_type_id, value, count(*) AS count
FROM leg_history_list
WHERE tournament_id = :tournamentId
GROUP BY value_type_id, value
ORDER BY count DESC
LIMIT :limit
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":limit", _countOfMostHittingFields);
    sqlQuery.bindValue(":tournamentId", tournamentId);

    return _convert10MostHittingFields(sqlQuery);
}

/*!
 * \brief Gibt die 10 am meisten getroffene Felder
 *
 * \param[in] player Spieler, von dem die Statistik angezeigt werden soll.
 * \param[in] tournamentId id des Turniers, von dem die Daten angezeigt werden sollen
 *
 * \return Die 10 am meisten Getroffenen Felder
 *
 */
QMap<QString, double> PlayerStatistics::get10MostHittingFields(const Player* player, const int tournamentId)
{
    QString sqlPrepare = R"(
SELECT value_type_id, value, count(*) AS count
FROM leg_history_list
WHERE player_id = :playerId
  AND tournament_id = :tournamentId
GROUP BY value_type_id, value
ORDER BY count DESC
LIMIT :limit
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":limit", _countOfMostHittingFields);
    sqlQuery.bindValue(":playerId", player->getId());
    sqlQuery.bindValue(":tournamentId", tournamentId);

    return _convert10MostHittingFields(sqlQuery);
}

/*!
 * \brief Konvetiere die sqlQuery
 *
 * \param[in] sqlQuery sqlQuery mit den entsprechenden Daten
 *
 * \return Average
 *
 */
QMap<QString, double> PlayerStatistics::_convertAverage(QSqlQuery& sqlQuery)
{
    QMap<QString, double> average;
    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQuery);

    for (QList<QVariant> field : rawData)
    {
        average[field[0].toString()] = field[1].toDouble();
    }

    return average;
}

/*!
 * \brief Gibt den Average zurück
 *
 * \return Average
 *
 */
QMap<QString, double> PlayerStatistics::getAverage(void)
{
    QString sqlPrepare = R"(
SELECT name, average
FROM (SELECT AVG(value_type_id * value) AS average, player_id
      FROM leg_history_list
      GROUP BY player_id)
         INNER JOIN player_list pl ON player_id = pl.id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);

    return _convertAverage(sqlQuery);
}

/*!
 * \brief Gibt den Average zurück
 *
 * \param[in] player Spieler, von dem die Statistik angezeigt werden soll.
 *
 * \return Average
 *
 */
// TODO: welche daten sollen vom spieler angezeigt werden???
QMap<QString, double> PlayerStatistics::getAverage(const Player* player)
{
    QString sqlPrepare = R"(
SELECT name, average
FROM (SELECT AVG(value_type_id * value) AS average, player_id
      FROM leg_history_list
      GROUP BY player_id)
         INNER JOIN player_list pl ON player_id = pl.id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);

    return _convertAverage(sqlQuery);
}

/*!
 * \brief Gibt den Average zurück
 *
 * \param[in] tournamentId id des Turniers, von dem die Daten angezeigt werden sollen
 *
 * \return Average
 *
 */
QMap<QString, double> PlayerStatistics::getAverage(const int tournamentId)
{
    QString sqlPrepare = R"(
SELECT name, average
FROM (SELECT AVG(value_type_id * value) AS average, player_id
      FROM leg_history_list
      WHERE tournament_id = :tournamentId
      GROUP BY player_id)
         INNER JOIN player_list pl ON player_id = pl.id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":tournamentId", tournamentId);

    return _convertAverage(sqlQuery);
}

/*!
 * \brief Gibt den Average zurück
 *
 * \param[in] player Spieler, von dem die Statistik angezeigt werden soll.
 * \param[in] tournamentId id des Turniers, von dem die Daten angezeigt werden sollen
 *
 * \return Average
 *
 */
// TODO: welche daten sollen vom spieler angezeigt werden???
QMap<QString, double> PlayerStatistics::getAverage(const Player* player, const int tournamentId)
{
    QString sqlPrepare = R"(
SELECT name, average
FROM (SELECT AVG(value_type_id * value) AS average, player_id
      FROM leg_history_list
      WHERE tournament_id = :tournamentId
      GROUP BY player_id)
         INNER JOIN player_list pl ON player_id = pl.id
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":tournamentId", tournamentId);

    return _convertAverage(sqlQuery);
}
