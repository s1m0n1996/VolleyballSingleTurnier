
#ifndef SPORTVERANSTALTUNG_GAMEMANAGEMENT_H
#define SPORTVERANSTALTUNG_GAMEMANAGEMENT_H

#include "Model/sqliteConnector.h"

/*!
 * \file gameManagement.h
 * \brief this class handles the global parameter for the game
 * \author Simon Flörke
 *
 * Thia class is a singleton class and would be used as global parameters that all models works with
 * the same tournament and game.
*/
class GameManagement : public QObject
{
Q_OBJECT
public:
    static GameManagement& instance(void)
    {
        static GameManagement instance;
        return instance;
    }
    int getTournamentId(void){ return _tournamentId;}
    int getSportTypeId(void){ return  _sportTypeId;}
    int getGameModeId(void){ return _gameModeId;}
    QString getTournamentName(void) {return _tournamentName;}
    QString getTournamentDate(void) {return _tournamentDate;}
    bool isTournamentStarted(void){return _isTournamentStarted;}

    void setIsTournamentStarted(void);

    void createNewTournament(const QString& name, const QDate& date);
    void loadOtherTournament(const QString& name, const QDate& date);
    void loadOtherTournament(int& id);
    void loadLastTournament(void);
    QList<QList<QString>> getSavedTournaments(void);

signals:
    void tournamentChanged(void);

private:
    GameManagement(void);
    ~GameManagement(void) = default;
    GameManagement(const GameManagement&) = delete; // Copy-Konstruktor
    GameManagement& operator=(const GameManagement&) = delete; // Kopierzuweisungsoperator
    bool _isTournamentStarted;
    int _sportTypeId = 1;
    int _gameModeId = 1;
    int _tournamentId = -1;
    QString _tournamentName;
    QString _tournamentDate;
    SqliteConnector* _db;
};


#endif //SPORTVERANSTALTUNG_GAMEMANAGEMENT_H
