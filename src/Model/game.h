#ifndef GAME_H
#define GAME_H
#include "Model/player.h"
#include "Model/sqliteConnector.h"
#include "Model/gameManagement.h"


class Game //: public QObject
{
        //Q_OBJECT
public:
    explicit Game();

    void loadNextGame(void);
    void setNextWinner(int winnerId);
    void prepareNextGame(int winnerId);
    int getPlayerAId(){return _playerA.getId();}
    int getPlayerBId(){return _playerB.getId();}
    int getGameId(){return _gameId;}
    bool getIsLastGame(){return _isLastgame;}
    QList<QString> getAllGamesWithNames();

    QList<QString> getAllPlayersForGameboardView();
    QString getNameOfPlayerForGameView(int playerId);

private:
    SqliteConnector* _db;
    GameManagement* _gameManagement;
    int _gameId;
    bool _isLastgame;
    Player _playerA;
    Player _playerB;

//signals:
    //void gameListChanged(void);

};

#endif // GAME_H
