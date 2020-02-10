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
}

void Referee::singleThrowScore(int valueMultiplikator, int scoreWithoutMultiplikator)
{
    if (3 <= _throwCounter)
    {
        _singleThrowScore = scoreWithoutMultiplikator * valueMultiplikator;
        _allThrows[_throwCounter] = _singleThrowScore;
        _throwCounter++;
    }

    //emit valueChanged();
}

QList<int> Referee::getThrows()
{
    return _allThrows;
}

void Referee::setRemainScore()
{
    _remainScore[_player] = _remainScore[_player] - _singleThrowScore;
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
    //emit valueChanged();
}

int Referee::getRemainScore()
{
    return _remainScore[_player];
}

//Game Referee::getAktivePlayer()
//{
//
//}



