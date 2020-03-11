#ifndef TOURNEMENTWINDOW_H
#define TOURNEMENTWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAbstractScrollArea>
#include <QGraphicsWidget>
#include <QGraphicsTextItem>
#include "Model/referee.h"
#include "Model/playermanagement.h"
class WindowLabel;
class WindowButton;
class QLabel;

class TournamentWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TournamentWindow(Referee* referee, PlayerManagement* playerManagement, QWidget  *parent = nullptr);

public slots:
    void createTexts(void);


private:
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

    double _numberOfColumn = 0;

    void createWidgets(void);
    WindowLabel* _title= nullptr;
    WindowButton* _startGame= nullptr;
    QGraphicsScene* _gameBoard = new QGraphicsScene();

    QLabel* _crown = nullptr;

    void createRects(void);
    void createLines(void);

};

#endif // TOURNEMENTWINDOW_H
