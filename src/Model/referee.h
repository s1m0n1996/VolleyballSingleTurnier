#ifndef REFEREE_H
#define REFEREE_H

#include <QList>
#include <QObject>

#include "Model/sqliteConnector.h"
#include "Model/game.h"

/*!
 * \file referee.h
 * \author Nico Meyer, Philipp Wietfeld
 * \brief Eine Klasse für den Richter
 *
*/
class Referee : public QObject
{
    Q_OBJECT
public:
    Referee(void);

    void singleThrowScore(int valueMultiplikator, int scoreWithoutMultiplikator);
    void undoThrow(void);
    void nextPlayer(void);
    void nextPlayerAfterWinningLeg(void);
    void legWinningCondition(void);

    int getAktivePlayer(void);
    QList<int> getThrows(void);
    int getRemainScore(void);
    int getCountOfWinningLegs(void);
    int getGameStart(void);
    int getThrowScore(void);
    int getRemainingThrows(void);

    // Methoden für den Zuschauer
    void scoreIsUnder170InLeg(void);
    void remainingThrowsAreZeroInLeg(void);
    int getValueMultiplikator(void);
    QList<int> getAllThrowsWithoutMultiplikator(void);
    QList<int> getRemainScoreForViewer(void);
    QList<int> getAllPlayersForViewer(void);

signals:
    void valueChanged(void);
    void playerBust(void);
    void playerWinsLeg(void);
    void scoreIsUnder170(void);
    void remainingThrowsAreZero(void);
    void gameListChanged(void);
    void tournamentFinished(void);

public slots:
    void updatePlayer(void);

private:   
    int getLastLegIdInSameGame(void);
    int getNumberOfCurrentLeg(void);
    void setRemainScore(void);
    void setWinner(void);
    void setActivePlayer(int activePlayerId);
    void resetAllStats(void);
    void loadLastGame(void);
    void loadLastThrows(void);
    void setLegWinner(int winnerId);
    void createAllPossibleLegs(void);
    void writeLegIntoDatabase(int legId);

     int _valueMultiplikator        = 0;
     int _throwScoreWithoutMulti    = 0;
     int _singleThrowScore          = 0;
     int _throwCounter              = 0;
     int _player                    = 0;
     int _winner                    = 0;
     int _throwScore                = 0;
     int _remainingThrows           = 0;
     int _gameId                    = 0;
     int _tournamentId              = 0;
     bool _gameWasLoaded            = false;
     bool _wasLastThrowInLegToWin   = false;
     bool _wasLastThrowInLegToBust  = false;
     QList<int> _winningLegCounter  = {0,0};
     QList<int> _remainScore        = {501,501};
     QList<int> _allThrows          = {0,0,0};
     QList<int> _allThrowsWithoutMulti = {0,0,0};
     QList<int> _allPlayers;
     SqliteConnector* _db;
     GameManagement* _gameManagement;

};

#endif // REFEREE_H
