#include "Model/gameboard.h"

/*!
 * \brief Mischt die Spielerliste zufällig
 *
 * \return void
 *
 */
Gameboard::Gameboard(QList<Player> players)
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();
    _players = players;
    prepareGameRandomly();
    createFirstGames();
    createRemainingGames();
}

/*!
 * \brief Mischt die Spielerliste zufällig
 *
 * \return void
 *
 */
void Gameboard::prepareGameRandomly(void)
{
    randomInitialisation();
    int i = 0;
    for (const Player& item : _players)
    {
        Q_UNUSED(item);
        const int k = rand() % _players.size();
        _players.swapItemsAt(i, k);
        i++;
    }
}

/*!
 * \brief Erzeugt die ersten Spiele des Turniers
 *
 * \return void
 *
 * Schreibt die ersten Spiele in die Datenbank
 *
 */
void Gameboard::createFirstGames(void)
{
    for (int i = 1; i < _players.size(); i = i + 2)
    {
        QString sqlPrepare = R"(
        INSERT INTO game_board_list (id, sport_type_id, game_mode_id, tournament_id, game_board_time, player_a_id, player_b_id)
        VALUES (:gameboardId, :sportTypeId, :gameModeId, :tournamentId, :gameTime, :playerAid, :playerBid))";                        //Testtime
        QSqlQuery sqlQuery;
        sqlQuery.prepare(sqlPrepare);
        sqlQuery.bindValue(":gameboardId", getLastGameIdInSameTournament() + 1);
        sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
        sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
        sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
        sqlQuery.bindValue(":gameTime", "Testtime");
        sqlQuery.bindValue(":playerAid", _players[i - 1].getId());
        sqlQuery.bindValue(":playerBid", _players[i].getId());
        _db->sqlQuery(sqlQuery);
    }
}

/*!
 * \brief Erzeugt die Spiele des Turniers nach der ersten Runde
 *
 * \return void
 *
 * Schreibt die restlichen Spiele in die Datenbank, die nach der ersten Runde
 * erzeugt werden. Das heißt die Spiele, bei denen noch keine Spieler feststehen.
 *
 */
void Gameboard::createRemainingGames(void)
{
    int numberOfGames = 0;
    for (int i = _players.size(); i > 1; i = i / 2)
    {
        numberOfGames += i / 2;
    }
    numberOfGames -= _players.size() / 2;

    for (int k = 0; k < numberOfGames; k++)
    {
        QString sqlPrepare = R"(
        INSERT INTO game_board_list (id, sport_type_id, game_mode_id, tournament_id, game_board_time)
        VALUES (:gameboardId, :sportTypeId, :gameModeId, :tournamentId, :gameTime))";
        QSqlQuery sqlQuery;
        sqlQuery.prepare(sqlPrepare);
        sqlQuery.bindValue(":gameboardId", getLastGameIdInSameTournament() + 1);
        sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
        sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
        sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
        sqlQuery.bindValue(":gameTime", "Testtime");
        _db->sqlQuery(sqlQuery);
    }
}

/*!
 * \brief Gibt die Game Id aus einem Turnier wieder
 *
 * \return void
 *
 * Gibt die letzte erstellte Game Id eines Turniers wieder. Wenn es
 * noch kein erstelltes Spiel gibt, gibt die Methode 0 wieder.
 *
 */
int Gameboard::getLastGameIdInSameTournament(void)
{
    QString sqlPrepare = R"(SELECT id
                         FROM game_board_list
                         WHERE sport_type_id = :sportTypeId
                         AND game_mode_id = :gameModeId
                         AND tournament_id = :tournamentId
                         ORDER by id desc
                         limit 1)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    QList<QList<QVariant>> id = _db->sqlQuery(sqlQuery);
    int lastId;
    if (id.isEmpty())
    {
        lastId = 0;
    }
    else
    {
        lastId = id[0][0].toInt();
    }
    return lastId;
}

void Gameboard::randomInitialisation(void)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    rand();
}
