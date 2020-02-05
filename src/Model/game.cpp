#include "Model/game.h"

Game::Game(const Player playerA, const Player playerB)
{
    _playerA = Player(playerA.getId());
    _playerB = Player(playerB.getId());

}
