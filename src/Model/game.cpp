#include "Model/game.h"

// FIXME: nicht immer einfach spieler 2 und 4 aufrufen
Game::Game():_playerA(2),_playerB(4)
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();
    _isLastgame = false;
    loadNextGame();
}

//TODO: GameId muss noch aus der Datenbank entnommen werden


void Game::loadNextGame()
{

    QString sqlPrepare = R"(SELECT *
                         FROM game_board_list
                         WHERE sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                           AND winner_id IS NULL
                         ORDER by id asc
                         LIMIT 1)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> newGame = _db->sqlQuery(sqlQuery);
    // TODO: überprüfe hotfix
    if (newGame.isEmpty())
    {
        qDebug() << "Was last game in Tournament";          //TODO: Die Dartscheibe unable setzten!!
        _isLastgame = true;
        return;
    }
    Player playerA(newGame[0][5].toInt());
    _playerA = playerA;
    Player playerB(newGame[0][6].toInt());
    _playerB = playerB;
    _gameId = newGame[0][0].toInt();
}


void Game::setNextWinner(int winnerId)
{
    QString sqlPrepare = R"(
                         UPDATE game_board_list
                         SET winner_id = :winnerId
                         WHERE id = :gameId
                           AND sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId;)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":winnerId", winnerId);
    sqlQuery.bindValue(":gameId", _gameId);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    _db->sqlQuery(sqlQuery);
    prepareNextGame(winnerId);
}


void Game::prepareNextGame(int winnerId)
{
    QString sqlPrepare = R"(
            SELECT *
            FROM game_board_list
            WHERE sport_type_id = :sportTypeId
              AND game_mode_id = :gameModeId
              AND tournament_id = :tournamentId
              AND winner_id IS NULL
              AND (player_a_id IS NULL
              OR  player_b_id IS NULL)
            ORDER by id asc
            LIMIT 1)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> nextGames = _db->sqlQuery(sqlQuery);

    if (nextGames[0][5].isNull())
    {
        QString sqlPrepare2 = R"(
                             UPDATE game_board_list
                             SET player_a_id = :playerAId
                             WHERE id = :gameId
                               AND sport_type_id = :sportTypeId
                               AND game_mode_id = :gameModeId
                               AND tournament_id = :tournamentId
                              )";
        QSqlQuery sqlQuery;
        sqlQuery.prepare(sqlPrepare2);
        sqlQuery.bindValue(":playerAId", winnerId);
        sqlQuery.bindValue(":gameId", nextGames[0][0].toInt());
        sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
        sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
        sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
        _db->sqlQuery(sqlQuery);
    }
    else
    {
        QString sqlPrepare2 = R"(
                             UPDATE game_board_list
                             SET player_b_id = :playerBId
                             WHERE id = :gameId
                               AND sport_type_id = :sportTypeId
                               AND game_mode_id = :gameModeId
                               AND tournament_id = :tournamentId;)";
        QSqlQuery sqlQuery;
        sqlQuery.prepare(sqlPrepare2);
        sqlQuery.bindValue(":playerBId", winnerId);
        sqlQuery.bindValue(":gameId", nextGames[0][0]);
        sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
        sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
        sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
        _db->sqlQuery(sqlQuery);
    }
}


QList<QString> Game::getAllPlayersForGameboardView()
{
    QList <QString> allPlayers;

    QString sqlPrepare = R"(
                         SELECT MAX(id)
                         FROM game_board_list
                         WHERE tournament_id = :tournamentId
                         )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> countOfGames = _db->sqlQuery(sqlQuery);
    int numberOfGames = countOfGames[0][0].toInt();

    QString sqlPrepare2 = R"(
                         SELECT player_a_id, player_b_id
                         FROM game_board_list
                         WHERE tournament_id = :tournamentId
                         )";
    QSqlQuery sqlQuery2;
    sqlQuery2.prepare(sqlPrepare2);
    sqlQuery2.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> allGames = _db->sqlQuery(sqlQuery2);

    qDebug() << "Anzahl Spiele:" << numberOfGames;
    qDebug() << allGames;
    PlayerManagement playerManagement;

    for (int i=0; i < numberOfGames; i+=2)
    {
        qDebug() << playerManagement.countSelectedPlayersForNewGame();
        if(i <= (playerManagement.countSelectedPlayersForNewGame()/2))                   //Finale wird hier betrachtet.
        {
            allPlayers.append(getNameOfPlayerForGameView(allGames[i][0].toInt()));
            allPlayers.append(getNameOfPlayerForGameView(allGames[i+1][0].toInt()));
            allPlayers.append(getNameOfPlayerForGameView(allGames[i][1].toInt()));
            allPlayers.append(getNameOfPlayerForGameView(allGames[i+1][1].toInt()));
        }
        else
        {            
            allPlayers.append(getNameOfPlayerForGameView(allGames[i][0].toInt()));
            allPlayers.append(getNameOfPlayerForGameView(allGames[i][1].toInt()));
        }

    }
    return allPlayers;
}


QString Game::getNameOfPlayerForGameView(int playerId)
{
    qDebug() << "SpielerId:" << playerId;
    Player player(playerId);
    return player.getName();
}


