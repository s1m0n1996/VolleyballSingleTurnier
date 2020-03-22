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
 * \brief Model Klasse der Meldestelle
 *
 *
*/
class PlayerManagement : public QObject{

    Q_OBJECT
public:
    PlayerManagement(void);

    QList<Player> getAllStoredPlayers(bool onlyAvailable = true);

    QList<Player> getPlayersForNextGame(void);

    QSqlQueryModel* getDatabaseTableModel(void){return _databasePlayerTable;}

    QSqlQueryModel* getNextGamePlayerTableModel(void){return _nextGamePlayerTableModel;}

    QSqlQueryModel* getDeletedPlayerTableModel(void){return _deletedPlayerTableModel;}

    void addPlayerForNewGame(const Player& addPlayer);

    void deletePlayerForNewGame(const Player& deletePlayer);

    int countSelectedPlayersForNewGame(void);

    int countMissingPlayersForNewGame(void);

    void refreshDatabasePlayerTable(void);

    void refreshNextGamePlayerTable(void);

    void refreshDeletedPlayerTable(void);

    void deletePlayerFromDatabase(Player& deletePlayer);

    void restorePlayerFromDatabase(const Player& restoredPlayer);

signals:
    void valueChanged(void);


private:
    SqliteConnector* _db;
    GameManagement* _gameManagement;

    QSqlQueryModel* _databasePlayerTable = new QSqlQueryModel();
    QSqlQueryModel* _nextGamePlayerTableModel = new QSqlQueryModel();
    QSqlQueryModel* _deletedPlayerTableModel = new QSqlQueryModel();

};


#endif //SPORTVERANSTALTUNG_PLAYERMANAGEMENT_H
