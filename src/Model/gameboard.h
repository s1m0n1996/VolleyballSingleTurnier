#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Model/player.h"
#include <QRandomGenerator>


class Gameboard
{
public:
    Gameboard(QList <Player> players);
    void addGame(Player player1, Player player2);
    void prepareGameRandomly();


private:
    QList <Player> _players;
};

#endif // GAMEBOARD_H
