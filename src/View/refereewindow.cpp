/*!
* \file refereewindow.cpp
* \brief Diese Klasse ist die View Klasse des Richters
* \author Nico Meyer
*/

#include "View/refereewindow.h"
#include "ui_refereewindow.h"
#include "View/dartboard.h"
#include <QtMath>
#include <QtDebug>


RefereeWindow::RefereeWindow(Referee* referee, RefereepopupWinningLeg *popupWinningLeg, RefereePopupBustLeg *popupBustLeg, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RefereeWindow),
    _referee(referee),
    _popupWinningLeg(popupWinningLeg),
    _popupBustLeg(popupBustLeg)
{
    RefereepopupWinningLeg* winLeg = new RefereepopupWinningLeg;
    _popupWinningLeg =winLeg;

    RefereePopupBustLeg* bustLeg = new RefereePopupBustLeg;
    _popupBustLeg = bustLeg;

    ui->setupUi(this);
    connect(ui->DartboardView,SIGNAL(mousePos()),this, SLOT(mouseCurrentPos()));
    connect(ui->DartboardView,SIGNAL(mouseReleasedOnDartboard()),this, SLOT(mouseReleasedOnDartboard()));

    connect(_referee,SIGNAL(valueChanged()),this, SLOT(writeScore()));
    connect(ui->nextPlayer,SIGNAL(released()),this, SLOT(nextPlayer()));
    connect(ui->undoLastThrow,SIGNAL(released()),this, SLOT(undoLastThrow()));
    connect(ui->nextPlayer,SIGNAL(released()),this, SLOT(writeNextPlayer()));
    connect(ui->gameStart,SIGNAL(released()),this, SLOT(gameStart()));

    connect(_referee,SIGNAL(playerWinsLeg()),this, SLOT(playerWinsLeg()));
    connect(_popupWinningLeg,SIGNAL(playerWonLeg()),this,SLOT(nextPlayerAfterWinningLeg()));
    connect(_popupWinningLeg,SIGNAL(undoLastThrow()),this,SLOT(undoLastThrow()));

    connect(_referee,SIGNAL(playerBust()),this, SLOT(playerBust()));
    connect(_popupBustLeg,SIGNAL(playerBustLeg()),this,SLOT(nextPlayer()));
    connect(_popupBustLeg,SIGNAL(undoLastThrow()),this,SLOT(undoLastThrow()));
}

RefereeWindow::~RefereeWindow()
{
    delete ui;
}

/*!
 * \brief Gibt den Wert des Multiplikators des Wurfes zurück.
 *
 * \return int
 *
 * Diese Metohde berechnet den Multiplikator des Wurfes anhand der Position wo die Maus auf der
 * Dartscheibe geklickt wurde. Der Multiplikatorbereich der Dartscheibe ist prozentual aufgeteilt.
 * Es wurde der Radius der Dartscheibe berechnet und prozentual vom Mittelpunkt der Dartscheibe
 * wird der Multiplikator bestimmt.
 *
 */
int RefereeWindow::valueMultiplikator()
{
    int multiplikator = 0;
    _centralPointXY = ui->DartboardView->width() / 2;

    double deltaX = _centralPointXY - ui->DartboardView->x;
    double deltaY = _centralPointXY - ui->DartboardView->y;
    // Radius berechnen um die Art des Feldes anzugeben
    _radius = qSqrt(qPow((deltaX), 2) + qPow((deltaY), 2));

    // Variable zur Berechnung für den Prozentualen Anteil eines Feldes
    int areaPercentage = ((_radius * 100) / _centralPointXY);

    // Prüfen in welchen Bereich man sich befindet.
    if ((areaPercentage >= 3 and areaPercentage <= 6) or
            (areaPercentage >= 7 and areaPercentage <= 42) or
            (areaPercentage >= 49 and areaPercentage <= 69))
    {
        multiplikator = _single;
    }
    else if ((areaPercentage <= 2) or
             (areaPercentage >= 70 and areaPercentage <= 75))
    {
        multiplikator = _double;
    }
    else if (areaPercentage >= 43 and areaPercentage <= 48)
    {
        multiplikator = _trible;
    }
    else
    {
        multiplikator = _miss;
    }

    return  multiplikator;
}

/*!
 * \brief Gibt den Wert des geworfenen Pfeils ohne Multiplikator zurück.
 *
 * \return int
 *
 * Diese Metohde gibt anhand der Position wo die Maus auf der Dartscheibe geklickt wurde einen Zahlenwert aus.
 *
 */
