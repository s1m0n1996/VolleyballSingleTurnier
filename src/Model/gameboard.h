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
    Gameboard(QList <Player> players);
    void addGame(Player player1, Player player2);
    void prepareGameRandomly(void);
    void addWinnerToDatabank(int winnerId, int gameId);
    void getNewTournamentId(void);
    void randomInitialisation(void);
    void createFirstGame(QString tournemantName);
    void gameFlow(void);
    int getLastGameIdInSameTournament(void);
    int getGameId(void);

private:
    QList <Player> _players;
    QList <int> _gameIds;
    SqliteConnector* _db;
    int _tournamentId;
};

#endif // GAMEBOARD_H
