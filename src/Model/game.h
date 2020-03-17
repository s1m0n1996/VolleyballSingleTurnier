#ifndef GAME_H
#define GAME_H
#include "Model/player.h"
#include "Model/sqliteConnector.h"
#include "Model/gameManagement.h"
#include "Model/playerManagement.h"


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
    QString getNameOfPlayerForGameView(int playerId);

signals:
    void tournamentFinishes();

private:
    int getNextGameIdForPlayerA(void);
    int getNextGameIdForPlayerB(void);
    int getNumberOfWinnerInTournament(void);
    int getNumberOfGamesInTournament(void);
    void setWinnerPlayerAinDatabase(int winnerId);
    void setWinnerPlayerBinDatabase(int winnerId);
    void setWinnerToPrepareFinal(int winnerId);


    SqliteConnector* _db;
    GameManagement* _gameManagement;
    int _gameId;
    Player* _playerA = nullptr;
    Player* _playerB = nullptr;
    QList<QString> _allPlayers;

//signals:
    //void gameListChanged(void);

};

#endif // GAME_H
