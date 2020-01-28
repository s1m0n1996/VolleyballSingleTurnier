#include "playermanagment.h"
#include "ui_playermanagment.h"
#include "math.h"

using namespace std;

Playermanagment::Playermanagment(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Playermanagment)
{
    ui->setupUi(this);
    connect(ui->maxPlayerLabel, SIGNAL(ValueChanged()), this, SLOT(setMaxPlayerLabel));
}

Playermanagment::~Playermanagment()
{
    delete ui;
}

void Playermanagment:: setMaxPlayerLabel(int playerNumber)
{

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

//void Playermanagment:: addPlayer(string playername)
//{
//    int hallo
//}

