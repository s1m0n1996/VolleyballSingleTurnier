
#include "create_volleyball_game_board.h"
#include "gameManagement.h"
#include <ctime>
#include <cstdlib>
#include <utility>


CreateVolleyballGameBoard::CreateVolleyballGameBoard(QList<Player>& players)
{
    _gamePlayers = std::move(players);
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();

    initRand();
}

CreateVolleyballGameBoard::CreateVolleyballGameBoard()
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();

    initRand();
    refreshPlayers();
}

int CreateVolleyballGameBoard::getCurrentGameId()
{
    QString sqlPrepare = R"(
SELECT max(game_board_id)
FROM game_player_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());


    QList<QList<QVariant>> raw = _db->sqlQuery(sqlQuery);

    int currentGameId = raw[0][0].toInt();

    if (currentGameId == 0)
    {
        return 1;
    }

    sqlPrepare = R"(
SELECT count(*)
FROM game_player_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
  AND game_board_id = :gameBoardId
)";
    sqlQuery.clear();
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":gameBoardId", currentGameId);


    const int cPlayersForGame = _db->sqlQuery(sqlQuery)[0][0].toInt();

    if (cPlayersForGame >= _nPlayersPerTeam * 2)
    {
        currentGameId += 1;
    }
    return currentGameId;
}

void CreateVolleyballGameBoard::initRand()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    rand();
}

Player CreateVolleyballGameBoard::getRandomPlayer()
{
    // zufallszahl von >= start bis <= end
    const int start = 0;
    const int end = _gamePlayers.length() - 1;
    const int randomNumber = rand() % (end + 1 - start) + start;

    return _gamePlayers[randomNumber];
}

/*!
 * \brief Ist der Spieler noch verfügbar
 *
 * Wenn der Spieler bereits im spiel spielt gebe False zurück da ein spieler nicht 2 mal gleichzeitig
 * pro spiel spielen kann
 */
bool CreateVolleyballGameBoard::isPlayerAvailable(Player& player)
{
    const QString sqlPrepare = R"(
SELECT sum(game_board_id)
FROM game_player_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
  AND player_id = :playerId
  AND game_board_id = :gameBoardId
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":playerId", player.getId());
    sqlQuery.bindValue(":gameBoardId", getCurrentGameId());


    QList<QList<QVariant>> raw = _db->sqlQuery(sqlQuery);

    const int count = raw[0][0].toInt();

    return count == 0;

}

/*!
 * \brief Ist der unterschied der gespielten spieler okay
 *
 * Zähle wie viele spiele jeder spieler gemacht hat und prüfe dass wenn die spieler spielen würde, der Unterschied
 * dann nicht größer als 1 ist.
 * z.B. wenn aktueller spieler 3 spiele gespielt hat und er nochmal spielen würde, würde er 4 spiele haben
 * wenn es aber einen spieler gibt der erst 2 spiele hat würe der unterschied dann 2 spiele und es würde false
 * zurückgegeben werden
 */
bool CreateVolleyballGameBoard::isDifferentPlaysOkWhenPlayerPlays(Player& player)
{
    const int playedGames = player.getPlayedGames() + 1;

    const QString sqlPrepare = R"(
SELECT player_id, count(*) AS count
FROM game_player_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
GROUP BY player_id
ORDER BY count
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    QList<QList<QVariant>> raw = _db->sqlQuery(sqlQuery);
    // wenn liste leer ist ist alles erlaubt
    if (raw.isEmpty())
    {
        return true;
    }

    const int minGames = raw.first()[1].toInt();
    const int maxGames = raw.last()[1].toInt();

    return (playedGames - minGames) <= 1;
}

/*!
 * \brief Ist die Männer Frauen verteilung im aktuellen spiel gleich
 *
 * Sint wenn der aktuelle spieler mit eingerechnet wird gleich viele männer und frauen im spiel?
 * max abweichung ist mit der variable angegeben
 */
