#ifndef GAME_H
#define GAME_H
#include "Model/player.h"
#include "Model/sqliteConnector.h"


class Game
{
public:
    explicit Game(const int _tournamentId);


    void loadNextGame(void);
    void setNextWinner(int winnerId);
    void prepareNextGame(void);

private:
    SqliteConnector* _db;
    int _tournamentId;
    int _gameId;
    Player _playerA;
    Player _playerB;
};

#endif // GAME_H
