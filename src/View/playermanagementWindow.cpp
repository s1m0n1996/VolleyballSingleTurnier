
#include "playermanagementWindow.h"



PlayermanagementWindow::PlayermanagementWindow(QWidget *parent):
    QMainWindow(parent),
 
    ui(new Ui::PlayermanagementWindow)
{
    ui->setupUi(this);
    createButtons();
    createAddPlayerEdit();
    setButtonLayout();
    connecting();
}


PlayermanagementWindow::~PlayermanagementWindow()
{
    delete ui;
}

void PlayermanagementWindow:: connecting()
{
    connect(_startTournementButton, SIGNAL(released()), this, SLOT(test));
    connect(_addPlayerButton,SIGNAL(released()), this, SLOT(addPlayer()));
}
void PlayermanagementWindow::test()
{
    ui->maxPlayerLabel->setText("0");

}

void PlayermanagementWindow::createAddPlayerEdit()
{

}

void PlayermanagementWindow::createButtons()
{
    _addPlayerButton        = new WindowButton("Spieler hinzufügen");
    _startTournementButton  = new WindowButton("Turnier starten");
    _startTournementButton->setEnabled(false);
}


void PlayermanagementWindow::setButtonLayout()
{
    ui->startTournementLayout->addWidget(_startTournementButton, Qt::AlignBottom,0);
    ui->verticalLayout_2->addWidget(_addPlayerButton);
}


void PlayermanagementWindow::setMaxPlayerLabel()
{
    int playerNumber= 1;
    if (round(log(playerNumber)/log(2)) == playerNumber)
    {
        ui->maxPlayerLabel->setText("0");
        _startTournementButton->setEnabled(true);
    }
    else
    {
        int neededPlayerNumber = 4;
        ui->maxPlayerLabel->setText(QString::number(neededPlayerNumber));
    }

}


void PlayermanagementWindow::addPlayer()
{
    ui->maxPlayerLabel->setText("0");
}

