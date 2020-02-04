#include "gameboard.h"

Gameboard::Gameboard(QList <Player> players)
{
    _players = players;
}

void Gameboard::prepareGameRandomly()
{
    QRandomGenerator randomZahl;
    for(int i = 0; i < (_players.length()-1); i++)
    {
        _players.swap(i , randomZahl.bounded(_players.length()-1));
    }
}

