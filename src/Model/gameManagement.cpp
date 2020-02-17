
#include <QString>
#include <QList>

#include "gameManagement.h"

GameManagement::GameManagement(void)
{
    _db = &SqliteConnector::instance();

    loadLastTournament();
}

/*!
 * \brief load a tournament
 *
 * \param[in] name the tournament name
 * \param[in] date the tournament date in format: "YYYY-MM-dd"
 */
void GameManagement::loadOtherTournament(QString& name, QString& date)
{
    QString sqlPrepare = R"(
SELECT id, name, date, sport_type_id, game_mode_id
FROM tournament_list
WHERE name = ?
  AND date = ?
)";
    QList<QString> sqlParameters;
    sqlParameters.append(name);
    sqlParameters.append(date);

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlPrepare, sqlParameters);

    if (rawData.length() == 0)
    {
        qWarning() << "data returned nothing";
        return;
    }

    _tournamentId = rawData[0][0].toInt();
    _tournamentName = rawData[0][1].toString();
    _tournamentDate = rawData[0][2].toString();
    _sportTypeId = rawData[0][3].toInt();
    _sportTypeId = rawData[0][4].toInt();

}

/*!
 * \brief load a tournament
 *
 * \param[in] id the tournament name
 */
void GameManagement::loadOtherTournament(int& id)
{
    QString sqlPrepare = R"(
SELECT id, name, date, sport_type_id, game_mode_id
FROM tournament_list
WHERE id = ?
 AND sport_type_id = ?
 AND game_mode_id = ?;
)";
    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(id));
    sqlParameters.append(QString::number(_sportTypeId));
    sqlParameters.append(QString::number(_gameModeId));

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlPrepare, sqlParameters);

    if (rawData.length() == 0)
    {
        qWarning() << "data returned nothing";
        return;
    }

    _tournamentId = rawData[0][0].toInt();
    _tournamentName = rawData[0][1].toString();
    _tournamentDate = rawData[0][2].toString();
    _sportTypeId = rawData[0][3].toInt();
    _sportTypeId = rawData[0][4].toInt();

}

/*!
 * \brief Create a new tournament in the database
 *
 * \param[in] name the tournament name
 * \param[in] date the tournament date in format: "YYYY-MM-dd"
 *
 * this method creates a new tournament in the database.
 * Attention this method only create a new tournament if you wil use the new tournament you must load it!
 */
void GameManagement::createNewTournament(QString& name, QString& date)
{
    QString sqlPrepare = R"(
INSERT INTO tournament_list (id, sport_type_id, game_mode_id, name, date)
VALUES ((SELECT max(id + 1) FROM tournament_list WHERE sport_type_id = ? AND game_mode_id = ?), ?, ?, ?, ?)
)";
    QList<QVariant> sqlParameters;
    sqlParameters.append(QVariant(_sportTypeId));
    sqlParameters.append(QVariant(_gameModeId));
    sqlParameters.append(QVariant(_sportTypeId));
    sqlParameters.append(QVariant(_gameModeId));
    sqlParameters.append(QVariant(name));
    sqlParameters.append(QVariant(date));
    _db->sqlQuery(sqlPrepare, sqlParameters);
}

/*!
 * \brief load the last tournament
 *
 * this method load the last tournament from the database.
 */
void GameManagement::loadLastTournament(void)
{
    QString sqlPrepare = R"(
SELECT max(id), name, date
FROM tournament_list
WHERE sport_type_id = ?
  AND game_mode_id = ?)";

    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(_sportTypeId));
    sqlParameters.append(QString::number(_gameModeId));

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlPrepare, sqlParameters);

    if (rawData.length() <= 0)
    {
        qWarning() << "data returned nothing";
        return;
    }

    _tournamentId = rawData[0][0].toInt();
    _tournamentName = rawData[0][1].toString();
    _tournamentDate = rawData[0][2].toString();
}
