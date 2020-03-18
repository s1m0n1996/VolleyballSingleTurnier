/*!
* \file referee.cpp
* \brief Diese Klasse repräsentiert den Richter der das Dartspiel leitet
* \author Nico Meyer
*/

#include "Model/referee.h"
#include <QDateTime>


Referee::Referee()
{
    _db = &SqliteConnector::instance();
    _gameManagement = &GameManagement::instance();
    updatePlayer();
    loadLastGame();
    connect(_gameManagement, SIGNAL(tournamentChanged()), this, SLOT(updatePlayer()));
}


/*!
 * \brief Lädt die Spieler für das nächste Spiel
 *
 * \return void
 *
 * Bekommt von der Klasse "Game" die Spieler Id´s vom nächsten Spiel und lädt diese
 * in die Membervariable
 *
 */
void Referee::updatePlayer()
{

    if (_gameManagement->isTournamentStarted() and !_gameManagement->isTournamentFinished())
    {
        _allPlayers.clear();
        Game game;
        _gameId = game.getGameId();
        _allPlayers.append(game.getPlayerAId());
        _allPlayers.append(game.getPlayerBId());
        resetAllStats();
        emit valueChanged();
    }
    else
    {
        emit tournamentFinished();
    }
}

/*!
 * \brief Setzt alle Spielwerte auf den Anfangswert
 *
 * \return void
 *
 *Setzt alle Spielwerte auf den Anfangswert
 *
 */
void Referee::resetAllStats()
{
    //writeLegIntoDatabase();                   Wo musst du hin??
    _winningLegCounter  = {0,0};
    _throwCounter = 0;
    _allThrows = {0,0,0};
    _allThrowsWithoutMulti = {0,0,0};
    _remainScore = {501,501};
    _wasLastThrowInLegToWin = false;            //TODO: Muss geguckt werden ob die rein müssen oder nicht
    _wasLastThrowInLegToBust = false;
}

/*!
 * \brief Setzt den nächsten Spieler während eines Legs.
 *
 * \return void
 *
 * Diese Metohde setzt den nächsten aktiven Spieler im Leg
 * und gibt dieses als Signal an das View weiter
 *
 */
void Referee::nextPlayer()
{
    if (_wasLastThrowInLegToBust)
    {
        for(int i = 0; i < 4; i++)
        {
           undoThrow();
        }
        _wasLastThrowInLegToBust = false;
    }
    // switch player id
    _player = !_player;

    _throwCounter = 0;
    _allThrows[0] = 0;
    _allThrows[1] = 0;
    _allThrows[2] = 0;

    scoreIsUnder170InLeg();
    emit valueChanged();
}

/*!
 * \brief Setzt den nächsten aktiven Spieler nach einem gewonnen Leg.
 *
 * \return void
 *
 * Diese Metohde setzt den nächsten aktiven Spieler nach einem gewonnen Leg
 * und gibt dieses als Signal an das View weiter
 *
 */
void Referee::nextPlayerAfterWinningLeg()
{
    if (_winningLegCounter[0] < 3 and _winningLegCounter[1] < 3)
    {
        int allWonLegsInMatch = _winningLegCounter[0] + _winningLegCounter[1];
        if ((allWonLegsInMatch % 2) == 0)
        {
            _player = 0;
        }
        else
        {
            _player = 1;
        }

        _remainScore[0] = 501;
        _remainScore[1] = 501;
        _throwCounter = 0;
        _allThrows[0] = 0;
        _allThrows[1] = 0;
        _allThrows[2] = 0;
        _wasLastThrowInLegToWin = false;
        _wasLastThrowInLegToBust = false;
        emit valueChanged();
    }
    else
    {
        _remainScore[0] = 501;
        _remainScore[1] = 501;
        _throwCounter = 0;
        _allThrows[0] = 0;
        _allThrows[1] = 0;
        _allThrows[2] = 0;
        _wasLastThrowInLegToWin = false;
        _wasLastThrowInLegToBust = false;
        emit valueChanged();
        setWinner();
    }
}


/*!
 * \brief Gibt die Player Id des aktiven Spielers wieder
 *
 * \return int
 *
 * Diese Metohde gibt die Id des Spielers der aktuell am Zug ist zurück
 *
 */
int Referee::getAktivePlayer()
{
   return _allPlayers[_player];
}


