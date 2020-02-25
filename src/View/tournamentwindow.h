#ifndef TOURNEMENTWINDOW_H
#define TOURNEMENTWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
class WindowLabel;
class WindowButton;
class QLabel;

class TournamentWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TournamentWindow(QWidget *parent = nullptr);

public slots:

private:
    // TODO: Es muss von der Meldestelle übergeben werden wie viele Leute an dem Tunier teilnehmen

    double _beginXLeft = 50.0;
    double _beginXRight = 0.0;

    double _width = 150.0;
    double _hight = 40.0;

    double _beginYFirstRec =  0.0;
    double _beginYSecondRec = 0.0;

    double _xSpacing = 100.0;
    double _ySpacing = 50.0;

    QList<QList<QRectF>> _rects;
    QList<QList<QLineF>> _lines;

    double _numberOfColumn = 0;

    void createWidges(void);
    QLabel* _color = nullptr;
    WindowLabel* _title= nullptr;
    WindowButton* _startGame= nullptr;
    QGraphicsScene* _gameBoard = new QGraphicsScene();


     void createRects(void);
     void createLines(void);

};

#endif // TOURNEMENTWINDOW_H
