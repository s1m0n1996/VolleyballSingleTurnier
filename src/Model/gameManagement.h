
#ifndef SPORTVERANSTALTUNG_GAMEMANAGEMENT_H
#define SPORTVERANSTALTUNG_GAMEMANAGEMENT_H

#include "Model/sqliteConnector.h"

/*!
 * \file gameManagement.h
 * \brief Die Singelton Klasse hat die Globalen parameter des Spiels
 * \author Simon Flörke
 *
 * Es werden alle Globalen parameter gespeichert und es kann darauf zugegriffen werden.
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
    bool isTournamentStarted(void);

    void createNewTournament(const QString& name, const QDate& date);
    void loadOtherTournament(const QString& name, const QDate& date);
    void loadOtherTournament(const int& id);
    void loadLastTournament(void);
    QList<QList<QString>> getSavedTournaments(void);

signals:
    void tournamentChanged(void);

private:
    GameManagement(void);
    ~GameManagement(void) = default;
    GameManagement(const GameManagement&) = delete; // Copy-Konstruktor
    GameManagement& operator=(const GameManagement&) = delete; // Kopierzuweisungsoperator
    int _sportTypeId = 1;
    int _gameModeId = 1;
    int _tournamentId = -1;
    QString _tournamentName;
    QString _tournamentDate;
    SqliteConnector* _db;
};


#endif //SPORTVERANSTALTUNG_GAMEMANAGEMENT_H
