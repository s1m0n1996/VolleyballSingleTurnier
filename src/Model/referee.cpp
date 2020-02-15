#include "Model/referee.h"


Referee::Referee()
{

}

void Referee::nextPlayer()
{
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
        _throwCounter = 3;
        qDebug()<<"win" <<_throwCounter <<_winningLegCounter;
        emit valueChanged();
        emit playerWinsLeg();
    }
    else if ((_remainScore[_player] == 0 and _valueMultiplikator != 2)
             or (_remainScore[_player] < 0 and _throwCounter <= 3))
    {
        // es soll sich ein Popup öffnen indem man auswählen kann ob man UNDO auswählen möchte oder
        //nächster Spieler wegen Bust dort muss dann der letzte Wurf zurück gesetzt werden.
        _throwCounter = 3;
        emit valueChanged();
        emit playerBust();
        qDebug()<<"bust" <<_throwCounter;
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



