#ifndef REFEREE_H
#define REFEREE_H

#include <QList>

#include "Model/player.h"


class Referee
{
public:
    Referee();

    void singleThrowScore(int valueMultiplikator, int scoreWithoutMultiplikator);
    void undoThrow();
    void nextPlayer();

    int getRemainingScore();
    Player aktivePlayer();
    QList<int> getThrows();


};

#endif // REFEREE_H
