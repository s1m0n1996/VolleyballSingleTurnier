
#include "Model/tournament.h"


Tournament::Tournament(void){
    _db = &SqliteConnector::instance();
}

/*!
 * \brief get all players
 *
 * \param[in]onlyAvailable default: true. get only players who is the available flag true.
 *
 * \return the players in a 2 dim. list
 *
 * This method returned all players that are stored in the database.
 * For Example the player_list table from the database have this entries:
 *
 * id   name    birthday    country     is_available
 * 1	Klaus	2010-12-15	DE	        1
 * 2	Hilde	2000-12-15	DE	        1
 * 3	Peter	2020-12-15	DE	        1
 *
 * the returned data is:
 *  (
 *  	(
 *  		QVariant(QString, "Hilde"),
 *  		QVariant(QString, "2010-12-15"),
 *  		QVariant(QString, "DE")
 *  	),
 *  	(
 *  		QVariant(QString, "Klaus"),
 *  		QVariant(QString, "2000-12-15"),
 *  		QVariant(QString, "DE")
 *  	),
 *  	(
 *  		QVariant(QString, "Peter"),
 *  		QVariant(QString, "2020-12-15"),
 *  		QVariant(QString, "DE")
 *  	)
 *  )
 */
QList<QList<QVariant>> Tournament::getAllStoredPlayers(bool onlyAvailable) {

    QString sqlPrepare = R"(
SELECT name, birthday, country
FROM player_list
WHERE is_available >= ?
ORDER BY name;
    )";

    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(onlyAvailable));

    return _db->sqlQuery(sqlPrepare, sqlParameters);
}

