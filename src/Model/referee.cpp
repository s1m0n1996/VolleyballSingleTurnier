#include "Model/referee.h"


Referee::Referee()
{

}

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



