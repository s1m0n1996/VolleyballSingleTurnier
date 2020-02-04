#include "playermanagementWindow.h"


PlayermanagementWindow::PlayermanagementWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::PlayermanagementWindow)
{
    ui->setupUi(this);
    createButtons();
    setButtonLayout();
    connecting();


}

PlayermanagementWindow::~PlayermanagementWindow()
{
    delete ui;
}

void PlayermanagementWindow::test()
{
    ui->maxPlayerLabel->setText("0");
}
void PlayermanagementWindow:: connecting()
{
    connect(ui->maxPlayerLabel, SIGNAL(ValueChanged()), this, SLOT(setMaxPlayerLabel()));
    connect(_startTournement, SIGNAL(clicked()),this, SLOT(test()));

    if ( 0 == ui->maxPlayerLabel->text())
    {
        connect(_addPlayer, SIGNAL(clicked()),this, SLOT(addPlayer()));
    }
}
void PlayermanagementWindow::createButtons()
{
    _addPlayer = new QPushButton("Spieler hinzufügen");
    _startTournement = new QPushButton("Turnier starten");
}

void PlayermanagementWindow::setButtonLayout()
{
    ui->startTournementLayout->addWidget(_startTournement);
    ui->verticalLayout_2->addWidget(_addPlayer);
}


void PlayermanagementWindow::setMaxPlayerLabel()
{
    int playerNumber= 1;
    if (round(log(playerNumber)/log(2)) == playerNumber)
    {
        ui->maxPlayerLabel->setText("0");
    }
    else
    {
        int neededPlayerNumber = 4;
        ui->maxPlayerLabel->setText(QString::number(neededPlayerNumber));

    }
}

void PlayermanagementWindow::addPlayer()
{
    ui->maxPlayerLabel->setText("Anna");
}

