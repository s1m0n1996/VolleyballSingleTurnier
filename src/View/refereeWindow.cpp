/*!
* \file refereewindow.cpp
* \brief Diese Klasse ist die View Klasse des Richters
* \author Nico Meyer
*/

#include "View/refereeWindow.h"
#include "ui_refereewindow.h"
#include "View/dartboard.h"
#include <QtMath>
#include <QtDebug>
#include <QResizeEvent>


RefereeWindow::RefereeWindow(Referee* referee, RefereepopupWinningLeg *popupWinningLeg, RefereePopupBustLeg *popupBustLeg, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RefereeWindow),
    _referee(referee),
    _popupWinningLeg(popupWinningLeg),
    _popupBustLeg(popupBustLeg)
{
    RefereepopupWinningLeg* winLeg = new RefereepopupWinningLeg;
    _popupWinningLeg = winLeg;

    RefereePopupBustLeg* bustLeg = new RefereePopupBustLeg;
    _popupBustLeg = bustLeg;

    setWindowIcon(QIcon(":/img/referee.png"));

    ui->setupUi(this);

    ui->title->setMainTitleStyle();

    QPixmap* pix = new QPixmap(":/img/Dartboard.png");
    ui->DartboardView->setPixmap(*pix);

    connect(ui->DartboardView,SIGNAL(mousePos()),this, SLOT(mouseCurrentPos()));
    connect(ui->DartboardView,SIGNAL(mouseReleasedOnDartboard()),this, SLOT(mouseReleasedOnDartboard()));

    connect(_referee,SIGNAL(valueChanged()),this, SLOT(writeScore()));
    connect(ui->nextPlayer,SIGNAL(released()),this, SLOT(nextPlayer()));
    connect(ui->undoLastThrow,SIGNAL(released()),this, SLOT(undoLastThrow()));
    connect(ui->nextPlayer,SIGNAL(released()),this, SLOT(writeNextPlayer()));

    connect(_referee,SIGNAL(playerWinsLeg()),this, SLOT(playerWinsLeg()));
    connect(_popupWinningLeg,SIGNAL(playerWonLeg()),this,SLOT(nextPlayerAfterWinningLeg()));
    connect(_popupWinningLeg,SIGNAL(undoLastThrow()),this,SLOT(undoLastThrow()));

    connect(_referee,SIGNAL(playerBust()),this, SLOT(playerBust()));
    connect(_popupBustLeg,SIGNAL(playerBustLeg()),this,SLOT(nextPlayer()));
    connect(_popupBustLeg,SIGNAL(undoLastThrow()),this,SLOT(undoLastThrow()));

    writeScore();
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
    _centralPointXY = ui->DartboardView->pixmap()->width() / 2;

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
    double shiftedAngle = angle - 10;
    if (shiftedAngle < 0)
    {
        shiftedAngle += 360;
    }

    // Die Dartscheibe in die 20 Felder einteilen und den Punktwert festlegen
    // dart points around counter clock wise
    QList<int> scoreSection = {5, 12, 9, 14, 11, 8, 16, 7, 19, 3, 17, 2, 15, 10, 6, 13, 4, 18, 1, 20};

    scoreWithoutMultiplikator = scoreSection[static_cast<int>(shiftedAngle / 18)];

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
    _referee->undoThrow();
}

void RefereeWindow::writeNextPlayer()
{
    Player player(_referee->getAktivePlayer());
    ui->playerName->setText(player.getName());
}

/*!
 * \brief Öffnet ein Popupfenter sobald der Spieler sich überworfen hat.
 *
 * \return void
 *
 * Diese Methode öffnet ein Popuppfentser und die Dartscheibe wird sobald das Fenster da ist auf
 * unenable gesetzt.
 *
 */
void RefereeWindow::playerBust()
{
    _popupBustLeg->show();
    ui->DartboardView->setEnabled(false);
}

/*!
 * \brief Öffnet ein Popupfenter sobald der Spieler das Leg gewonnen hat.
 *
 * \return void
 *
 * Diese Methode öffnet ein Popuppfentser und die Dartscheibe wird sobald das Fenster da ist auf
 * unenable gesetzt.
 *
 */
void RefereeWindow::playerWinsLeg()
{
    _popupWinningLeg->show();
    ui->DartboardView->setEnabled(false);
}

void RefereeWindow::mouseReleasedOnDartboard()
{
    qDebug() <<"x" <<ui->DartboardView->x <<"y" <<ui->DartboardView->y;
    _valueMultiplikator = valueMultiplikator();
    _valueScoreWithoutMultiplikator = valueScoreWithoutMultiplikator();

    _referee->singleThrowScore(_valueMultiplikator, _valueScoreWithoutMultiplikator);
    _referee->scoreIsUnder170InLeg();
    _referee->remainingThrowsAreZeroInLeg();
    _referee->legWinningCondition();

}


/*!
 * \brief Schreibt alle Werte aus dem Model in die vorhergesehnen Felder rein.
 *
 * \return void
 *
 * Holt sich die Werte aus dem Model und schreibt diese nun in die dafür vorgesehene UI in die passenden
 * Labels.
 * Der Button für den Nächsten Spieler wird erst auf enable gesetzt wenn der aktive Spieler keine Würfe
 * mehr hat.
 *
 */
void RefereeWindow::writeScore()
{
    ui->throw1->setText(QString::number(_referee->getThrows()[0]));
    ui->throw2->setText(QString::number(_referee->getThrows()[1]));
    ui->throw3->setText(QString::number(_referee->getThrows()[2]));
    ui->gesmatpunkte->setText(QString::number(_referee->getThrowScore()));
    ui->remainScore->setText(QString::number(_referee->getRemainScore()));
    ui->countWinningLegs->setText(QString::number(_referee->getCountOfWinningLegs()));
    ui->remainingThrows->setText(QString::number(_referee->getRemainingThrows()));

    // Damit nicht einfach so auf nächster Spieler geklickt werden kann
    // Benötigt für die Datenbank damit wirklich alle Würfe gespeichert werden
    if (_referee->getRemainingThrows() == 0)
    {
        ui->nextPlayer->setEnabled(true);
        ui->nextPlayer->setEnableStyle();
        ui->DartboardView->setEnabled(false);
    }
    else
    {
        ui->nextPlayer->setEnabled(false);
        ui->DartboardView->setEnabled(true);
    }

    writeNextPlayer();
}
