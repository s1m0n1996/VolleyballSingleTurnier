#include <QAbstractScrollArea>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QLineF>
#include <QList>
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QtMath>

#include "Model/playerManagement.h"
#include "Model/referee.h"

#include "View/tournamentWindow.h"
#include <View/windowButton.h>
#include <View/windowLabel.h>


TournamentWindow::TournamentWindow(Referee* referee, PlayerManagement* playerManagement, QWidget* parent) :
        QWidget(parent),
        _referee(referee),
        _playerManagement(playerManagement)
{
    _gameBoard = new QGraphicsScene();

    createWidgets();
    createRects();
    createLines();
    createTexts();


    for (int i = 0; i < _numberOfColumn; i++)
    {
        int rectCount = _rects[i].size();
        int lineCount = _lines[i].size();
        for (int k = 0; k < rectCount; k++)
        {
            _gameBoard->addRect(_rects[i][k]);

        }
        for (int j = 0; j < lineCount; j++)
        {
            _gameBoard->addLine(_lines[i][j]);
        }
    }

    QGraphicsView* viewGame = new QGraphicsView(_gameBoard);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(_title);
    layout->addWidget(viewGame);
    setLayout(layout);

    connect(_referee, SIGNAL(gameListChanged()), this, SLOT(createTexts()));
}


void TournamentWindow::createWidgets(void)
{
    setWindowTitle("Spielplan");
    setWindowIcon(QIcon(":/img/gameplan.png"));

    _title = new WindowLabel("Spielplan");
    _title->setMainTitleStyle();

}

/*!
 * \brief Es werden alle Rectecke erstellt welche im Spielplan angezeigt werden sollen.
 *
 * \return void
 *
 * Diese Methode erstellt anhand der Personen welche an einem Turnier teilnehmen die passende
 * Anzahl an Rechtecken.
 *
 */
void TournamentWindow::createRects(void)
{
    double numberOfPlayer = _playerManagement->countSelectedPlayersForNewGame();
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
    else if (_numberOfColumn > 2)
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
                _beginYFirstRec = _beginYFirstRec + 40.0;
            }
            else
            {
                _beginYFirstRec = _beginYFirstRec + 140.0;
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
        _rects[0].append(QRectF(_beginXLeft, _beginYFirstRec + 40.0, _width, _hight));
    }


    int counter2 = 0;
    _beginXLeft = _beginXLeft + _xSpacing + _width;
    _beginXRight = _beginXRight - _xSpacing - _width;
    _beginYFirstRec = _rects[0][0].y() + _hight + 10.0;
    _beginYSecondRec = 3 * _hight + 100.0;

    // Die übrigen Spalten mit Rechtecken füllen
    for (int i = 1; i < _numberOfColumn; i++)
    {
        int numberOfRect = _rects[i - 1].count() / 4;

        for (int k = 0; k < numberOfRect; k++)
        {
            if (counter2 < 2)
            {
                _beginYFirstRec = _beginYFirstRec + 40.0;
            }
            else
            {
                _beginYFirstRec = _beginYFirstRec + _beginYSecondRec + 100.0;
                counter2 = 0;
            }


            _rects[i].append(QRectF(_beginXLeft, _beginYFirstRec, _width, _hight));
            _rects[i].append(QRectF(_beginXRight, _beginYFirstRec, _width, _hight));

            counter2++;

        }
        _beginXLeft = _beginXLeft + _xSpacing + _width;
        _beginXRight = _beginXRight - _xSpacing - _width;
        _beginYSecondRec = (_rects[0][0].y() + _hight + 10.0) * ((qPow(2, i + 2) - 2)) + 40.0;
        _beginYFirstRec =
                _rects[0][0].y() + (3 + (i - 1) * (4 + (2 * (i - 2)))) * _hight + 10.0 + 100.0 * (qPow(2, i) - 1);
        counter2 = 0;
    }

    // FinalSpiel hinzufügen
    if (numberOfPlayer > 2)
    {
        double beginFinalGame = _rects[_numberOfColumn - 1][0].x() +
                                ((_rects[_numberOfColumn - 1][1].x() - _rects[_numberOfColumn - 1][0].x()) / 2.0);
        _rects[_numberOfColumn - 1].append(QRectF(beginFinalGame, _rects[_numberOfColumn - 1][0].y(), _width, _hight));
        _rects[_numberOfColumn - 1].append(QRectF(beginFinalGame, _rects[_numberOfColumn - 1][2].y(), _width, _hight));
    }
}

