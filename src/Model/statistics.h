#ifndef STATISTICS_H
#define STATISTICS_H
#include "Model/sqliteConnector.h"
#include "Model/player.h"
#include "Model/gameManagement.h"
#include "Model/game.h"

class Statistics
{
public:
    Statistics();

    double getAverageOfPlayerInCurrentLeg(const Player& player, const int legId);
    double getAverageOfPlayerInCurrentGame(const Player& player);
    double getAverageOfPlayerEver(const Player& player);
    int getCountOfHundretEightyInGame(const Player& player, const int gameId);
    int getWonTournamentsOfPlayer(const Player& player);
    int getWonGamesOfPlayer(const Player& player);
    int getCountOfHundretEightyInTournament(const Player& player);


private:
    SqliteConnector* _db;
    GameManagement* _gameManagement;
};

#endif // STATISTICS_H
