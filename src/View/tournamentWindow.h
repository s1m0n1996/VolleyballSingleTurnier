#ifndef TOURNEMENTWINDOW_H
#define TOURNEMENTWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAbstractScrollArea>
#include <QGraphicsWidget>
#include <QGraphicsTextItem>
#include "Model/referee.h"
#include "Model/playerManagement.h"
class WindowLabel;
class WindowButton;
class QLabel;

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
    QGraphicsScene* _gameBoard = new QGraphicsScene();

    QList<QString> winner;

};

#endif // TOURNEMENTWINDOW_H
