#include "tournamentwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>
#include <QGridLayout>

TournamentWindow::TournamentWindow(QWidget *parent) : QWidget(parent)
{

    QGraphicsScene* game = new QGraphicsScene();
    game->addRect(50, 0, 150.0, 80.0);
    game->addRect(50, 40, 150.0, 40.0,QPen(),QBrush(Qt::red));

    game->addRect(50,100,150.0,80.0);
    game->addRect(50,140,150.0,40.0,QPen(),QBrush(Qt::blue));

    game->addRect(50,250,150.0,80.0);
    game->addRect(50,290,150.0,40.0,QPen(),QBrush(Qt::yellow));

    game->addRect(50,350,150.0,80.0);
    game->addRect(50,390,150.0,40.0,QPen(),QBrush(Qt::green));

    game->addRect(250,350,150.0,80.0);
    game->addRect(250,390,150.0,40.0,QPen(),QBrush(Qt::black));

    game->addRect(250,60,150.0,80.0);
    game->addRect(250,100,150.0,40.0,QPen(),QBrush(Qt::darkRed));

    game->addLine(250,60,200,100);


    QGraphicsView* viewgame = new QGraphicsView(game);
    viewgame->setAlignment(Qt::AlignLeft);

    QGridLayout* layout = new QGridLayout;

        layout->addWidget(viewgame);
        setLayout(layout);
}