/*!
 * \brief Es werden alle Lininen (Verbindungslininen zwischen den Rechtecken)
 *  erstellt welche im Spielplan angezeigt werden sollen.
 *
 * \return void
 *
 * Diese Methode erstellt anhand der Anzahl von Rechtecken welche in der Methode(void TournamentWindow::createRects())
 * berechnet wurde die passende Anzahl an Verbindungslininen ziwschen den einezelnen Rechtecken.
 *
 */
void TournamentWindow::createLines(void)
{
    for (double i = 0.0; i < _numberOfColumn; i++)
    {
        _lines.append(QList<QLineF>());
    }

    // Alle Splaten werden durchgegangen damit alle Rechtecke verbunden werden können
    for (int i = 0; i < _numberOfColumn - 1; i++)
    {
        double numberOfRects = _rects[i].size();

        double lengthLineY1 = 0.0;

        // Linien von links die nach unten gehen
        for (int k = 2; k < numberOfRects; k += 8)
        {
            if (k < numberOfRects)
            {
                double startLineX1Left = _rects[i][k].x() + _width;
                double startLineY1 = _rects[i][k].y();
                double lengthLineX1 = 50.0;
                lengthLineY1 = _rects[i + 1][2].y() - _rects[i][2].y() - _hight / 2;

                _lines[i].append(QLineF(startLineX1Left, startLineY1, startLineX1Left + lengthLineX1, startLineY1));
                _lines[i].append(QLineF(startLineX1Left + lengthLineX1, startLineY1, startLineX1Left + lengthLineX1,
                                        startLineY1 + lengthLineY1));
                _lines[i].append(QLineF(startLineX1Left + lengthLineX1, startLineY1 + lengthLineY1,
                                        startLineX1Left + lengthLineX1 + 50.0, startLineY1 + lengthLineY1));
            }
        }

        // Linien von links die nach oben gehen
        for (int k = 6; k < numberOfRects; k += 8)
        {
            if (k < numberOfRects)
            {
                double startLineX1 = _rects[i][k].x() + _width;
                double startLineY1 = _rects[i][k].y();
                double lengthLineX1 = 50.0;
                lengthLineY1 = _rects[i + 1][2].y() - _rects[i][2].y() - _hight / 2;

                _lines[i].append(QLineF(startLineX1, startLineY1, startLineX1 + lengthLineX1, startLineY1));
                _lines[i].append(QLineF(startLineX1 + lengthLineX1, startLineY1, startLineX1 + lengthLineX1,
                                        startLineY1 - lengthLineY1));
                _lines[i].append(QLineF(startLineX1 + lengthLineX1, startLineY1 - lengthLineY1,
                                        startLineX1 + lengthLineX1 + 50.0, startLineY1 - lengthLineY1));
            }
        }

        // Linien von rechts die nach unten gehen
        for (int k = 3; k < numberOfRects; k += 8)
        {
            if (k < numberOfRects)
            {
                double startLineX1Right = _rects[i][k].x();
                double startLineY1 = _rects[i][k].y();
                double lengthLineX1 = 50.0;
                lengthLineY1 = _rects[i + 1][2].y() - _rects[i][2].y() - _hight / 2;

                _lines[i].append(QLineF(startLineX1Right, startLineY1, startLineX1Right - lengthLineX1, startLineY1));
                _lines[i].append(QLineF(startLineX1Right - lengthLineX1, startLineY1, startLineX1Right - lengthLineX1,
                                        startLineY1 + lengthLineY1));
                _lines[i].append(QLineF(startLineX1Right - lengthLineX1, startLineY1 + lengthLineY1,
                                        startLineX1Right - lengthLineX1 - 50.0, startLineY1 + lengthLineY1));
            }
        }

        // Linien von rechts die nach oben gehen
        for (int k = 7; k < numberOfRects; k += 8)
        {
            if (k < numberOfRects)
            {
                double startLineX1Right = _rects[i][k].x();
                double startLineY1 = _rects[i][k].y();
                double lengthLineX1 = 50.0;
                lengthLineY1 = _rects[i + 1][2].y() - _rects[i][2].y() - _hight / 2;

                _lines[i].append(QLineF(startLineX1Right, startLineY1, startLineX1Right - lengthLineX1, startLineY1));
                _lines[i].append(QLineF(startLineX1Right - lengthLineX1, startLineY1, startLineX1Right - lengthLineX1,
                                        startLineY1 - lengthLineY1));
                _lines[i].append(QLineF(startLineX1Right - lengthLineX1, startLineY1 - lengthLineY1,
                                        startLineX1Right - lengthLineX1 - 50.0, startLineY1 - lengthLineY1));
            }
        }
    }

    // FinalSpiel Linien
    if (_rects[_numberOfColumn - 1].size() > 2)
    {
        // links
        _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x(),
                                _rects[_numberOfColumn - 1].last().y() + _hight / 2,
                                _rects[_numberOfColumn - 1].last().x() - 50.0,
                                _rects[_numberOfColumn - 1].last().y() + _hight / 2));

        _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x() - 50.0,
                                _rects[_numberOfColumn - 1].last().y() + _hight / 2,
                                _rects[_numberOfColumn - 1].last().x() - 50.0,
                                _rects[_numberOfColumn - 1].last().y()));

        _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x() - 50.0,
                                _rects[_numberOfColumn - 1].last().y(),
                                _rects[_numberOfColumn - 1].last().x() - 50.0 - _width,
                                _rects[_numberOfColumn - 1].last().y()));
        // rechts
        _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x() + _width,
                                _rects[_numberOfColumn - 1].last().y() - _hight / 2,
                                _rects[_numberOfColumn - 1].last().x() + _width + 50.0,
                                _rects[_numberOfColumn - 1].last().y() - _hight / 2));

        _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x() + _width + 50.0,
                                _rects[_numberOfColumn - 1].last().y() - _hight / 2,
                                _rects[_numberOfColumn - 1].last().x() + _width + 50.0,
                                _rects[_numberOfColumn - 1].last().y()));

        _lines[0].append(QLineF(_rects[_numberOfColumn - 1].last().x() + _width + 50.0,
                                _rects[_numberOfColumn - 1].last().y(),
                                _rects[_numberOfColumn - 1].last().x() + _width + 50.0 + _width,
                                _rects[_numberOfColumn - 1].last().y()));
    }
}

