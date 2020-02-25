
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
#include "Model/gameManagement.h"

/*!
 * \file playermanagement.h
 * \author Simon Flörke
 * \brief A class for the player management
 *
 *
*/
class PlayerManagement : public QObject{

    Q_OBJECT
public:
    PlayerManagement();

    QList<Player> getAllStoredPlayers(bool onlyAvailable = true);

    QList<Player> getPlayersForNextGame();

    QSqlQueryModel* getDatabaseTableModel(){return _databasePlayerTable;}

    QSqlQueryModel* getNextGamePlayerTableModel(){return _nextGamePlayerTableModel;}

    void addPlayerForNewGame(const Player addPlayer);

    void addPlayerForNewGame(const QList<Player> addPlayers);

    void dropPlayerForNewGame(const Player dropPlayer);

    int countSelectedPlayersForNewGame();

    int countMissingPlayersForNewGame();

    void refreshDatabasePlayerTable();

    void refreshNextGamePlayerTable();

    void dropPlayerFromDatabase(Player dropPlayer);

signals:
    void valueChanged(void);


private:
    SqliteConnector* _db;
    GameManagement* _gameManagement;

    QSqlQueryModel* _databasePlayerTable = new QSqlQueryModel();
    QSqlQueryModel* _nextGamePlayerTableModel = new QSqlQueryModel();

};


#endif //SPORTVERANSTALTUNG_PLAYERMANAGEMENT_H
