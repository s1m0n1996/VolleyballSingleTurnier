
#ifndef SPORTVERANSTALTUNG_CREATE_VOLLEYBALL_GAME_BOARD_H
#define SPORTVERANSTALTUNG_CREATE_VOLLEYBALL_GAME_BOARD_H

#include <QtCore>
#include <QList>
#include "Model/sqliteConnector.h"

class Player;
class SqliteConnector;
class GameManagement;

/*!
 * \file create_volleyball_game_board.h
 * \author Simon Flörke
 * \brief Erstellt einen gemischten Spielplan
 *
 * Es wird ein Spielplan in der Datenbank erstellt und dieser kann wieder gelesen werden.
 *
*/
class CreateVolleyballGameBoard : public QObject
{
Q_OBJECT

public:
    CreateVolleyballGameBoard(QList <Player>& players);

    void randomGame();

    int getCurrentGameId();

    void createOneGame();


private:


    Player getRandomBestPlayer();
    void initRand();
    Player getRandomPlayer();
    bool isPlayerAvailable(Player& player);
    bool isDifferentPlaysOkWhenPlayerPlays(Player& player);
    bool isMaleFemaleAllocationOk(Player& player, int maxDifference=2);
    QList<Player> _gamePlayers;
    SqliteConnector* _db;
    GameManagement* _gameManagement;
    const int _nPlayersPerTeam = 4;

};


#endif //SPORTVERANSTALTUNG_CREATE_VOLLEYBALL_GAME_BOARD_H
