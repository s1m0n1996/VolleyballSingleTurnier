#include "playermanagment.h"
#include "ui_playermanagment.h"
#include "math.h"
#include <QDebug>

using namespace std;

Playermanagment::Playermanagment(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::Playermanagment)
{
    ui->setupUi(this);
    ui->maxPlayerLabel->setText("LEa");
    _addPlayer = new QPushButton("Spieler hinzufügen");
    _startTournement = new QPushButton("Turnier starten");
    ui->startTournementLayout->addWidget(_startTournement);
    ui->verticalLayout_2->addWidget(_addPlayer);
    connect(ui->maxPlayerLabel, SIGNAL(ValueChanged()), this, SLOT(setMaxPlayerLabel()));
    connect(_startTournement, SIGNAL(clicked()),this, SLOT(test()));
    if ( 0 == ui->maxPlayerLabel->text())
    {
        connect(_addPlayer, SIGNAL(clicked()),this, SLOT(addPlayer()));
    }
}

Playermanagment::~Playermanagment()
{
    delete ui;
}

void Playermanagment:: test()
{
     ui->maxPlayerLabel->setText("5");
}
void Playermanagment:: setMaxPlayerLabel()
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

void Playermanagment:: addPlayer()
{
    //ui->maxPlayerLabel->clear();
    ui->maxPlayerLabel->setText("Anna");
}

