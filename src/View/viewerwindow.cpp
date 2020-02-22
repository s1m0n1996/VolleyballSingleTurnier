#include "viewerwindow.h"
#include "ui_viewerWindow.h"

ViewerWindow::ViewerWindow(Referee *referee, Viewer* viewer, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerWindow),
    _referee(referee),
    _viewer(viewer)
{
    Viewer* viewers = new Viewer;
    _viewer = viewers;

    ui->setupUi(this);
    connect(_referee,SIGNAL(valueChanged()),this, SLOT(writeScore()));
    connect(_referee,SIGNAL(scoreIsUnder170()),this, SLOT(scoreIsUnder170InLeg()));
    connect(_referee,SIGNAL(remainingThrowsAreZero()),this, SLOT(remainingThrowsAreZeroInLeg()));
    connect(_viewer,SIGNAL(howToFinishLeg()),this,SLOT(possibleWayToFinishLeg()));
}

ViewerWindow::~ViewerWindow()
{
    delete ui;
}

void ViewerWindow::writeScore()
{
    _player1 = _referee->getAllPlayersForViewer()[0];
    _player2 = _referee->getAllPlayersForViewer()[1];

    ui->nameOfPlayer1->setText(QString::number(_player1));
    ui->nameOfPlayer2->setText(QString::number(_player2));

    _aktivePlayer = _referee->getAktivePlayer();

    ui->aktivePlayer->setText(QString::number(_aktivePlayer));

    if ((_aktivePlayer + 1) == _player1)
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

    ui->remainScorePlayer1->setText(QString::number(_referee->getRemainScoreForViewer()[0]));
    ui->remainScorePlayer2->setText(QString::number(_referee->getRemainScoreForViewer()[1]));

}

void ViewerWindow::scoreIsUnder170InLeg()
{
    _viewer->createJsonDocument(_referee->getRemainingThrows(), _referee->getRemainScore());
}



void ViewerWindow::possibleWayToFinishLeg()
{
    _aktivePlayer = _referee->getAktivePlayer();

    if ((_aktivePlayer + 1) == _player1)
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
    ui->possibleWayToFinishThrow1FromPlayer1->setText("");
    ui->possibleWayToFinishThrow2FromPlayer1->setText("");
    ui->possibleWayToFinishThrow3FromPlayer1->setText("");
}


