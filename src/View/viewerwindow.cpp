#include "viewerwindow.h"
#include "ui_viewerWindow.h"
#include "Model/statistics.h"
#include "View/dartboardviewer.h"
#include <QGraphicsScene>

ViewerWindow::ViewerWindow(Referee *referee, Viewer* viewer, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerWindow),
    _referee(referee),
    _viewer(viewer)
{
    Viewer* viewers = new Viewer;
    _viewer = viewers;

    setWindowIcon(QIcon(":/img/viewer.png"));

    ui->setupUi(this);

    ui->title->setMainTitleStyle();

    ui->groupBox_3->setStyleSheet("border:none");
    connect(_referee,SIGNAL(valueChanged()),this, SLOT(writeScore()));
    connect(_referee,SIGNAL(scoreIsUnder170()),this, SLOT(scoreIsUnder170InLeg()));
    connect(_referee,SIGNAL(remainingThrowsAreZero()),this, SLOT(remainingThrowsAreZeroInLeg()));
    connect(_viewer,SIGNAL(howToFinishLeg()),this,SLOT(possibleWayToFinishLeg()));
    //wenn nächster Spieler dran
    ui->player1GroupBox->setStyleSheet("border-color: red;");
    ui->player2GroupBox->setStyleSheet("border-color: red;");

    ui->photoPlayer1->setScaledContents(true);
    ui->photoPlayer1->resize(175,175);
    ui->photoPlayer1->setPixmap(QPixmap(":/img/user.png"));

    ui->photoPlayer2->setScaledContents(true);
    ui->photoPlayer2->resize(175,175);
    ui->photoPlayer2->setPixmap(QPixmap(":/img/user.png"));

    test->setPos(0, 0);
    scene->addItem(test);

    ui->graphicsView->setScene(scene);

}

ViewerWindow::~ViewerWindow()
{
    delete ui;
}

void ViewerWindow::writeScore()
{   
    scene->update(0,0,20,10);

    _player1 = _referee->getAllPlayersForViewer()[0];
    _player2 = _referee->getAllPlayersForViewer()[1];

    Player playerA(_player1);
    Player playerB(_player2);

    Statistics statisticsPlayerA;
    Statistics statisticsPlayerB;

    ui->nameOfPlayer1->setText(playerA.getName());
    ui->nameOfPlayer2->setText(playerB.getName());

    _aktivePlayer = _referee->getAktivePlayer();
    Player aktivePlayer(_aktivePlayer);

    ui->aktivePlayer->setText(aktivePlayer.getName());

    if (_aktivePlayer == _player1)
    {
        ui->throw1FromPlayer1->setText(QString::number(_referee->getThrows()[0]));
        ui->throw2FromPlayer1->setText(QString::number(_referee->getThrows()[1]));
        ui->throw3FromPlayer1->setText(QString::number(_referee->getThrows()[2]));
        ui->all3ThrowsPlayer1->setText(QString::number(_referee->getThrowScore()));
        ui->remainingThrowsForPlayer1->setText(QString::number(_referee->getRemainingThrows()));
        ui->countOfWinningLegsPlayer1->setText(QString::number(_referee->getCountOfWinningLegs()));
    }
    else
    {
        ui->throw1FromPlayer2->setText(QString::number(_referee->getThrows()[0]));
        ui->throw2FromPlayer2->setText(QString::number(_referee->getThrows()[1]));
        ui->throw3FromPlayer2->setText(QString::number(_referee->getThrows()[2]));
        ui->all3ThrowsPlayer2->setText(QString::number(_referee->getThrowScore()));
        ui->remainingThrowsForPlayer2->setText(QString::number(_referee->getRemainingThrows()));
        ui->countOfWinningLegsPlayer2->setText(QString::number(_referee->getCountOfWinningLegs()));
    }

    ui->remainScorePlayer1->setText(QString::number(_referee->getRemainScoreForViewer()[1]));
    ui->remainScorePlayer2->setText(QString::number(_referee->getRemainScoreForViewer()[0]));
    ui->averageOfPlayer1->setText(QString::number(statisticsPlayerA.getAverageOfPlayerInCurrentGame(playerA)));
    ui->averageOfPlayer2->setText(QString::number(statisticsPlayerA.getAverageOfPlayerInCurrentGame(playerB)));
}


void ViewerWindow::scoreIsUnder170InLeg()
{
    _viewer->createJsonDocument(_referee->getRemainingThrows(), _referee->getRemainScore());
}



void ViewerWindow::possibleWayToFinishLeg()
{
    qDebug() << "emit";
    _aktivePlayer = _referee->getAktivePlayer();
    qDebug() << _aktivePlayer;

    if (_aktivePlayer == _player1)
    {

        ui->possibleWayToFinishThrow1FromPlayer1->setText(_viewer->getHowToFinishLeg()[0]);
        ui->possibleWayToFinishThrow2FromPlayer1->setText(_viewer->getHowToFinishLeg()[1]);
        ui->possibleWayToFinishThrow3FromPlayer1->setText(_viewer->getHowToFinishLeg()[2]);
    }
    else
    {

        ui->possibleWayToFinishThrow1FromPlayer2->setText(_viewer->getHowToFinishLeg()[0]);
        ui->possibleWayToFinishThrow2FromPlayer2->setText(_viewer->getHowToFinishLeg()[1]);
        ui->possibleWayToFinishThrow3FromPlayer2->setText(_viewer->getHowToFinishLeg()[2]);

    }

}

void ViewerWindow::remainingThrowsAreZeroInLeg()
{
    if (_aktivePlayer == _player1)
    {
        ui->possibleWayToFinishThrow1FromPlayer1->setText("");
        ui->possibleWayToFinishThrow2FromPlayer1->setText("");
        ui->possibleWayToFinishThrow3FromPlayer1->setText("");
    }
    else
    {
        ui->possibleWayToFinishThrow1FromPlayer2->setText("");
        ui->possibleWayToFinishThrow2FromPlayer2->setText("");
        ui->possibleWayToFinishThrow3FromPlayer2->setText("");
    }

}


