
#include "Model/sqliteConnector.h"

#ifndef SPORTVERANSTALTUNG_GAMEMANAGEMENT_H
#define SPORTVERANSTALTUNG_GAMEMANAGEMENT_H

/*!
 * \file gameManagement.h
 * \brief this class handles the global parameter for the game
 * \author Simon Flörke
 *
 * Thia class is a singleton class and would be used as global parameters that all models works with
 * the same tournament and game.
*/
class gameManagement
{
public:
    static gameManagement& instance(void)
    {
        static gameManagement instance;
        return instance;
    }
    int getTournamentId(void){ return _tournamentId;}
    int getSportTypeId(void){ return  _sportTypeId;}
    int getGameModeId(void){ return _gameModeId;}
    QString getTournamentName(void) {return _tournamentName;}
    QString getTournamentDate(void) {return _tournamentDate;}

    void createNewTournament(QString& name, QString& date);
    void loadOtherTournament(QString& name, QString& date);
    void loadOtherTournament(int& id);
    void loadLastTournament(void);

private:
    gameManagement(void);
    ~gameManagement(void) = default;
    gameManagement(const gameManagement&) = delete; // Copy-Konstruktor
    gameManagement& operator=(const gameManagement&) = delete; // Kopierzuweisungsoperator
    int _sportTypeId = 1;
    int _gameModeId = 1;
    int _tournamentId;
    QString _tournamentName;
    QString _tournamentDate;
    SqliteConnector* _db;
};


#endif //SPORTVERANSTALTUNG_GAMEMANAGEMENT_H
