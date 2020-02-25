#ifndef GAME_H
#define GAME_H
#include "Model/player.h"
#include "Model/sqliteConnector.h"
#include "Model/gameManagement.h"


class Game
{
public:
    explicit Game();

    void loadNextGame(void);
    void setNextWinner(int winnerId);
    void prepareNextGame(int winnerId);
    int getPlayerAId(){return _playerA.getId();}
    int getPlayerBId(){return _playerB.getId();}

private:
    SqliteConnector* _db;
    GameManagement* _gameManagement;
    int _gameId;
    Player _playerA;
    Player _playerB;
};

#endif // GAME_H
