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


// Gibt Average von allen Lags jemals an... Das älteste Leg ist das Erste in der Liste
QList<double> PlayerStatistics::averageOfAllLegs(Player& player)
{
    QList<double> averages;
    QString sqlPrepare = R"(
                         SELECT average
                         FROM (SELECT AVG(value_type_id * value) as average, leg_id, player_id, tournament_id, game_board_id
                              FROM leg_history_list group by player_id, tournament_id, game_board_id, leg_id)
                         Where player_id = :playerId;
    )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":playerId", player.getId());
    QList<QList<QVariant>> averageList = _db->sqlQuery(sqlQuery);

    for (const QList<QVariant>& list : averageList)
    {
        averages.append(list[0].toDouble());
    }

    return averages;
}

