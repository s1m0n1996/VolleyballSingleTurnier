#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Model/player.h"
#include "Model/game.h"
#include "Model/game.h"
#include <QList>
#include <ctime>


class Gameboard
{
public:
    Gameboard(QList <Player> players);
    void addGame(Player player1, Player player2);
    void prepareGameRandomly();
    void addPlayerToDatabank();
    void getNewTournamentId();
    void randomInitialisation();
    void createGame();

private:
    QList <Player> _players;
    SqliteConnector* _db;
    int _tournamentId;
};

#endif // GAMEBOARD_H
