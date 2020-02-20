/*!
* \file referee.cpp
* \brief Diese Klasse repräsentiert den Richter der das Dartspiel leitet
* \author Nico Meyer
*/

#include "Model/referee.h"


Referee::Referee()
{
    _db = &SqliteConnector::instance();
    _playerAId = 0;
    _playerBId = 1;
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
        undoThrow();
        _wasLastThrowInLegToBust = false;
    }

    // switch player id
    _player = !_player;

    _throwCounter = 0;
    _allThrows[0] = 0;
    _allThrows[1] = 0;
    _allThrows[2] = 0;

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
    int allWonLegsInMatch = _winningLegCounter[0] + _winningLegCounter[1];
    qDebug() << allWonLegsInMatch << "Alle Legs gewonnen";
    if ((allWonLegsInMatch % 2) == 0)
    {
        _player = 0;
        qDebug() << _player << "Spieler 0";
    }
    else
    {
        _player = 1;
        qDebug() << _player << "Spieler 1";
    }

    _remainScore[0] = 501;
    _remainScore[1] = 501;
    _throwCounter = 0;
    _allThrows[0] = 0;
    _allThrows[1] = 0;
    _allThrows[2] = 0;

     emit valueChanged();
}

int Referee::getAktivePlayer()
{
    return _player;
}

/*QString Referee::getAktivePlayer()
{
    Player aktivePlayer(getAktivePlayerId());
    return aktivePlayer.getName();
}*/

/*!
 * \brief Berechnet den Wert des geworfenen Dartpfeils.
 * \param[in,in] Den Multiplikator des Wurf [int], die Punktzahl des Wurf's ohne Multiplikator [int]
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
        _singleThrowScore = scoreWithoutMultiplikator * valueMultiplikator;
        _allThrows[_throwCounter] = _singleThrowScore;
        qDebug()<<_singleThrowScore;
        _valueMultiplikator = valueMultiplikator;
        _throwCounter++;

        QString sqlPrepare = R"(
                             INSERT INTO leg_history_list (id, sport_type_id, game_mode_id, tournament_id, game_board_id, leg_id, player_id, time, value_type_id, value)
                             VALUES (?, 1, 1, ?, ?, ?, ?, ?, ?, ?)
                             )";
        QList <QString> sqlParameters;
        sqlParameters.append(QString::number(getLastLegIdInSameGame()+1));
        sqlParameters.append(QString::number(_tournamentId));
        sqlParameters.append(QString::number(_gameId));
        sqlParameters.append(QString::number(getNumberOfCurrentLeg()));
        sqlParameters.append(QString::number(getAktivePlayerId()));
        sqlParameters.append("Uhrzeit");
        sqlParameters.append(QString::number(valueMultiplikator));
        if(valueMultiplikator == 0)
        {
            sqlParameters.append(QString::number(0));
        }
        else
        {
            sqlParameters.append(QString::number(scoreWithoutMultiplikator));
        }
        _db->sqlQuery(sqlPrepare, sqlParameters);

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

void Referee::setWinner()
{
    if (_winningLegCounter[_player] == 3)
    {
        _winner = _player;
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
        qDebug()<<"win" <<_winningLegCounter <<_wasLastThrowInLegToWin;
        emit valueChanged();
        emit playerWinsLeg();
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
        qDebug()<<"bust";
    }
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
                         WHERE sport_type_id = 1
                           AND id = (SELECT MAX (id)
                         FROM leg_history_list
                         WHERE sport_type_id = 1
                           AND game_mode_id = 1
                           AND tournament_id = ?
                           AND game_board_id = ?
                           AND leg_id = ?)
                           AND game_mode_id = 1
                           AND tournament_id = ?
                           AND game_board_id = ?
                           AND leg_id = ?
                         )";
    QList <QString> sqlParameters;
    sqlParameters.append(QString::number(_tournamentId));
    sqlParameters.append(QString::number(_gameId));
    sqlParameters.append(QString::number(getNumberOfCurrentLeg()));
    sqlParameters.append(QString::number(_tournamentId));
    sqlParameters.append(QString::number(_gameId));
    sqlParameters.append(QString::number(getNumberOfCurrentLeg()));
    _db->sqlQuery(sqlPrepare, sqlParameters);

    if (0 < _throwCounter)
    {
        // last throw is actual throw - 1
        _remainScore[_player] = _remainScore[_player] + _allThrows[_throwCounter - 1];
        _allThrows[_throwCounter - 1] = 0;
        _throwCounter--;
    }
    emit valueChanged();
}


int Referee::getRemainScore()
{
    return _remainScore[_player];
}

//QString Referee::getWinner()
//{

//}

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


int Referee::getAktivePlayerId()
{
    if(_player == 0)
    {
        return _playerAId;
    }
    else
    {
        return _playerBId;
    }
}




QList<int> Referee::getRemainScoreForViewer()
{
    return  _remainScore;
}

QList<int> Referee::getWinningLegCounterForViewer()
{
    return _winningLegCounter;
}

QList<int> Referee::getAllPlayersForViewer()
{
    return  _allPlayers;
}


int Referee::getLastLegIdInSameGame()
{
    QString sqlPrepare = R"(SELECT id
                         FROM leg_history_list
                         WHERE sport_type_id = 1
                         AND game_mode_id = 1
                         AND tournament_id = ?
                         AND game_board_id = ?
                         AND leg_id = ?
                         ORDER by id desc
                         limit 1)";
    QList<QString> sqlParameters;
    sqlParameters.append(QString::number(_tournamentId));
    sqlParameters.append(QString::number(_gameId));
    sqlParameters.append(QString::number(getNumberOfCurrentLeg()));
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


int Referee::getNumberOfCurrentLeg()
{
    int legId = 1;
    for (int i = 0; i < 2; i++)           //TODO: Testen ob damit alle legs aufgegriffen werden
    {
        legId += _winningLegCounter[i];
    }
    return legId;
}
