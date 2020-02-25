#include "tournamentwindow.h"
#include <QPen>
#include <QPainter>
#include <QGridLayout>
#include <qdebug.h>
#include <QRectF>
#include <QLineF>
#include <QList>
#include <QtMath>
#include <QIcon>
#include <QLabel>
#include <View/windowbutton.h>
#include <View/windowlabel.h>

TournamentWindow::TournamentWindow(QWidget *parent) : QWidget(parent)
{
    createWidges();
    createRects();
    createLines();


    for(int i = 0; i < _numberOfColumn; i++)
    {
        for (QRectF leftSideTree : _rects[i])
        {
            _gameBoard->addRect(leftSideTree);
        }
    }

    for(int i = 0; i < _numberOfColumn; i++)
    {
        for (QLineF lines : _lines[i])
        {
            _gameBoard->addLine(lines);
        }
    }


    QGraphicsView* viewgameLeft = new QGraphicsView(_gameBoard);
    viewgameLeft->setAlignment(Qt::AlignLeft);
    // viewgameLeft->scale(1.5,1.5);

    QGridLayout* layout = new QGridLayout;

    layout->addWidget(viewgameLeft);
    setLayout(layout);
}

void TournamentWindow::createWidges()
{
    setWindowTitle("Spielplan");
    setWindowIcon(QIcon(":/img/gameplan.png"));

    _color       = new QLabel;
    _color->setStyleSheet("background-color:#550000;");
    _title = new WindowLabel("Spielplan");
    _title->setTitleStyel();

    _startGame = new WindowButton("Spiel starten");

}

void TournamentWindow::createRects()
{
    // TODO: muss noch eine ANzhal an Spielrn bekommen
    double numberOfPlayer = 32;
    double numberOfPlayerIntern = numberOfPlayer;

    // ANzahl der Spalten berechnen
    for (int i = 0; i < numberOfPlayer; i++)
    {
        if (numberOfPlayerIntern > 1)
        {
            numberOfPlayerIntern = numberOfPlayerIntern / 2;
            _numberOfColumn++;
        }
    }

    // Wenn Splaten gleich 2 Sind dann soll nur ein Spiel erstellt werden.
    if (_numberOfColumn == 2)
    {
        _numberOfColumn = 1;
    }
    else if(_numberOfColumn > 2)
    {
        _numberOfColumn = _numberOfColumn - 1;
    }

    _beginXRight = _beginXLeft + (_numberOfColumn * 2 + 1) * _width + (_numberOfColumn * 2) * _xSpacing;

    int counter = 0;

    // Liste nach Anzahl der Splaten erstellen
    for (int i = 0; i < _numberOfColumn; i++)
    {
        _rects.append(QList<QRectF>());
    }

    // Die erste Spalte mit Rechtecken füllen
    if (numberOfPlayer > 2)
    {
        for (int j = 0; j < numberOfPlayer / 2; j++)
        {
            if (counter < 2)
            {
                _beginYFirstRec = _beginYFirstRec + 40;
            }
            else
            {
                _beginYFirstRec = _beginYFirstRec + 140;
                counter = 0;
            }

            _rects[0].append(QRectF(_beginXLeft, _beginYFirstRec, _width, _hight));
            _rects[0].append(QRectF(_beginXRight, _beginYFirstRec, _width, _hight));

            counter++;
        }
    }
    else
    {
        _rects[0].append(QRectF(_beginXLeft, _beginYFirstRec, _width, _hight));
        _rects[0].append(QRectF(_beginXLeft, _beginYFirstRec + 40, _width, _hight));
    }


    int counter2 = 0;
    _beginXLeft = _beginXLeft + _xSpacing + _width;
    _beginXRight = _beginXRight - _xSpacing - _width;
    _beginYFirstRec = _rects[0][0].y() + _hight + 10;
    _beginYSecondRec = 3 * _hight + 100;

    // Die übrigen Spalten mit Rechtecken füllen
    for(int i = 1; i < _numberOfColumn; i++)
    {
        int numberOfRect = _rects[i -1].count() / 4;

        for(int k = 0; k < numberOfRect; k++)
        {
            if (counter2 < 2)
            {
                _beginYFirstRec = _beginYFirstRec + 40;
            }
            else
            {
                _beginYFirstRec = _beginYFirstRec + _beginYSecondRec + 100;
                counter2 = 0;
            }


            _rects[i].append(QRectF(_beginXLeft, _beginYFirstRec, _width, _hight));
            _rects[i].append(QRectF(_beginXRight, _beginYFirstRec, _width, _hight));

            counter2++;

        }
        _beginXLeft = _beginXLeft + _xSpacing + _width;
        _beginXRight = _beginXRight - _xSpacing - _width;
        _beginYSecondRec = (_rects[0][0].y() + _hight + 10) * ((qPow(2,i + 2) - 2)) + 40;
        _beginYFirstRec = _rects[0][0].y() + (3 + (i - 1) * (4 + (2 * (i - 2)))) * _hight + 10 + 100 * (qPow(2,i) - 1);
        counter2 = 0;
    }

    // FinalSpiel hinzufügen
    if (numberOfPlayer > 2)
    {
        int beginFinalGame = _rects[_numberOfColumn - 1][0].x() + ((_rects[_numberOfColumn - 1][1].x() - _rects[_numberOfColumn - 1][0].x()) / 2);
        _rects[_numberOfColumn - 1].append(QRectF(beginFinalGame, _rects[_numberOfColumn - 1][0].y(), _width, _hight));
        _rects[_numberOfColumn - 1].append(QRectF(beginFinalGame, _rects[_numberOfColumn - 1][2].y(), _width, _hight));
    }
}