int RefereeWindow::valueScoreWithoutMultiplikator()
{
    double angle = 0.0;
    double deltaX = _centralPointXY - ui->DartboardView->x;
    double deltaY = _centralPointXY - ui->DartboardView->y;
    int scoreWithoutMultiplikator = 0;

    // Quadrant 1
    if (deltaX >= 0 and deltaY >= 0)
    {
        angle = 90 - qRadiansToDegrees(qAtan(deltaY / deltaX));
    }
    // Quadrant 2
    else if (deltaX <= 0 and deltaY >= 0)
    {
        angle = 270 + qAbs(qRadiansToDegrees(qAtan(deltaY / deltaX)));
    }
    // Quadrant 3
    else if (deltaX >= 0 and deltaY <= 0)
    {
        angle = 90 + qAbs(qRadiansToDegrees(qAtan(deltaY / deltaX)));
    }
    // Quadrant 4
    else if (deltaX <= 0 and deltaY <= 0)
    {
        angle = 270 - qRadiansToDegrees(qAtan(deltaY / deltaX));
    }

    //Damit der Kreis nicht in der Mitte der 20 beginnt, sonder um ganz links von der 20.
    double shiftedAngle = angle - 9;
    if (shiftedAngle < 0)
    {
        shiftedAngle += 360;
    }

    // Die Dartscheibe in die 20 Felder einteilen und den Punktwert festlegen
    int scoreSection = static_cast<int>(shiftedAngle / 18);
    switch (scoreSection)
    {
    case 0:
    {
        scoreWithoutMultiplikator = 5;
    }
        break;
    case 1:
    {
        scoreWithoutMultiplikator = 12;
    }
        break;
    case 2:
    {
        scoreWithoutMultiplikator = 9;
    }
        break;
    case 3:
    {
        scoreWithoutMultiplikator = 14;
    }
        break;
    case 4:
    {
        scoreWithoutMultiplikator = 11;
    }
        break;
    case 5:
    {
        scoreWithoutMultiplikator = 8;
    }
        break;
    case 6:
    {
        scoreWithoutMultiplikator = 16;
    }
        break;
    case 7:
    {
        scoreWithoutMultiplikator = 7;
    }
        break;
    case 8:
    {
        scoreWithoutMultiplikator = 19;
    }
        break;
    case 9:
    {
        scoreWithoutMultiplikator = 3;
    }
        break;
    case 10:
    {
        scoreWithoutMultiplikator = 17;
    }
        break;
    case 11:
    {
        scoreWithoutMultiplikator = 2;
    }
        break;
    case 12:
    {
        scoreWithoutMultiplikator = 15;
    }
        break;
    case 13:
    {
        scoreWithoutMultiplikator = 10;
    }
        break;
    case 14:
    {
        scoreWithoutMultiplikator = 6;
    }
        break;
    case 15:
    {
        scoreWithoutMultiplikator = 13;
    }
        break;
    case 16:
    {
        scoreWithoutMultiplikator = 4;
    }
        break;
    case 17:
    {
        scoreWithoutMultiplikator = 18;
    }
        break;
    case 18:
    {
        scoreWithoutMultiplikator = 1;
    }
        break;
    case 19:
    {
        scoreWithoutMultiplikator = 20;
    }
        break;
    }

    // Bulls-Eye berechnung
    int areaPercentage = ((_radius * 100) / _centralPointXY);
    if (areaPercentage >= 0 and areaPercentage <= 6)
    {
        scoreWithoutMultiplikator = 25;
    }
    return scoreWithoutMultiplikator;

}

void RefereeWindow::mouseCurrentPos()
{

}

void RefereeWindow::nextPlayer()
{
    _referee->nextPlayer();
}

void RefereeWindow::nextPlayerAfterWinningLeg()
{
    _referee->nextPlayerAfterWinningLeg();
}

void RefereeWindow::undoLastThrow()
{
    qDebug() <<"kommt das was ??";
    _referee->undoThrow();
}

void RefereeWindow::writeNextPlayer()
{
    ui->playerName->setText(QString::number(_referee->getAktivePlayer()));
}

void RefereeWindow::gameStart()
{
    ui->playerName->setText(QString::number(_referee->getGameStart()));

}

void RefereeWindow::playerBust()
{
    _popupBustLeg->show();
}

void RefereeWindow::playerWinsLeg()
{
    _popupWinningLeg->show();
}

void RefereeWindow::mouseReleasedOnDartboard()
{
    _valueMultiplikator = valueMultiplikator();
    _valueScoreWithoutMultiplikator =valueScoreWithoutMultiplikator();

    _referee->singleThrowScore(_valueMultiplikator, _valueScoreWithoutMultiplikator);
    _referee->legWinningCondition();

}

void RefereeWindow::writeScore()
{
    ui->throw1->setText(QString::number(_referee->getThrows()[0]));
    ui->throw2->setText(QString::number(_referee->getThrows()[1]));
    ui->throw3->setText(QString::number(_referee->getThrows()[2]));
    ui->gesmatpunkte->setText(QString::number(_referee->getThrowScore()));
    ui->remainScore->setText(QString::number(_referee->getRemainScore()));
    ui->countWinningLegs->setText(QString::number(_referee->getCountOfWinningLegs()));
    writeNextPlayer();
}
