
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

    QSqlQueryModel* getDeletedPlayerTableModel(){return _deletedPlayerTableModel;}

    void addPlayerForNewGame(const Player addPlayer);

    void addPlayerForNewGame(const QList<Player> addPlayers);

    void dropPlayerForNewGame(const Player dropPlayer);

    int countSelectedPlayersForNewGame();

    int countMissingPlayersForNewGame();

    void refreshDatabasePlayerTable();

    void refreshNextGamePlayerTable();

    void refreshDeletedPlayerTable();

    void dropPlayerFromDatabase(Player dropPlayer);

    void restorePlayerFromDatabase(const Player restoredPlayer);

    void savePictureForPlayer(const Player& player, const QByteArray& picture);

    QByteArray loadPictureForPlayer(const Player& player);

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