/*!
 * \brief Die Namen der einzelnen Spieler werden in die Rechtecke geschrieben.
 *
 * \return void
 *
 * Diese Methode schreibt die Namen in richtiger Reihenfolge (nach Spielen) in die Rechtecke.
 * Das endgültige Layout für das Fenster wird hier festgelegt.
 *
 */
void TournamentWindow::createTexts(void)
{
    Game game;
    QList<QRectF> allRects;
    QList<QString> allPlayersForAllGames;

    winner = game.getAllWinnersInTournament();

    allPlayersForAllGames = game.getAllPlayersForGameboardView();
    _texts = allPlayersForAllGames;

    for (int i = 0; i < _numberOfColumn; i++)
    {
        int rectCount = _rects[i].size();
        for (int k = 0; k < rectCount; k++)
        {
            allRects.append(_rects[i][k]);
        }
    }


    int allRectCount = allRects.size();

    for (int i = 0; i < allRectCount; i++)
    {
        QGraphicsTextItem* allPlayernames = new QGraphicsTextItem(allPlayersForAllGames[i]);
        allPlayernames->setPos(allRects[i].x(), allRects[i].y());
        allPlayernames->setFont(QFont("Arial Nova Light", 10));
        _gameBoard->addItem(allPlayernames);
    }


    createColours();
}

/*!
 * \brief Die Rechtecke werden farbig markiert, sobald ein Gewinner aus der Partie feststeht.
 *
 * \return void
 *
 * Diese Methode färbt den Rahmen der Rechtecke grün, wenn dieser Spieler das Spiel gewonnen hat.
 *
 */
void TournamentWindow::createColours(void)
{
    QList<QList<QString>> test;

    for (int i = 0; i < _numberOfColumn; i++)
    {
        test.append(QList<QString>());
    }

    int indicatorWhereToStartInTextList = 0;

    for (int k = 0; k < _numberOfColumn; k++)
    {
        if (k == 1)
        {
            indicatorWhereToStartInTextList = _rects[k - 1].size();
        }
        else if (k > 1)
        {
            indicatorWhereToStartInTextList = indicatorWhereToStartInTextList + _rects[k - 1].size();
        }

        for (int m = 0; m < _rects[k].size(); m++)
        {
            test[k].append(_texts[m + indicatorWhereToStartInTextList]);
        }
    }

    for (int m = 0; m < _numberOfColumn; m++)
    {
        for (int i = 0; i < _rects[m].size(); i++)
        {
            for (int t = 0; t < winner.size(); t++)
            {
                if (winner[t] == test[m][i] and winner[t] != "")
                {
                    _gameBoard->addRect(_rects[m][i], QPen(Qt::green, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                    winner.removeAt(t);
                    break;
                }
            }
        }
    }
}
