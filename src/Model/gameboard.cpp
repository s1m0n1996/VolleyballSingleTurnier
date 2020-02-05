#include "gameboard.h"

Gameboard::Gameboard(QList <Player> players)
{
    _db = &SqliteConnector::instance();
    _players = players;
}

void Gameboard::prepareGameRandomly()
{
    randomInitialisation();
    int i = 0;

    for(const Player& item : _players)                                       //Kein rangebasiertes For, da ein integer für swap() benötigt wird
    {
        _players.swap(i , rand() % _players.size());
        i++;
    }
}

void Gameboard::createGame()                                        //Noch nicht getestet
{
    QList <Game> games;
    for (int i = 0; i < _players.size()/2 ; i = i+2)
    {
        games.append(Game(_players[i], _players[i+1]));
        QString sqlPrepare = R"(
        insert into game_board_list (id, sport_type_id, game_mode_id, tournament_id, game_board_time, player_a_id, player_b_id, winner_id)
        values ((select id
                 from game_board_list
                 where sport_type_id = 1
                 and game_mode_id = 1
                 and tournament_id = 1
                 order by id desc
                 limit 1) + 1,
                 1, 1, ?, 'Test', ?, ?, 0))";                        //Testtime
        QList<QString> sqlParameters;
        sqlParameters.append(QString::number(_tournamentId));
        sqlParameters.append(QString::number(_players[i].getId()));
        sqlParameters.append(QString::number(_players[i+1].getId()));
        _db->sqlQuery(sqlPrepare, sqlParameters);
    }
}


void Gameboard::addPlayerToDatabank()
{
}



void Gameboard::getNewTournamentId()
{
    QString sqlPrepare = R"(SELECT MAX(tournament_id)
                            FROM game_board_list)";
    QList<QVariant> tournament = _db->sqlQuery(sqlPrepare)[0];
    _tournamentId = tournament[0].toInt()+1;                        //Neue TunierId wird erzeugt      //Keine Ahnung ob das funktioniert

}



void Gameboard::randomInitialisation()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    rand();
}


