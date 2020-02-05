
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

void PlayermanagementWindow:: connecting()
{
    connect(_startTournement, SIGNAL(released()), this, SLOT(test));
}
void PlayermanagementWindow::test()
{
    ui->maxPlayerLabel->setText("0");

}


void PlayermanagementWindow::createButtons()
{
    _addPlayer = new QPushButton("Spieler hinzufügen");
    _startTournement = new QPushButton("Turnier starten");
    _startTournement->setEnabled(false);
}


void PlayermanagementWindow::setButtonLayout()
{
    ui->startTournementLayout->addWidget(_startTournement, Qt::AlignBottom,0);
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