void TournamentWindow::createLines()
{
    for (int i = 0; i < _numberOfColumn; i++)
    {
        _lines.append(QList<QLineF>());
    }

    // Alle Splaten werden durchgegangen damit alle Rechtecke verbunden werden können
    for (int i = 0; i < _numberOfColumn - 1; i++)
    {
        int numberOfRects = _rects[i].size();

        double lengthLineY1 = 0;

        // Linien von links die nach unten gehen
        for (int k = 2; k < numberOfRects; k += 8)
        {
            if (k < numberOfRects)
            {
                double startLineX1Left = _rects[i][k].x() + _width;
                double startLineY1 = _rects[i][k].y();
                double lengthLineX1 = 50;
                lengthLineY1 = _rects[i + 1][2].y() - _rects[i][2].y() - _hight / 2;

                _lines[i].append(QLineF(startLineX1Left, startLineY1, startLineX1Left + lengthLineX1, startLineY1));
                _lines[i].append(QLineF(startLineX1Left + lengthLineX1, startLineY1, startLineX1Left + lengthLineX1, startLineY1 + lengthLineY1));
                _lines[i].append(QLineF(startLineX1Left + lengthLineX1, startLineY1 + lengthLineY1, startLineX1Left + lengthLineX1 + 50.0,startLineY1 + lengthLineY1));
            }
        }

        // Linien von links die nach oben gehen
        for (int k = 6; k < numberOfRects; k += 8)
        {
            if (k < numberOfRects)
            {
                double startLineX1 = _rects[i][k].x() + _width;
                double startLineY1 = _rects[i][k].y();
                double lengthLineX1 = 50;
                lengthLineY1 = _rects[i + 1][2].y() - _rects[i][2].y() - _hight / 2;

                _lines[i].append(QLineF(startLineX1, startLineY1, startLineX1 + lengthLineX1, startLineY1));
                _lines[i].append(QLineF(startLineX1 + lengthLineX1, startLineY1, startLineX1 + lengthLineX1, startLineY1 - lengthLineY1));
                _lines[i].append(QLineF(startLineX1 + lengthLineX1, startLineY1 - lengthLineY1, startLineX1 + lengthLineX1 + 50.0,startLineY1 - lengthLineY1));
            }
        }

        // Linien von rechts die nach unten gehen
        for (int k = 3; k < numberOfRects; k += 8)
        {
            if (k < numberOfRects)
            {
                double startLineX1Right = _rects[i][k].x();
                double startLineY1 = _rects[i][k].y();
                double lengthLineX1 = 50;
                lengthLineY1 = _rects[i + 1][2].y() - _rects[i][2].y() - _hight / 2;

                _lines[i].append(QLineF(startLineX1Right, startLineY1, startLineX1Right - lengthLineX1, startLineY1));
                _lines[i].append(QLineF(startLineX1Right - lengthLineX1, startLineY1, startLineX1Right - lengthLineX1, startLineY1 + lengthLineY1));
                _lines[i].append(QLineF(startLineX1Right - lengthLineX1, startLineY1 + lengthLineY1, startLineX1Right - lengthLineX1 - 50.0,startLineY1 + lengthLineY1));
            }
        }

        // Linien von rechts die nach oben gehen
        for (int k = 7; k < numberOfRects; k += 8)
        {
            if (k < numberOfRects)
            {
                double startLineX1Right = _rects[i][k].x();
                double startLineY1 = _rects[i][k].y();
                double lengthLineX1 = 50;
                lengthLineY1 = _rects[i + 1][2].y() - _rects[i][2].y() - _hight / 2;

                _lines[i].append(QLineF(startLineX1Right, startLineY1, startLineX1Right - lengthLineX1, startLineY1));
                _lines[i].append(QLineF(startLineX1Right - lengthLineX1, startLineY1, startLineX1Right - lengthLineX1, startLineY1 - lengthLineY1));
                _lines[i].append(QLineF(startLineX1Right - lengthLineX1, startLineY1 - lengthLineY1, startLineX1Right - lengthLineX1 - 50.0,startLineY1 - lengthLineY1));
            }
        }
    }

    // FinalSpiel Linien
    // links
    _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x(),
                     _rects[_numberOfColumn - 1].last().y() - _hight / 2,
            _rects[_numberOfColumn - 1].last().x() - 50 ,
            _rects[_numberOfColumn - 1].last().y() - _hight / 2));

    _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x() - 50,
                     _rects[_numberOfColumn - 1].last().y() - _hight / 2,
            _rects[_numberOfColumn - 1].last().x() - 50,
            _rects[_numberOfColumn - 1].last().y()));

    _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x() - 50,
                     _rects[_numberOfColumn - 1].last().y(),
            _rects[_numberOfColumn - 1].last().x() - 50 - 125,
            _rects[_numberOfColumn - 1].last().y()));
    // rechts
    _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x() + _width,
                     _rects[_numberOfColumn - 1].last().y() + _hight / 2,
            _rects[_numberOfColumn - 1].last().x() + _width + 50 ,
            _rects[_numberOfColumn - 1].last().y() + _hight / 2));

    _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x() + _width + 50,
                     _rects[_numberOfColumn - 1].last().y() + _hight / 2,
            _rects[_numberOfColumn - 1].last().x() + _width + 50,
            _rects[_numberOfColumn - 1].last().y()));

    _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x() + _width + 50,
                     _rects[_numberOfColumn - 1].last().y(),
            _rects[_numberOfColumn - 1].last().x() + _width + 50 + 125,
            _rects[_numberOfColumn - 1].last().y()));

}
