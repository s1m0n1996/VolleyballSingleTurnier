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

    QString sqlPrepare = R"(SELECT id, player_a_id, player_b_id
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

    if (newGame.isEmpty())
    {
        qDebug() << "Was last game in Tournament";          //TODO: Die Dartscheibe unable setzten!!
        _isLastgame = true;
        return;
    }

    _gameId = newGame[0][0].toInt();
    if (newGame[0][1] > 0)
    {
        _playerA = Player(newGame[0][1].toInt());
    }
    if (newGame[0][2] > 0)
    {
        _playerB = Player(newGame[0][2].toInt());
    }
}


void Game::setNextWinner(int winnerId)
{
    prepareNextGame(winnerId);
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

}


void Game::prepareNextGame(int winnerId)
{
    int counter = getNumberOfWinnerInTournament()%4;

    if(getNumberOfGamesInTournament() - getNumberOfWinnerInTournament()!=3)
    {
        if(counter == 0 or counter == 1)
        {
            setWinnerPlayerAinDatabase(winnerId);
        }
        else
        {
            setWinnerPlayerBinDatabase(winnerId);
        }

    }
    else
    {
        setWinnerToPrepareFinal(winnerId);
    }
}


void Game::setWinnerToPrepareFinal(int winnerId)
{
    QString sqlPrepare = R"(
                         SELECT player_a_id
                         FROM game_board_list
                         WHERE sport_type_id = :sportTypeId
                         AND game_mode_id = :gameModeId
                         AND tournament_id = :tournamentId
                         AND id = (SELECT MAX(id)
                                   FROM game_board_list
                                   WHERE sport_type_id = :sportTypeId
                                   AND game_mode_id = :gameModeId
                                   AND tournament_id = :tournamentId
                                   ORDER by id asc
                                   LIMIT 1))";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> newGame = _db->sqlQuery(sqlQuery);

    qDebug() << "Final:" << newGame;

    if(newGame.isEmpty())
    {
        qDebug() << "Finaascaw";
        setWinnerPlayerAinDatabase(winnerId);
    }
    else
    {
        qDebug() << "Finawdapü,acwascaw";
        setWinnerPlayerBinDatabase(winnerId);
    }
}


void Game::setWinnerPlayerAinDatabase(int winnerId)
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
    sqlQuery.bindValue(":gameId", getNextGameIdForPlayerA());
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    _db->sqlQuery(sqlQuery);
}


void Game::setWinnerPlayerBinDatabase(int winnerId)
{
    QString sqlPrepare2 = R"(
                         UPDATE game_board_list
                         SET player_b_id = :playerBId
                         WHERE id = :gameId
                           AND sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                          )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare2);
    sqlQuery.bindValue(":playerBId", winnerId);
    sqlQuery.bindValue(":gameId", getNextGameIdForPlayerB());
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    _db->sqlQuery(sqlQuery);
}


int Game::getNextGameIdForPlayerA(void)
{
    QString sqlPrepare = R"(
                         SELECT *
                         FROM game_board_list
                         WHERE sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                           AND winner_id IS NULL
                           AND player_a_id IS NULL
                         ORDER by id asc
                         LIMIT 1)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> nextGames = _db->sqlQuery(sqlQuery);
    return nextGames[0][0].toInt();
}


int Game::getNextGameIdForPlayerB(void)
{
    QString sqlPrepare = R"(
                         SELECT *
                         FROM game_board_list
                         WHERE sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                           AND winner_id IS NULL
                           AND player_b_id IS NULL
                         ORDER by id asc
                         LIMIT 1)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> nextGames = _db->sqlQuery(sqlQuery);
    return nextGames[0][0].toInt();
}


int Game::getNumberOfWinnerInTournament(void)
{
    QString sqlPrepare = R"(
                         SELECT MAX(id)
                         FROM game_board_list
                         WHERE sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                           AND winner_id IS NOT NULL
                         ORDER by id asc
                         LIMIT 1)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> nextGames = _db->sqlQuery(sqlQuery);
    return nextGames[0][0].toInt();
}


int Game::getNumberOfGamesInTournament()
{
    QString sqlPrepare = R"(
                         SELECT MAX(id)
                         FROM game_board_list
                         WHERE sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                         ORDER by id asc
                         LIMIT 1)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> nextGames = _db->sqlQuery(sqlQuery);
    return nextGames[0][0].toInt();
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

    int iCounter;
    for (iCounter=0; iCounter < (numberOfGames-1); iCounter+=2)
    {
        allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter][0].toInt()));
        allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter+1][0].toInt()));
        allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter][1].toInt()));
        allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter+1][1].toInt()));

    }

    allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter][0].toInt()));
    allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter][1].toInt()));

    return allPlayers;
}


QString Game::getNameOfPlayerForGameView(int playerId)
{
    qDebug() << "SpielerId:" << playerId;
    return playerId > 0 ? Player(playerId).getName() : "";
}
