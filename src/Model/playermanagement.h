
#ifndef SPORTVERANSTALTUNG_PLAYERMANAGEMENT_H
#define SPORTVERANSTALTUNG_PLAYERMANAGEMENT_H

#include <math.h>
#include <QString>
#include <QList>
#include <QVariant>
#include <QSqlTableModel>
#include <QSqlQueryModel>


#include "Model/sqliteConnector.h"
#include "Model/player.h"

/*!
 * \file playermanagement.h
 * \author Simon Flörke
 * \brief A class for the player management
 *
 *
*/
class PlayerManagement {

public:
    PlayerManagement();

    QList<Player> getAllStoredPlayers(bool onlyAvailable = true);

    QSqlQueryModel* getDatabaseTableModel(){return _databasePlayerTable;}

    QSqlQueryModel* getNextGamePlayerTableModel(){return _nextGamePlayerTableModel;}

    void addPlayerForNewGame(const Player addPlayer);

    void dropPlayerForNewGame(const Player dropPlayer);

    int countSelectedPlayersForNewGame();

    int countMissingPlayersForNewGame();

    void refreshDatabasePlayerTable();

private:
    SqliteConnector* _db;

    QSqlQueryModel* _databasePlayerTable = new QSqlQueryModel();
    QSqlQueryModel* _nextGamePlayerTableModel = new QSqlQueryModel();

    void _refreshNextGamePlayerTable();
};


#endif //SPORTVERANSTALTUNG_PLAYERMANAGEMENT_H
