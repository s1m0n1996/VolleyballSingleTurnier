
#ifndef SPORTVERANSTALTUNG_TOURNAMENT_H
#define SPORTVERANSTALTUNG_TOURNAMENT_H

#include <QString>
#include <QList>
#include <QVariant>

#include "Model/sqliteConnector.h"

/*!
 * \file tournement.h
 * \author Simon Flörke
 * \brief A class for the tournement
 *
 *
*/
class Tournament {

public:
    Tournament(void);

    QList<QList<QVariant>> getAllStoredPlayers(bool onlyAvailable = true);

private:
    SqliteConnector* _db;
};


#endif //SPORTVERANSTALTUNG_TOURNAMENT_H
