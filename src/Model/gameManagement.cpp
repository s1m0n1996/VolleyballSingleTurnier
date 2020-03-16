
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
void GameManagement::loadOtherTournament(const QString& name, const QDate& date)
{
    QString sqlPrepare = R"(
SELECT id, name, date, sport_type_id, game_mode_id
FROM tournament_list
WHERE name = :name
  AND date = :date
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":name", name);
    sqlQuery.bindValue(":date", date);

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQuery);

    if (rawData.isEmpty())
    {
        qWarning() << "no tournament found";
        return;
    }

    _tournamentId = rawData[0][0].toInt();
    _tournamentName = rawData[0][1].toString();
    _tournamentDate = rawData[0][2].toString();
    _sportTypeId = rawData[0][3].toInt();
    _sportTypeId = rawData[0][4].toInt();

    emit tournamentChanged();
}

/*!
 * \brief load a tournament
 *
 * \param[in] id the tournament name
 */
void GameManagement::loadOtherTournament(const int& id)
{
    QString sqlPrepare = R"(
SELECT id, name, date, sport_type_id, game_mode_id
FROM tournament_list
WHERE id = :id
 AND sport_type_id = :sportTypeId
 AND game_mode_id = :gameModeId
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":id", id);
    sqlQuery.bindValue(":sportTypeId", _sportTypeId);
    sqlQuery.bindValue(":gameModeId", _gameModeId);

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQuery);

    if (rawData.isEmpty())
    {
        qWarning() << "no tournament found";
        return;
    }

    _tournamentId = rawData[0][0].toInt();
    _tournamentName = rawData[0][1].toString();
    _tournamentDate = rawData[0][2].toString();
    _sportTypeId = rawData[0][3].toInt();
    _sportTypeId = rawData[0][4].toInt();

    emit tournamentChanged();
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
void GameManagement::createNewTournament(const QString& name, const QDate& date)
{
    QString sqlPrepare = R"(
SELECT max(id + 1)
FROM tournament_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _sportTypeId);
    sqlQuery.bindValue(":gameModeId", _gameModeId);


    const int nextIdFromDatabase = _db->sqlQuery(sqlQuery)[0][0].toInt();

    const int nextTournamentId = nextIdFromDatabase < 1 ? 1 : nextIdFromDatabase;

    sqlPrepare = R"(
INSERT INTO tournament_list (id, sport_type_id, game_mode_id, name, date)
VALUES (:id, :sportTypeId, :gameModeId, :name, :date)
)";

    sqlQuery.clear();
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":id", nextTournamentId);
    sqlQuery.bindValue(":sportTypeId", _sportTypeId);
    sqlQuery.bindValue(":gameModeId", _gameModeId);
    sqlQuery.bindValue(":name", name);
    sqlQuery.bindValue(":date", date);

    _db->sqlQuery(sqlQuery);
    emit tournamentChanged();
}

/*!
 * \brief load the last tournament
 *
 * this method load the last tournament from the database.
 */
void GameManagement::loadLastTournament(void)
{
    QString sqlPrepare = R"(
SELECT id, name, date
FROM tournament_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
ORDER BY id DESC
LIMIT 1
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _sportTypeId);
    sqlQuery.bindValue(":gameModeId", _gameModeId);

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQuery);
    if (rawData.isEmpty())
    {
        qWarning() << "no tournament found";
        _tournamentId = -1;
        return;
    }
    _tournamentId = rawData[0][0].toInt();
    _tournamentName = rawData[0][1].toString();
    _tournamentDate = rawData[0][2].toString();

    emit tournamentChanged();
}

QList<QList<QString>> GameManagement::getSavedTournaments(void)
{
    QString sqlPrepare = R"(
SELECT name, date, id
FROM tournament_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _sportTypeId);
    sqlQuery.bindValue(":gameModeId", _gameModeId);

    QList<QList<QVariant>> rawData = _db->sqlQuery(sqlQuery);

    if (rawData.isEmpty())
    {
        return QList<QList<QString>>();
    }

    QList<QList<QString>> savedTournaments;

    for (QList<QVariant>& tournament : rawData)
    {
        savedTournaments.append(QList<QString>(
                {tournament[0].toString(), tournament[1].toString(), tournament[2].toString()}));
    }
    return savedTournaments;
}

bool GameManagement::isTournamentStarted() {
    QString sqlPrepare = R"(
SELECT count(*)
FROM game_board_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _sportTypeId);
    sqlQuery.bindValue(":gameModeId", _gameModeId);
    sqlQuery.bindValue(":tournamentId", _tournamentId);

    const int nGames = _db->sqlQuery(sqlQuery)[0][0].toInt();

    return 0 < nGames;
}
