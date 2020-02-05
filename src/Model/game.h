#ifndef GAME_H
#define GAME_H
#include "Model/player.h"


class Game
{
public:
    explicit Game(const Player playerA, const Player playerB);

    Player callWinner(void);

private:
    Player _playerA = Player(1);
    Player _playerB = Player(1);
};

#endif // GAME_H
