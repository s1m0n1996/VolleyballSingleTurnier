#include "tournamentwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>
#include <QPainter>
#include <QGridLayout>
#include <QLabel>
#include <QRectF>

#include "View/windowlabel.h"
#include "View/windowbutton.h"

TournamentWindow::TournamentWindow(QWidget *parent) : QWidget(parent)
{
    createWidges();

    QGraphicsScene* game = new QGraphicsScene();
//    game->addRect(50, 0, 150.0, 80.0);
//    game->addRect(50, 40,150.0, 40.0,QPen(),QBrush(Qt::red));

//    game->addRect(50,100,150.0,80.0);
//    game->addRect(50,140,150.0,40.0,QPen(),QBrush(Qt::blue));

//    game->addRect(50,250,150.0,80.0);
//    game->addRect(50,290,150.0,40.0,QPen(),QBrush(Qt::yellow));

//    game->addRect(50,350,150.0,80.0);
//    game->addRect(50,390,150.0,40.0,QPen(),QBrush(Qt::green));

//    game->addRect(250,300,150.0,80.0);
//    game->addRect(250,340,150.0,40.0,QPen(),QBrush(Qt::black));

//    game->addRect(250,50,150.0,80.0);
//    game->addRect(250,90,150.0,40.0,QPen(),QBrush(Qt::darkRed));

////    game->addLine(250,60,200,100);
//    game->addLine(200,290,225,290);//yellow mitte
//    game->addLine(225,290,225,320);
//    game->addLine(225,320,250,320);

//    game->addLine(200,390,225,390);//green
//    game->addLine(225,390,225,360);
//    game->addLine(225,360,250,360);

//    game->addLine(200,40,225,40);// red
//    game->addLine(225,40,225,80);
//    game->addLine(225,80,250,80);

//    game->addLine(200,140,225,140);//blue
//    game->addLine(225,140,225,110);
//    game->addLine(225,110,250,110);




    QRectF rectangle = QRectF(0, 0, 100, 50);

    game->addRect(rectangle);
    game->addText("hi");





//    game->addRect(QRectF(100,100,500,200));

//    double beginX = 50.0;
//    double beginYFirstRec =  0.0;
//    double beginYSecondRec = beginYFirstRec + 40.0;
//    double width = 150.0;
//    double high = 40.0;
//    double endX = beginX + width;
//    double endFirstLineX = endX + 25.0;
//    double endLastLineX = endFirstLineX + 25.0;
//    double distancePair = 60.0;

//    game->addRect(beginX, beginYFirstRec, width, high * 2);
//    game->addRect(beginX, beginYSecondRec,width, high,QPen(),QBrush(Qt::red));



//    game->addRect(beginX,beginYSecondRec + distancePair,width,high);
//    game->addRect(beginX,140,width,high,QPen(),QBrush(Qt::blue));

//    game->addRect(beginX,250,width,high * 2);
//    game->addRect(beginX,290,width,high,QPen(),QBrush(Qt::yellow));

//    game->addRect(beginX,350,width,high *2);
//    game->addRect(beginX,390,width,high,QPen(),QBrush(Qt::green));

//    game->addRect(beginX + 200.0,300,width,high * 2);
//    game->addRect(beginX + 200.0,340,width,high,QPen(),QBrush(Qt::black));

//    game->addRect(beginX + 200.0,50,width,high * 2);
//    game->addRect(beginX + 200.0,90,width,high,QPen(),QBrush(Qt::darkRed));

//    game->addLine(endX,290,endFirstLineX,290);//yellow mitte
//    game->addLine(endFirstLineX,290,endFirstLineX,320);
//    game->addLine(endFirstLineX,320,endLastLineX,320);

//    game->addLine(endX,390,endFirstLineX,390);//green
//    game->addLine(endFirstLineX,390,endFirstLineX,360);
//    game->addLine(endFirstLineX,360,endLastLineX,360);

//    game->addLine(endX,40,endFirstLineX,40);// red
//    game->addLine(endFirstLineX,40,endFirstLineX,80);
//    game->addLine(endFirstLineX,80,endLastLineX,80);

//    game->addLine(endX,140,endFirstLineX,140);//blue
//    game->addLine(endFirstLineX,140,endFirstLineX,110);
//    game->addLine(endFirstLineX,110,endLastLineX,110);

    QGraphicsView* viewgame = new QGraphicsView(game);
    viewgame->setAlignment(Qt::AlignLeft);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(_color);
    layout->addWidget(_title);
    layout->addWidget(viewgame);
    layout->addWidget(_startGame);

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
