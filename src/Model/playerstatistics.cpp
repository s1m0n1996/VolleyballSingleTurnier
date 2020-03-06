#include "playerstatistics.h"

Playerstatistics::Playerstatistics()
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();
}


QList<int> Playerstatistics::gamesWonAndLost(const Player &player)
{
    QList<int> winLoseStatistics;
    Statistics wins;
    winLoseStatistics.append(wins.getWonGamesOfPlayer(player));
    winLoseStatistics.append(gamesLostOfPlayer(player));

    return winLoseStatistics;
}


int Playerstatistics::gamesLostOfPlayer(const Player& player)
{
    QString sqlPrepare = R"(
                         SELECT *
                         FROM game_board_list
                         WHERE winner_id IS NOT NULL
                         AND  winner_id != :playerId
                         AND (player_a_id = :playerId
                         OR  player_b_id = :playerId)
    )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":playerId", player.getId());
    QList<QList<QVariant>> list = _db->sqlQuery(sqlQuery);
    return list.size();
}


// Gibt Average von allen Lags jemals an... Das älteste Leg ist das Erste in der Liste
QList<double> Playerstatistics::averageOfAllLegs(const Player& player)
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

    for(const QList<QVariant>& list : averageList){
        averages.append(list[0].toDouble());
    }

    return averages;
}
