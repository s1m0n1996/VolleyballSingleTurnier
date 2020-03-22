#ifndef STATISTICS_H
#define STATISTICS_H

#include "Model/sqliteConnector.h"
#include "Model/player.h"
#include "Model/gameManagement.h"
#include "Model/game.h"

/*!
 * \file statistics.h
 * \author Philipp Wietfeld
 * \brief Eine Klasse für die Zuschaueransicht
 *
 * Stellt der Zuschaueransicht die Daten aus der Datenbank wie Average und andere nützliche
 * Informationen zum Turnier und Spieler zur Verfügung
*/

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
