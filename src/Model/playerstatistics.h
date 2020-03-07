#ifndef PLAYERSTATISTICS_H
#define PLAYERSTATISTICS_H
#include "QList"
#include "Model/sqliteConnector.h"
#include "Model/player.h"
#include "Model/gameManagement.h"
#include "Model/statistics.h"
#include <QMap>

/*!
 * \file playerstatistic.h
 * \author Simon Flörke
 * \brief Holt alle Daten Für das Statistic Fenster
 *
 * Diese klasse holt alle Daten, um diese im Statistik Fenster anzuzeigen.
 *
*/
class PlayerStatistics
{
public:
    PlayerStatistics();

    QMap<QString, double> getWinningStatistic(void);
    QMap<QString, double> getWinningStatistic(const Player* player);
    QMap<QString, double> getWinningStatistic(const int tournamentId);
    QMap<QString, double> getWinningStatistic(const Player* player, const int tournamentId);

    QList<double> averageOfAllLegs(Player& player);

private:
    SqliteConnector* _db;
    GameManagement* _gameManagement;
};

#endif // PLAYERSTATISTICS_H
