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
    void legWinningCondition();

    int getAktivePlayer();              //TODO: Name des Spielers
    int getAktivePlayerId();
    QList<int> getThrows();
    int getRemainScore();
    int getCountOfWinningLegs();
    int getGameStart();
    int getThrowScore();
    int getRemainingThrows();
    int getLastLegIdInSameGame();
    int getNumberOfCurrentLeg();

    void setActivePlayer(int activePlayerId);

    void loadLastGame();
    void loadLastThrows();

    // Methoden für den Zuschauer
    void scoreIsUnder170InLeg();
    void remainingThrowsAreZeroInLeg();
    int valueMultiplikator();
    int throwScoreWithoutMulti();
    QList<int> getRemainScoreForViewer();
    QList<int> getAllPlayersForViewer();

signals:
    void valueChanged(void);
    void playerBust(void);
    void playerWinsLeg(void);
    void scoreIsUnder170(void);
    void remainingThrowsAreZero(void);
    void gameListChanged(void);

public slots:
    void updatePlayer();

private:
    void setRemainScore();
    void setWinner();

     int _valueMultiplikator        = 0;
     int _throwScoreWithoutMulti    = 0;
     int _singleThrowScore          = 0;
     int _throwCounter              = 0;
     int _player                    = 0;
     int _winner                    = 0;
     int _throwScore                = 0;
     int _remainingThrows           = 0;
     int _gameId                    = 5;            // Muss noch übergeben werden.
     int _tournamentId              = 7;
     bool _wasLastThrowInLegToWin   = false;
     bool _wasLastThrowInLegToBust  = false;
     QList<int> _winningLegCounter  = {0,0};
     QList<int> _remainScore        = {501,501};
     QList<int> _allThrows          = {0,0,0};
     //TODO: Muss noch ein String werden. Mit der Game Klasse absprechen
     QList<int> _allPlayers;
     SqliteConnector* _db;
     GameManagement* _gameManagement;
};

#endif // REFEREE_H