/*!
 * \brief Berechnet den Wert des geworfenen Dartpfeils.
 * \param[int,int] Den Multiplikator des Wurf [int], die Punktzahl des Wurf's ohne Multiplikator [int]
 * \return void
 *
 * Diese Metohde berechent den Wert des geworfenen Dartpfeils
 * indem dort der Wert des Wurfes mit dem Multiplikator multipliziert wird
 *
 */
void Referee::singleThrowScore(int valueMultiplikator, int scoreWithoutMultiplikator)
{
    if (_throwCounter < 3)
    {
        _throwScoreWithoutMulti = scoreWithoutMultiplikator;
        _singleThrowScore = scoreWithoutMultiplikator * valueMultiplikator;
        _allThrows[_throwCounter] = _singleThrowScore;
        _allThrowsWithoutMulti[_throwCounter] = _throwScoreWithoutMulti;
        _valueMultiplikator = valueMultiplikator;
        _throwCounter++;

        QString sqlPrepare = R"(
                             INSERT INTO leg_history_list (id, sport_type_id, game_mode_id, tournament_id, game_board_id, leg_id, player_id, time, value_type_id, value)
                             VALUES (:throwId, :sportTypeId, :gameModeId, :tournamentId, :gameBoardId, :legId, :playerId, :time, :valueTypeId, :value)
                             )";
        QSqlQuery sqlQuery;
        sqlQuery.prepare(sqlPrepare);
        sqlQuery.bindValue(":throwId", (getLastLegIdInSameGame()+1));
        sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
        sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
        sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
        sqlQuery.bindValue(":gameBoardId", _gameId);
        sqlQuery.bindValue(":legId", getNumberOfCurrentLeg());
        sqlQuery.bindValue(":playerId", getAktivePlayer());
        sqlQuery.bindValue(":time", QDateTime::currentDateTime());
        sqlQuery.bindValue(":valueTypeId", valueMultiplikator);

        if(valueMultiplikator == 0)
        {
            sqlQuery.bindValue(":value", 0);
        }
        else
        {
            sqlQuery.bindValue(":value", scoreWithoutMultiplikator);
        }
        _db->sqlQuery(sqlQuery);

        setRemainScore();
        emit valueChanged();
    }
}


QList<int> Referee::getThrows()
{
    return _allThrows;
}

void Referee::setRemainScore()
{
    _remainScore[_player] = _remainScore[_player] - _singleThrowScore;
}


/*!
 * \brief Legt den Gewinner des Spieles fest
 *
 * \return void
 *
 * Sobald drei Legs gewonnen wurden legt diese Methode den Gewinner fest und
 * übergibt ihn an das Spiel
 *
 */
void Referee::setWinner()
{
    if (_winningLegCounter[_player] == 3)
    {
        _winner = _player;
        Game game;
        game.setNextWinner(getAktivePlayer());
        game.loadNextGame();
        updatePlayer();
        emit gameListChanged();
        _winningLegCounter[0] = 0;
        _winningLegCounter[1] = 0;
    }
}

/*!
 * \brief Es wird geprüft ob das Leg gewonwen wurde.
 *
 * \return void
 *
 * Es wird geprüft ob das Leg auf korrekten Weg gewonnen wurde, das heißt: Der letzte Wurf war ein Doppel
 * und der Restscore ist bei Null.
 * Wenn es nicht auf korrekten Weg beendet wurde.
 * In beiden Fällen wird ein Signal an das View rausgegebn.
 *
 */
void Referee::legWinningCondition()
{
    if (_remainScore[_player] == 0 and _valueMultiplikator == 2
        and _throwCounter <= 3 and _winningLegCounter[_player] < 3)
    {
        _winningLegCounter[_player]++;
        _wasLastThrowInLegToWin = true;
        _wasLastThrowInLegToBust = false;
        emit valueChanged();
        emit playerWinsLeg();
       // setWinner();
    }
    else if ((_remainScore[_player] == 0 and _valueMultiplikator != 2)
             or (_remainScore[_player] < 0 and _throwCounter <= 3)
             or _remainScore[_player] == 1)
    {
        // es soll sich ein Popup öffnen indem man auswählen kann ob man UNDO auswählen möchte oder
        //nächster Spieler wegen Bust dort muss dann der letzte Wurf zurück gesetzt werden.
        _wasLastThrowInLegToBust = true;
        _wasLastThrowInLegToWin = false;
        emit valueChanged();
        emit playerBust();
    }
}


/*!
 * \brief Gibt ein Singal raus sobald die Restpunktzahl unter 170 Punkte ist.
 *
 * \return void
 *
 * Diese Methode prüft ob bei einem Spieler die Restpunktzahl unter 170 ist, wenn dieses
 * der Fall ist, dann wird ein Singal rausgegeben.
 *
 */
