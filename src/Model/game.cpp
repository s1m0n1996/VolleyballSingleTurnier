#include "Model/game.h"
#include "Model/gameManagement.h"
#include "Model/playerManagement.h"
#include "Model/sqliteConnector.h"

/*!
 * \brief Erstellt ein Spiel
 *
 * \return void
 *
 * Erstellt ein Spiel und lädt das nächste zu spielende Spiel aus der Datenbank
 *
 */
Game::Game(void)
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();
    loadNextGame();
}

/*!
 * \brief Lädt das nächste Spiel aus der Datenbank
 *
 * \return void
 *
 * Schreibt die ersten Spiele in die Datenbank
 *
 */
void Game::loadNextGame(void)
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
        qDebug() << "Was last game in Tournament";
        emit tournamentFinishes();
        return;
    }

    _gameId = newGame[0][0].toInt();
    if (newGame[0][1] > 0)
    {
        if (!_playerA)
        {
            delete _playerA;
        }
        _playerA = new Player(newGame[0][1].toInt());
    }
    if (newGame[0][2] > 0)
    {
        if (!_playerB)
        {
            delete _playerB;
        }
        _playerB = new Player(newGame[0][2].toInt());
    }
}

/*!
 * \brief Berechnet ob es das letzte Spiel in der Datenbank war
 *
 * \return bool
 *
 *
 */
bool Game::wasFinal(void)
{
    return getNumberOfGamesInTournament() - getNumberOfWinnerInTournament() == 1;
}

/*!
 * \brief Erzeugt die ersten Spiele des Turniers
 *
 * \param[in] die Spieler Id des Gewinners eines Spiels
 * \return void
 *
 * Schreibt die übergeben Gewinner Id in das aktuelle Spiel in der
 * Datenbank
 *
 */
void Game::setNextWinner(int winnerId)
{
    if (!wasFinal())
    {
        prepareNextGame(winnerId);
    }

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

/*!
 * \brief Schreibt die Gewinner in die Folgespiele
 *
 * \param[in]
 * \return void
 *
 * Schreibt die Gewinner des letzten Spiels in die nächsten Spiele und
 * berechnet die neuen Gegenspieler
 *
 */
void Game::prepareNextGame(int winnerId)
{
    int counter = getNumberOfWinnerInTournament() % 4;

    if (getNumberOfGamesInTournament() - getNumberOfWinnerInTournament() != 3)
    {
        if (counter == 0 or counter == 1)
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

/*!
 * \brief Schreibt die Gewinner für das Finale in die Datenbank
 *
 * \param[in] Spieler Id des Gewinners
 * \return void
 *
 * Behandelt einen Ausnahmefall falls das es die letzten beide Spiele vor dem Finale
 * waren und schreibt die beiden Gegenspieler in die Datenbank
 *
 */
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


    if (newGame.isEmpty())
    {
        setWinnerPlayerAinDatabase(winnerId);
    }
    else
    {
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

/*!
 * \brief Gibt die Anzahl der Gewinner des aktuellen Turniers wieder
 *
 * \return int
 *
 */
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

/*!
 * \brief Gibt die Anzahl von Spielen des aktuellen Turniers wieder
 *
 * \return int
 *
 */
int Game::getNumberOfGamesInTournament(void)
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

/*!
 * \brief Erzeugt eine Spielerliste in der angepassten Reihenfolge für den Turnierplan
 *
 * \return QList<QString>
 *
 */
QList<QString> Game::getAllPlayersForGameboardView(void)
{
    QList<QString> allPlayers;

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

    int iCounter;
    for (iCounter = 0; iCounter < (numberOfGames - 1); iCounter += 2)
    {
        allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter][0].toInt()));
        allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter + 1][0].toInt()));
        allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter][1].toInt()));
        allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter + 1][1].toInt()));

    }

    allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter][0].toInt()));
    allPlayers.append(getNameOfPlayerForGameView(allGames[iCounter][1].toInt()));

    return allPlayers;
}

QString Game::getNameOfPlayerForGameView(int playerId)
{
    return playerId > 0 ? Player(playerId).getName() : "";
}

QList<QString> Game::getAllWinnersInTournament(void)
{
    QList<QString> winnerList;
    QString sqlPrepare = R"(
                         SELECT id ,winner_id
                         FROM game_board_list
                         WHERE sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                         )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> nextGames = _db->sqlQuery(sqlQuery);

    for (const QVariant liste : nextGames)
    {
        Player player(liste.toList()[1].toInt());
        winnerList.append(player.getName());
    }
    return winnerList;
}
