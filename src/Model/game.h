#ifndef GAME_H
#define GAME_H

#include "Model/player.h"

class GameManagement;
class PlayerManagement;
class SqliteConnector;



class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game();

    int getPlayerAId(){return _playerA->getId();}
    int getPlayerBId(){return _playerB->getId();}
    int getGameId(){return _gameId;}

    void loadNextGame(void);
    void setNextWinner(int winnerId);
    void prepareNextGame(int winnerId);
    bool wasFinal(void);
    QList<QString> getAllGamesWithNames();
    QList<QString> getAllPlayersForGameboardView();
    QList<QString> getAllWinnersInTournament();


signals:
    void tournamentFinishes();

private:
    QString getNameOfPlayerForGameView(int playerId);
    int getNextGameIdForPlayerA(void);
    int getNextGameIdForPlayerB(void);
    int getNumberOfWinnerInTournament(void);
    int getNumberOfGamesInTournament(void);
    void setWinnerPlayerAinDatabase(int winnerId);
    void setWinnerPlayerBinDatabase(int winnerId);
    void setWinnerToPrepareFinal(int winnerId);

    SqliteConnector* _db;
    GameManagement* _gameManagement;    
    Player* _playerA = nullptr;
    Player* _playerB = nullptr;
    QList<QString> _allPlayers;
    int _gameId;

};

#endif // GAME_H
