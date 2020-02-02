
#ifndef SPORTVERANSTALTUNG_PLAYERMANAGEMENT_H
#define SPORTVERANSTALTUNG_PLAYERMANAGEMENT_H

#include <QString>
#include <QList>
#include <QVariant>

#include "Model/sqliteConnector.h"

/*!
 * \file playermanagement.h
 * \author Simon Flörke
 * \brief A class for the player management
 *
 *
*/
class PlayerManagement {

public:
    PlayerManagement(void);

    QList<QList<QVariant>> getAllStoredPlayers(bool onlyAvailable = true);

private:
    SqliteConnector* _db;
};


#endif //SPORTVERANSTALTUNG_PLAYERMANAGEMENT_H
