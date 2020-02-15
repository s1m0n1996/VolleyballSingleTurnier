/*!
* \file referee.cpp
* \brief Diese Klasse repräsentiert den Richter der das Dartspiel leitet
* \author Nico Meyer
*/

#include "Model/referee.h"


Referee::Referee()
{

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

    if (0 == _player)
    {
        _player = 1;
    }
    else
    {
        _player = 0;
    }

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
             or (_remainScore[_player] < 0 and _throwCounter <= 3))
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
    if (3 == _throwCounter)
    {
        _remainScore[_player] = _remainScore[_player] + _allThrows[2];
        _allThrows[2] = 0;
        _throwCounter--;
    }
    else if (2 == _throwCounter)
    {
        _remainScore[_player] = _remainScore[_player] + _allThrows[1];
        _allThrows[1] = 0;
        _throwCounter--;;
    }
    else if (1 == _throwCounter)
    {
        _remainScore[_player] = _remainScore[_player] + _allThrows[0];
        _allThrows[0] = 0;
        _throwCounter--;
    }

    if (_wasLastThrowInLegToWin)
    {
        _winningLegCounter[_player]--;
        _wasLastThrowInLegToWin = false;
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

//Game Referee::getAktivePlayer()
//{
//
//}



