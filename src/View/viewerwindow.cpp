#include "viewerwindow.h"
#include "ui_viewerWindow.h"

ViewerWindow::ViewerWindow(Referee *referee, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerWindow),
    _referee(referee)
{
    ui->setupUi(this);
    connect(_referee,SIGNAL(valueChanged()),this, SLOT(writeScore()));

    writeScore();
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
    }
    else
    {
        ui->throw1FromPlayer2->setText(QString::number(_referee->getThrows()[0]));
        ui->throw2FromPlayer2->setText(QString::number(_referee->getThrows()[1]));
        ui->throw3FromPlayer2->setText(QString::number(_referee->getThrows()[2]));
        ui->all3ThrowsPlayer2->setText(QString::number(_referee->getThrowScore()));
        ui->remainingThrowsForPlayer2->setText(QString::number(_referee->getRemainingThrows()));
    }

    ui->remainScorePlayer1->setText(QString::number(_referee->getRemainScoreForViewer()[0]));
    ui->remainScorePlayer2->setText(QString::number(_referee->getRemainScoreForViewer()[1]));

    ui->countOfWinningLegsPlayer1->setText(QString::number(_referee->getWinningLegCounterForViewer()[0]));
    ui->countOfWinningLegsPlayer1->setText(QString::number(_referee->getWinningLegCounterForViewer()[1]));
}