bool CreateVolleyballGameBoard::isMaleFemaleAllocationOk(Player& player, int maxDifference)
{
    const QString sqlPrepare = R"(
SELECT gender, count(*)
FROM game_player_list
         INNER JOIN player_list pl ON pl.id = game_player_list.player_id
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
  AND game_board_id = :gameBoardId
GROUP BY gender
)";

    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":gameBoardId", getCurrentGameId());

    QList<QList<QVariant>> raw = _db->sqlQuery(sqlQuery);

    QMap<QString, int> genderAllocation;
    genderAllocation["m"] = 0;
    genderAllocation["w"] = 0;
    for (QList<QVariant> row : raw)
    {
        genderAllocation[row[0].toString()] = row[1].toInt();
    }

    genderAllocation[player.getGender()] += 1;

    QList<QString> genders = genderAllocation.keys();
    if (genders.length() == 2) // male and female
    {
        const int different = abs(genderAllocation[genders[0]] - genderAllocation[genders[1]]);
        return different <= maxDifference;
    }
    else
    {
        return false;
    }
}

/*!
 * \brief Gebe den besten spieler zurück
 *
 * Es werden einige algorythmen durchlaufen und dadurch wird der optimale spieler ausgesucht
 */
Player CreateVolleyballGameBoard::getRandomBestPlayer()
{
    const int maxTry = 1000;

    int nTry = 0;

    while (true)
    {
        // Bedingungen die unbedingt erfüllt seinj müssen
        Player randomPlayer = getRandomPlayer();

        if (!isPlayerAvailable(randomPlayer))
        {
            continue;
        }

        // optionale bedingungen nach wichtigkeit geordnet
        nTry++;

        if (nTry > maxTry)
        {
            return randomPlayer;
        }

        if (!isDifferentPlaysOkWhenPlayerPlays(randomPlayer))
        {
            continue;
        }

        const int maxGenderDifferent = abs(nTry / 200);
        if (!isMaleFemaleAllocationOk(randomPlayer, maxGenderDifferent))
        {
            continue;
        }
        return randomPlayer;
    }
}

/*!
 * \brief Erstelle Spiel
 *
 * Erstelle ein Spiel
 */
void CreateVolleyballGameBoard::createOneGame()
{
    const QList<int> teams = {1, 2};

    for (const int& team : teams)
    {
        for (int i = 0; i < _nPlayersPerTeam; i++)
        {
            // todo game id als parameter übergeben
            Player nextPlayer = getRandomBestPlayer();

            const QString sqlPrepare = R"(
INSERT INTO game_player_list (id, game_board_id, sport_type_id, game_mode_id, tournament_id, player_id, team_id)
VALUES ((SELECT count(id) + 1
         FROM game_player_list
         WHERE game_board_id = :gameBoardId
           AND sport_type_id = :sportTypeId
           AND game_mode_id = :gameModeId
           AND tournament_id = :tournamentId), :gameBoardId, :sportTypeId, :gameModeId, :tournamentId, :playerId,
        :teamId);
)";

            QSqlQuery sqlQuery;
            sqlQuery.prepare(sqlPrepare);
            sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
            sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
            sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
            sqlQuery.bindValue(":gameBoardId", getCurrentGameId());
            sqlQuery.bindValue(":playerId", nextPlayer.getId());
            sqlQuery.bindValue(":teamId", team);

            QList<QList<QVariant>> raw = _db->sqlQuery(sqlQuery);
        }
    }

    emit refreshGameBoard();
}

void CreateVolleyballGameBoard::refreshPlayers()
{
    const QString sqlPrepare = R"(
SELECT player_id
FROM tournament_players_list
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    QList<QList<QVariant>> raw = _db->sqlQuery(sqlQuery);

    _gamePlayers.clear();
    for (QList<QVariant> row : raw)
    {
        _gamePlayers.append(Player(row[0].toInt()));
    }
}

QMap<int, QMap<int, QString>> CreateVolleyballGameBoard::getGames()
{
    const QString sqlPrepare = R"(
SELECT game_board_id, pl.name, team_id
FROM game_player_list
         INNER JOIN player_list pl ON pl.id = game_player_list.player_id
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());

    QList<QList<QVariant>> raw = _db->sqlQuery(sqlQuery);

    QMap<int, QMap<int, QString>> teams;
    for (QList<QVariant> row : raw)
    {
        const int gameId = row[0].toInt();
        const int team = row[2].toInt();
        const QString name = row[1].toString();

        teams[gameId][team] += "\n" + name;
    }
    return teams;
}
