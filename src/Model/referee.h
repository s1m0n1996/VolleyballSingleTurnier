#ifndef REFEREE_H
#define REFEREE_H

#include <QList>
#include <QObject>

#include "Model/game.h"


class Referee : public QObject
{
    Q_OBJECT
public:
    Referee();

    void singleThrowScore(int valueMultiplikator, int scoreWithoutMultiplikator);
    void undoThrow();
    void nextPlayer();
    void setRemainScore();
    void setWinner();
    void legWinningCondition();


   // Game getAktivePlayer();
    QList<int> getThrows();
    int getAktivePlayer();
    int getRemainScore();
    QString getWinner();
    int getCountOfWinningLegs();
    int getGameStart();
    int  getThrowScore();

signals:
    void valueChanged(void);
    void playerBust(void);
    void playerWinsLeg(void);

private:
     int _valueMultiplikator        = 0;
     int _singleThrowScore          = 0;
     int _throwCounter              = 0;
     int _player                    = 0;
     int _winner                    = 0;
     int _throwScore                = 0;
     bool _wasLastThrowInLegToWin   = false;
     bool _wasLastThrowInLegToBust  = false;
     QList<int> _winningLegCounter  = {0,0};
     QList<int> _remainScore        = {501,501};
     QList<int> _allThrows          = {0,0,0};
};

#endif // REFEREE_H
