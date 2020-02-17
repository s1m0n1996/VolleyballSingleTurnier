#ifndef REFEREE_H
#define REFEREE_H

#include <QList>
#include <QObject>
#include <QDebug>

#include "Model/sqliteConnector.h"
#include "Model/game.h"

/*!
 * \file referee.h
 * \author Nico Meyer
 * \brief Eine Klasse für den Richter
 *
*/
class Referee : public QObject
{
    Q_OBJECT
public:
    Referee();

    void singleThrowScore(int valueMultiplikator, int scoreWithoutMultiplikator);
    void undoThrow();
    void nextPlayer();
    void nextPlayerAfterWinningLeg();
    void setRemainScore();
    void setWinner();
    void legWinningCondition();


    int getAktivePlayer();              //TODO: Name des Spielers
    int getAktivePlayerId();
    QList<int> getThrows();
    int getRemainScore();
    QString getWinner();
    int getCountOfWinningLegs();
    int getGameStart();
    int  getThrowScore();
    int getRemainingThrows();
    int getLastLegIdInSameGame();
    int getNumberOfCurrentLeg();

    // Methoden für den Zuschauer
    QList<int> getRemainScoreForViewer();
    QList<int> getWinningLegCounterForViewer();
    QList<int> getAllPlayersForViewer();

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
     int _remainingThrows           = 0;
     int _tournamentId              = 7;            //TODO: tournamentId übergeben oder durch singletonklasse bekommen
     int _gameId                    = 5;            // Hier auch!
     int _playerAId;
     int _playerBId;
     bool _wasLastThrowInLegToWin   = false;
     bool _wasLastThrowInLegToBust  = false;
     QList<int> _winningLegCounter  = {0,0};
     QList<int> _remainScore        = {501,501};
     QList<int> _allThrows          = {0,0,0};
     //TODO: Muss noch ein String werden. Mit der Game Klasse absprechen
     QList<int> _allPlayers         ={1,2};
     SqliteConnector* _db;
};

#endif // REFEREE_H
