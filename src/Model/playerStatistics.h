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

    QMap<int, QMap<int, double>> getAverageHistory(void);
    QMap<int, QMap<int, double>> getAverageHistory(const Player* player);
    QMap<int, QMap<int, double>> getAverageHistory(const int tournamentId);
    QMap<int, QMap<int, double>> getAverageHistory(const Player* player, const int tournamentId);

    QMap<QString, double> get10MostHittingFields(void);
    QMap<QString, double> get10MostHittingFields(const Player* player);
    QMap<QString, double> get10MostHittingFields(const int tournamentId);
    QMap<QString, double> get10MostHittingFields(const Player* player, const int tournamentId);

    QMap<QString, double> getAverage(void);
    QMap<QString, double> getAverage(const int tournamentId);

private:
    SqliteConnector* _db;
    GameManagement* _gameManagement;

    const int _countOfMostHittingFields = 10;

    QMap<QString, double> _convert10MostHittingFields(QSqlQuery& sqlQuery);
    QMap<QString, double> _convertAverage(QSqlQuery& sqlQuery);
};

#endif // PLAYERSTATISTICS_H
