#include "Model/game.h"

Game::Game(const int tournamentId)
{
    _db = &SqliteConnector::instance();
    _tournamentId = tournamentId;
    qDebug() << 1;
}


void Game::loadNextGame()
{
    QString sqlPrepare = R"(SELECT *
                         FROM game_board_list
                         WHERE sport_type_id = 1
                           AND game_mode_id = 1
                           AND tournament_id = ?
                           AND winner_id IS NULL
                         ORDER by id asc
                         LIMIT 1)";
    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(_tournamentId));
    QList<QList<QVariant>> newGame = _db->sqlQuery(sqlPrepare, sqlParameters);
    Player playerA(newGame[0][5].toInt());
    //_playerA = playerA;
    Player playerB(newGame[0][6].toInt());
    //_playerB = playerB;
    _gameId = newGame[0][0].toInt();
}


void Game::setNextWinner(int winnerId)
{
    QString sqlPrepare = R"(
                         UPDATE game_board_list
                         SET winner_id = ?
                         WHERE id = ?
                           AND sport_type_id = 1
                           AND game_mode_id = 1
                           AND tournament_id = ?;)";
    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(winnerId));
    sqlParameters.append(QString::number(_gameId));
    sqlParameters.append(QString::number(_tournamentId));
    _db->sqlQuery(sqlPrepare, sqlParameters);
}


