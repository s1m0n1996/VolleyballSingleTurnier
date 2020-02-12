#include "gameboard.h"

Gameboard::Gameboard(QList <Player> players, QString tournamentName)
{
    _db = &SqliteConnector::instance();
    _players = players;
    createNewTournament(tournamentName);
    prepareGameRandomly();
    createFirstGames();
    createRemainingGames();
}


void Gameboard::createNewTournament(QString tournamentName)
{
    getNewTournamentId();
    QString sqlPrepare = R"(
            INSERT INTO tournament_list (id, sport_type_id, game_mode_id, name, date)
            VALUES(?, 1, 1, ?, ?))";
    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(_tournamentId));
    sqlParameters.append(tournamentName);
    QDate date;
    sqlParameters.append(date.currentDate().toString("yyyy-MM-dd"));
    _db->sqlQuery(sqlPrepare, sqlParameters);
}


void Gameboard::prepareGameRandomly()
{
    randomInitialisation();
    int i = 0;
    for(const Player& item : _players)
    {
        int k = rand()%_players.size();
        _players.swap(i,k);
        i++;
    }
}


void Gameboard::createFirstGames()
{
    for (int i = 1; i < _players.size() ; i=i+2)
    {
        QString sqlPrepare = R"(
        INSERT INTO game_board_list (id, sport_type_id, game_mode_id, tournament_id, game_board_time, player_a_id, player_b_id)
        VALUES (?, 1, 1, ?, 'Test', ?, ?))";                        //Testtime
        QList<QString> sqlParameters;
        sqlParameters.append(QString::number(getLastGameIdInSameTournament()+1));
        sqlParameters.append(QString::number(_tournamentId));
        sqlParameters.append(QString::number(_players[i-1].getId()));
        sqlParameters.append(QString::number(_players[i].getId()));
        _db->sqlQuery(sqlPrepare, sqlParameters);
    }
}


void Gameboard::createRemainingGames()
{
    int numberOfGames = 0;
    for (int i=_players.size(); i > 1; i=i/2)
    {
        numberOfGames+= i/2;
    }
    numberOfGames -= _players.size()/2;

    for (int k = 0; k<numberOfGames; k++)
    {
        QString sqlPrepare = R"(
        INSERT INTO game_board_list (id, sport_type_id, game_mode_id, tournament_id, game_board_time)
        VALUES (?, 1, 1, ?, 'Test'))";                        //Testtime
        QList<QString> sqlParameters;
        sqlParameters.append(QString::number(getLastGameIdInSameTournament()+1));
        sqlParameters.append(QString::number(_tournamentId));
        _db->sqlQuery(sqlPrepare, sqlParameters);
    }
}


void Gameboard::getNewTournamentId()
{
    QString sqlPrepare = R"(SELECT MAX(tournament_id)
                            FROM game_board_list)";
    QList<QVariant> tournament = _db->sqlQuery(sqlPrepare)[0];
    _tournamentId = tournament[0].toInt()+1;                        //Neue TunierId wird erzeugt
}


int Gameboard::getLastGameIdInSameTournament()
{
    QString sqlPrepare = R"(SELECT id
                         FROM game_board_list
                         WHERE sport_type_id = 1
                         AND game_mode_id = 1
                         AND tournament_id = ?
                         ORDER by id desc
                         limit 1)";
    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(_tournamentId));
    QList<QList<QVariant>> id = _db->sqlQuery(sqlPrepare, sqlParameters);
    int lastId;
    if(id.isEmpty())
    {
        lastId = 0;
    }
    else
    {
        lastId = id[0][0].toInt();
    }

    return lastId;
}


void Gameboard::randomInitialisation()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    rand();
}
