#ifndef TOURNEMENTWINDOW_H
#define TOURNEMENTWINDOW_H

#include <QWidget>

class QGraphicsScene;
class PlayerManagement;
class QLabel;
class Referee;
class WindowLabel;
class WindowButton;

/*!
* \file tournamentwindow.h
* \brief Diese Klasse erstellt einen Spielplan
* \author Nico Meyer
*
* Anhand der vorhanden Spieler wird ein Spielplan ertsellt. Sobald ein Spieler gewonnen hat,
* wird der Gewinner in die nächste Partie eingetragen und in vorheriegen Spiel grün markiert.
*
*/
class TournamentWindow : public QWidget
{
    Q_OBJECT
public:
    TournamentWindow(Referee* referee, PlayerManagement* playerManagement, QWidget  *parent = nullptr);

public slots:
    void createTexts(void);

private:    
    void createWidgets(void);
    void createRects(void);
    void createLines(void);
    void createColours(void);

    Referee* _referee;
    PlayerManagement* _playerManagement;

    double _beginXLeft = 50.0;
    double _beginXRight = 0.0;

    double _width = 250.0;
    double _hight = 40.0;

    double _beginYFirstRec =  0.0;
    double _beginYSecondRec = 0.0;

    double _xSpacing = 100.0;
    double _ySpacing = 50.0;

    QList<QList<QRectF>> _rects;
    QList<QList<QLineF>> _lines;

    QList<QString> _texts;

    int _numberOfColumn = 0;

    WindowLabel* _title= nullptr;
    WindowButton* _startGame= nullptr;
    QGraphicsScene* _gameBoard = nullptr;

    QList<QString> _winner;

};

#endif // TOURNEMENTWINDOW_H