void Referee::scoreIsUnder170InLeg()
{
    if ((_remainScore[_player] <= 170 and _remainScore[_player] >= 0) and _remainingThrows > 0)

    {
        qDebug() << "Emit Score uner 170";
        emit scoreIsUnder170();
    }
}


/*!
 * \brief Gibt ein Singal raus wenn keine Würfe in einem Leg für einen Spieler verfügbar sind.
 *
 * \return void
 *
 * Dieses Methode sendet ein Signal sobald ein der aktive Spieler während er dran ist keinen
 * Wurf mehr zurverfügung hat.
 *
 */
void Referee::remainingThrowsAreZeroInLeg()
{
    if (_remainingThrows == 0)
    {
        emit remainingThrowsAreZero();
    }
}


int Referee::valueMultiplikator()
{
    return _valueMultiplikator;
}


QList<int> Referee::allThrowsWithoutMultiplikator()
{
    return _allThrowsWithoutMulti;
}

/*!
 * \brief Setzt den zu letzt geworfenen Wurf zurück.
 *
 * \return void
 *
 * Diese Metohde setzt den zuletzt geworfenen Pfeil zurück und gibt ein Signal an das View heraus.
 *
 */
void Referee::undoThrow()
{
    if (_wasLastThrowInLegToWin)
    {
        _winningLegCounter[_player]--;
        _wasLastThrowInLegToWin = false;
    }

    QString sqlPrepare = R"(DELETE
                         from leg_history_list
                         WHERE sport_type_id = :sportTypeId
                           AND id = (SELECT MAX (id)
                         FROM leg_history_list
                         WHERE sport_type_id = :sportTypeId
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                           AND game_board_id = :gameBoardId
                           AND leg_id = :legId)
                           AND game_mode_id = :gameModeId
                           AND tournament_id = :tournamentId
                           AND game_board_id = :gameBoardId
                           AND leg_id = :legId
                         )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":gameBoardId", _gameId);
    sqlQuery.bindValue(":legId", getNumberOfCurrentLeg());

    _db->sqlQuery(sqlQuery);

    if (0 < _throwCounter)
    {

        // last throw is actual throw - 1
        _remainScore[_player] = _remainScore[_player] + _allThrows[_throwCounter - 1];
        _allThrows[_throwCounter - 1] = 0;
         _allThrowsWithoutMulti[_throwCounter - 1] = 0;
        _throwCounter--;
    }

    _wasLastThrowInLegToBust = false;

    scoreIsUnder170InLeg();
    emit valueChanged();
}


int Referee::getRemainScore()
{
    return _remainScore[_player];
}


int Referee::getCountOfWinningLegs()
{
    return _winningLegCounter[_player];
}

int Referee::getGameStart()
{
    return _player;
}

int Referee::getThrowScore()
{
    return _throwScore = _allThrows[0] + _allThrows[1] +_allThrows[2];
}

int Referee::getRemainingThrows()
{
    return _remainingThrows = 3 - _throwCounter;
}


QList<int> Referee::getRemainScoreForViewer()
{
    return  _remainScore;
}

QList<int> Referee::getAllPlayersForViewer()
{
    return  _allPlayers;
}


/*!
 * \brief Gibt die letzte Leg Id des gleichen Spiels wieder
 *
 * \return int
 *
 * Diese Methode ist vorbereitend für die loadLastGame() Methode und liefert
 * die letzte Leg Id im gleichen Spiel
 *
 */
int Referee::getLastLegIdInSameGame()
{
    QString sqlPrepare = R"(SELECT id
                         FROM leg_history_list
                         WHERE sport_type_id = :sportTypeId
                         AND game_mode_id = :gameModeId
                         AND tournament_id = :tournamentId
                         AND game_board_id = :gameBoardId
                         AND leg_id = :legId
                         ORDER by id desc
                         limit 1)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);

    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":gameBoardId", _gameId);
    sqlQuery.bindValue(":legId", getNumberOfCurrentLeg());

    QList<QList<QVariant>> id = _db->sqlQuery(sqlQuery);
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


/*!
 * \brief Gibt die aktuelle Anzahl an Legs wieder
 *
 * \return int
 *
 */
int Referee::getNumberOfCurrentLeg()
{
    int legId = 1;
    for (int i = 0; i < 2; i++)
    {
        legId += _winningLegCounter[i];
    }
    return legId;
}


