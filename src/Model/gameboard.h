#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Model/player.h"
#include "Model/game.h"
#include "Model/game.h"
#include <QList>
#include <ctime>
#include "QDate"


class Gameboard
{
public:
    Gameboard(QList <Player> players, QString tournamentName);
    void addWinnerToDatabase(int winnerId, int gameId);

private:
    QList <Player> _players;
    QList <int> _gameIds;
    SqliteConnector* _db;
    int _tournamentId;
    int _totalGames;
    int getLastGameIdInSameTournament(void);
    void createFirstGames();
    void prepareGameRandomly(void);
    void getNewTournamentId(void);
    void randomInitialisation(void);
    void createRemainingGames();
    void createNewTournament(QString tournamentName);
};

#endif // GAMEBOARD_H
