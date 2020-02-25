#include "statistics.h"

Statistics::Statistics()
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();

}


double Statistics::getAverageOfPlayerInCurrentGame(Player player, int gameId)
{
    QString sqlPrepare = R"(
                         SELECT *
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
    sqlQuery.bindValue(":gameId", gameId);
    sqlQuery.bindValue(":playerId", player.getId());
    QList<QList<QVariant>> list = _db->sqlQuery(sqlQuery);
    return calculateAverage(list);
}


double Statistics::getAverageOfPlayerEver(Player player)
{
    QString sqlPrepare = R"(
                         SELECT *
                         FROM leg_history_list
                         WHERE sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND player_id = :playerId;
    )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":playerId", player.getId());
    QList<QList<QVariant>> list = _db->sqlQuery(sqlQuery);
    return calculateAverage(list);
}


double Statistics::calculateAverage(QList<QList<QVariant>> list)
{
    double average = 0;

    if (list.isEmpty())
    {
        qWarning() << "The list of throws of this Player is empty";
        return 0.0;
    }
    qDebug() << "Test:" << list.size();

    for (int i=0; i<list.size(); i++)
    {
        average += list[i][8].toDouble()  * list[i][9].toDouble();
    }

    return average/list.size();
}