/*!
 * \brief Setzt den aktiven Spieler auf die übergeben Spieler Id
 *
 * \param[in] die zu setztene Spieler Id
 * \return void
 *
 *
 */
void Referee::setActivePlayer(int activePlayerId)
{
    if(activePlayerId == _allPlayers[1])
    {
        _player = 1;
    }
    else
    {
        _player = 0;
    }
}


/*!
 * \brief Lädt das zuletzt unbeendete Spiel in einem geladenen Turnier
 *
 * \return void
 *
 *
 */
void Referee::loadLastGame(){
    QString sqlPrepare = R"(
                         SELECT punkte, player_id, game_board_id, tournament_id
                         FROM (SELECT SUM(value * value_type_id) as punkte, player_id, game_board_id, tournament_id
                               FROM leg_history_list
                               group by tournament_id = :tournamentId, game_board_id = :gameId, leg_id, player_id)
                         WHERE game_board_id = :gameId
                           AND tournament_id = :tournamentId;
                         )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":gameId", _gameId);   //_gameId
    QList<QList<QVariant>> legPoints = _db->sqlQuery(sqlQuery);

    if(legPoints.isEmpty())                                         //gibt es keine zuvor gemachten Würfe wird rausgesprungen
    {
        return;
    }


    int straightLegPointsSize = legPoints.size()%2;                 //Testen ob es eine gerade Anzahl an Punktergebnissen gibt

    for (int i=0 ; i < (legPoints.size()-straightLegPointsSize) ; i+=2)
    {
        if(legPoints[i][0]==501)
        {
            if(legPoints[i][1] == _allPlayers[0])
            {
                _winningLegCounter[0]+=1;

            }
            else
            {
                _winningLegCounter[1]+=1;
            }
        }
        if(legPoints[i+1][0]==501)
        {
            if(legPoints[i+1][1] == _allPlayers[0])
            {
                 _winningLegCounter[0]+=1;

            }
            else
            {
                 _winningLegCounter[1]+=1;
            }
        }
        if(legPoints[i][0]!=501 && legPoints[i+1][0]!=501)              //TODO: WARUM
        {
            loadLastThrows();
            if(_allPlayers[0]<_allPlayers[1])
            {
                _remainScore[0] = 501 - legPoints[i][0].toInt();
                _remainScore[1] = 501 - legPoints[i+1][0].toInt();
            }
            else
            {
                _remainScore[1] = 501 - legPoints[i][0].toInt();
                _remainScore[0] = 501 - legPoints[i+1][0].toInt();
            }
        }
    }

    if(straightLegPointsSize == 1)                                   //Bei ungerader Listenlänge
    {
        if(legPoints[legPoints.size()-1][1] == _allPlayers[0])
        {
            _remainScore[0] = 501 - legPoints[legPoints.size()-1][0].toInt();
        }
        else
        {
            _remainScore[1] = 501 - legPoints[legPoints.size()-1][0].toInt();
        }
        loadLastThrows();
    }
    return;
}


/*!
 * \brief Lädt die zuletzt geworfenen Würfe eines unbeendeten Spiels in dem geladenen Turnier
 *
 * \return void
 *
 */
void Referee::loadLastThrows()
{
    QString sqlPrepare = R"(
                         SELECT * FROM leg_history_list
                         WHERE tournament_id = :tournamentId
                           AND game_board_id = :gameId
                           AND leg_id = (SELECT MAX(leg_id)
                                         FROM leg_history_list
                                         WHERE tournament_id = :tournamentId
                                           AND game_board_id = :gameId)
                         )";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.bindValue(":gameId", _gameId);
    QList<QList<QVariant>> lastThrows = _db->sqlQuery(sqlQuery);

    int legIdPerfectToThree = (lastThrows.size()/3)*3;
    int throws = 0;

    for (int i = legIdPerfectToThree; i < lastThrows.size(); i++)           //Alle die größer als Bsp.: 13 /3*3 = 12 sind, throwcounter wird rauf gesetzt
    {                                                                       //außerdem wird der aktuelle Spieler ermittelt
        setActivePlayer(lastThrows[i][6].toInt());
        _allThrows[throws] =  (lastThrows[i][9].toInt() * lastThrows[i][8].toInt());
        _throwCounter++;
        throws++;
    }

    if((lastThrows.size()/3) == 0)
    {
        int lastPlayer = lastThrows[(lastThrows.size()-1)][6].toInt();
        if(lastPlayer == _allPlayers[0])
        {
            setActivePlayer(_allPlayers[1]);
        }
    }
    return;
}
