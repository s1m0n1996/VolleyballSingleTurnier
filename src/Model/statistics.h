#ifndef STATISTICS_H
#define STATISTICS_H
#include "Model/sqliteConnector.h"
#include "Model/player.h"
#include "Model/gameManagement.h"

class Statistics
{
public:
    Statistics();

    double getAverageOfPlayerInCurrentLeg(Player& player, int gameId, int legId);
    double getAverageOfPlayerInCurrentGame(Player& player, int gameId);
    double getAverageOfPlayerEver(Player& player);
    int getCountOfHundretEightyInGame(Player& player, int gameId);
    int getWonTournamentsOfPlayer(Player& player);
    int getWonGamesOfPlayer(Player& player);
    int getCountOfHundretEightyInTournament(Player& player);


private:
    SqliteConnector* _db;
    GameManagement* _gameManagement;
    double calculateAverage(QList<QList<QVariant>> list);
};

#endif // STATISTICS_H
