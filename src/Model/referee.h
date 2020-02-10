#ifndef REFEREE_H
#define REFEREE_H

#include <QList>

#include "Model/game.h"


class Referee
{
public:
    Referee();

    void singleThrowScore(int valueMultiplikator, int scoreWithoutMultiplikator);
    void undoThrow();
    void nextPlayer();
    void setRemainScore();

   // Game getAktivePlayer();
    QList<int> getThrows();
    int getRemainScore();

signals:
    void valueChanged(void);

private:
     int _singleThrowScore      = 0;
     int _throwCounter          = 0;
     int _player                = 0;
     QList<int> _remainScore    = {501,501};
     QList<int> _allThrows      = {0,0,0};
};

#endif // REFEREE_H
