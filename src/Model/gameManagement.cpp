#include <QList>
#include <QString>


#include "Model/gameManagement.h"

GameManagement::GameManagement(void)
{
    _db = &SqliteConnector::instance();

    loadLastTournament();
}

/*!
 * \brief Turnier Laden
 *
 * \param[in] name Turnier Name
 * \param[in] Datum im Format: "YYYY-MM-dd"
 *
 * Es wird das angegebene Turnier aus der Datenbank geladen und und die Membervariablen geschrieben.
 * Wenn es das angegebene Turnier nicht gibt, werden die membervariablen nicht verändert
 * und bleiben auf dem alten Wert.
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
 * \brief Turnier Laden
 *
 * \param[in] Die Turnier ID
 *
 * Es wird das angegebene Turnier aus der Datenbank geladen und und die Membervariablen geschrieben.
 * Wenn es das angegebene Turnier nicht gibt, werden die membervariablen nicht verändert
 * und bleiben auf dem alten Wert.
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
 * \brief Neues Turnier Erstellen
 *
 * \param[in] Turniername
 * \param[in] Turnierdatum im format: "YYYY-MM-dd"
 *
 * Es wird ein Turnier mit den eingegebenen parametern in der Datenbank erstellt.
 * Eine Turnier Id wird automatisch erstellt.
 * Wenn es das Turnier bereits gibt stürzt das Programm ab.
 *
 * Hinweis: Es wird nur ein Turnier erstellt und nicht geladen. Wenn das neu erstellte Turnier auch geladen werden soll,
 * muss zusätzlich (<GameManagement>"::")n<loadOtherTournament>"("<id>")" aufgerufen werden.
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
}

/*!
 * \brief Letztes Turnier laden
 *
 * Es wird das letzte tuenier geladen.
 * Das letzte Turnier ist das mit der höchsten Id.
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

/*!
 * \brief Lade alle Gespeicherten Turniere
 *
 * \return alle gespeicherten Turniere in einer zweidimensionalen Liste
 *
 * \example
 * (
 *      ("Turniername 1", "Datum", Turnier Id),
 *      ("Turniername 2", "Datum", Turnier Id),
 * )
 *
 */
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

/*!
 * \brief Ist das Turnier gestartet
 *
 * \return ist das Turnier gestartet = true sonst false
 *
 * Gibt zurück ob das Turnier gestartet ist. Das Turnier wird als gestartet erkannt, sobalt spieler
 * in der game_board_list Tabelle eingetragen sind. Dies ist der Fall, sobalt in der Meldestelle auf
 * Turnier erstellen geklickt wird.
 */
bool GameManagement::isTournamentStarted()
{
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

/*!
 * \brief Ist das Turnier beendet
 *
 * \return ist das Turnier bendet = true sonst false
 *
 * Gibt zurück ob das Turnier beendet ist. Das Turnier wird als beendet erkannt, sobalt kein Feld
 * in der game_board_list Tabelle NULL ist.
 */
bool GameManagement::isTournamentFinished()
{
    QString sqlPrepare = R"(
SELECT count(*)
FROM game_board_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
  AND winner_id is NULL
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _sportTypeId);
    sqlQuery.bindValue(":gameModeId", _gameModeId);
    sqlQuery.bindValue(":tournamentId", _tournamentId);

    const int notPlayedGames = _db->sqlQuery(sqlQuery)[0][0].toInt();

    return 0 == notPlayedGames && isTournamentStarted();
}

/*!
 * \brief Gebe Gewinner vom Turnier aus
 *
 * \return das Gewinner vom Turnier
 *
 * Wenn es keinen Gewinner gibt, stürzt das Programm ab.
 */
Player GameManagement::getTournamentWinner()
{
    QString sqlPrepare = R"(
SELECT winner_id
FROM game_board_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
ORDER BY id DESC
LIMIT 1
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _sportTypeId);
    sqlQuery.bindValue(":gameModeId", _gameModeId);
    sqlQuery.bindValue(":tournamentId", _tournamentId);

    Player wonPlayer = Player(_db->sqlQuery(sqlQuery)[0][0].toInt());

    return wonPlayer;
}
