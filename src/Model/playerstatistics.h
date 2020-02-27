#ifndef PLAYERSTATISTICS_H
#define PLAYERSTATISTICS_H
#include "QList"
#include "Model/sqliteConnector.h"
#include "Model/player.h"
#include "Model/gameManagement.h"
#include "Model/statistics.h"


class Playerstatistics
{
public:
    Playerstatistics();
    int gamesLostOfPlayer(Player& player);
    QList<int> gamesWonAndLost(Player& player);
    QList<double> averageOfAllLegs(Player& player);

private:
    SqliteConnector* _db;
    GameManagement* _gameManagement;
};

#endif // PLAYERSTATISTICS_H
