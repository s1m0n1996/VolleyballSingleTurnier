
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

    QSqlQueryModel* getDatabaseTableModel(){return _playerTable;}

    void addPlayerForNewGame(const Player addPlayer);

    void dropPlayerForNewGame(Player dropPlayer);

    int countSelectedPlayersForNewGame();

    int countMissingPlayersForNewGame();

    void printPlayersList();

private:
    SqliteConnector* _db;
    QList<Player> _selectedPlayersForNewGame;

    QSqlQueryModel* _playerTable;

    void _createPlayerTable();
};


#endif //SPORTVERANSTALTUNG_PLAYERMANAGEMENT_H
