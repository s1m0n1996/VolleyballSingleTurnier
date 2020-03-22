#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Model/player.h"
#include "Model/game.h"
#include <QList>
#include <ctime>
#include "QDate"
#include "Model/gameManagement.h"

/*!
 * \file gameboard.h
 * \author Philipp Wietfeld
 * \brief Eine Klasse zur Erstellung der ersten Spiele in dem Turnier
 *
 * Bereitet die Datenbank auf ein neues Turnier vor, indem es die TurnierId erzeugt und alle
 * möglichen Spiele des Turniers vorbereitend in die Datenbank schreibt
 *
*/

class Gameboard
{
public:
    Gameboard(QList <Player> players);
    void addWinnerToDatabase(int winnerId, int gameId);

private:
    GameManagement* _gameManagement;
    QList <Player> _players;
    QList <int> _gameIds;
    SqliteConnector* _db;
    int getLastGameIdInSameTournament(void);
    void createFirstGames(void);
    void prepareGameRandomly(void);
    void getNewTournamentId(void);
    void randomInitialisation(void);
    void createRemainingGames(void);
    void createNewTournament(QString tournamentName);
};

#endif // GAMEBOARD_H
