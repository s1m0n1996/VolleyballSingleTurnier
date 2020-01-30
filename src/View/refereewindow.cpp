#include "View/refereewindow.h"
#include "ui_refereewindow.h"
#include "View/dartboard.h"
#include <QtMath>


RefereeWindow::RefereeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RefereeWindow)
{
    ui->setupUi(this);
    connect(ui->DartboardView,SIGNAL(mousePos()),this, SLOT(mouseCurrentPos()));
    connect(ui->DartboardView,SIGNAL(mouseReleasedOnDartboard()),this, SLOT(mouseReleasedOnDartboard()));
}

RefereeWindow::~RefereeWindow()
{
    delete ui;
}


void RefereeWindow::mouseCurrentPos()
{
    ui->Koordinaten->setText(QString("X = %1, Y = %2")
                             .arg(ui->DartboardView->x)
                             .arg(ui->DartboardView->y));
}

int RefereeWindow::valueMultiplikator()
{
    int multiplikator = 0;
    _mittelpunktXY = ui->DartboardView->width() / 2;

    double deltaX = _mittelpunktXY - ui->DartboardView->x;
    double deltaY = _mittelpunktXY - ui->DartboardView->y;
    // Radius berechnen um die Art des Feldes anzugeben
    //FIXME: wenn genau der Mittelpunkt gedrückt wird dann Wurzel von 0 --> Programm stürzt ab
    _radius = qSqrt(qPow((deltaX), 2) + qPow((deltaY), 2));

    // Prüfen in welchen Bereich man sich befindet.
    if (((((_radius * 100) / _mittelpunktXY) >= 3) and (((_radius * 100) / _mittelpunktXY) <= 6)) or
            ((((_radius * 100) / _mittelpunktXY) >= 7) and (((_radius * 100) / _mittelpunktXY) <= 42)) or
            ((((_radius * 100) / _mittelpunktXY) >= 49) and (((_radius * 100) / _mittelpunktXY) <= 69)))
    {
        multiplikator = _single;
        ui->Multiplikator->setText(QString("Feld Typ = %1")
                                   .arg(multiplikator));
    }
    else if ((((_radius * 100) / _mittelpunktXY) <= 2) or
             ((((_radius * 100) / _mittelpunktXY) >= 70) and (((_radius * 100) / _mittelpunktXY) <= 75)))
    {
        multiplikator = _double;
        ui->Multiplikator->setText(QString("Feld Typ = %1")
                                   .arg(multiplikator));
    }
    else if (((_radius * 100) / _mittelpunktXY) >= 43 and ((_radius * 100) / _mittelpunktXY) <= 48)
    {
        multiplikator = _trible;
        ui->Multiplikator->setText(QString("Feld Typ = %1")
                                   .arg(multiplikator));
    }
    else
    {
        multiplikator = _miss;
        ui->Multiplikator->setText(QString("Feld Typ = %1")
                                   .arg(multiplikator));
    }

    return  multiplikator;
}


int RefereeWindow::valueScoreWithoutMultiplikator()
{
    double angle = 0.0;
    double deltaX = _mittelpunktXY - ui->DartboardView->x;
    double deltaY = _mittelpunktXY - ui->DartboardView->y;
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

    double shiftedAngle = angle - 9;
    if (shiftedAngle < 0)
    {
        shiftedAngle += 360;
    }

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
        scoreWithoutMultiplikator = 9;
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

    return scoreWithoutMultiplikator;

}


void RefereeWindow::mouseReleasedOnDartboard()
{
    int multiplikator = valueMultiplikator();
    int scoreWithoutMultiplikator = valueScoreWithoutMultiplikator();

    if (_throwCounter == 0)
    {
        _throwCounter++;
        _throwScore = scoreWithoutMultiplikator * multiplikator;
        ui->wurf1->setText(QString::number(_throwScore));
    }
    else if (_throwCounter == 1)
    {
        _throwCounter++;
        _throwScore = scoreWithoutMultiplikator * multiplikator;
        ui->wurf2->setText(QString::number(_throwScore));
    }
    else if (_throwCounter == 2)
    {
        _throwCounter++;
        _throwScore = scoreWithoutMultiplikator * multiplikator;
        ui->wurf3->setText(QString::number(_throwScore));
    }
}

//Nächster Spieler ist an der Reihe
void RefereeWindow::on_naechsterSpieler_released()
{
    _throwCounter = 0;
    ui->wurf1->setText(QString::number(0));
    ui->wurf2->setText(QString::number(0));
    ui->wurf3->setText(QString::number(0));
}

// Setzt den letzten Wurf zurück. Notwendig, da man sich verklicken kann
void RefereeWindow::on_undoLetzterWurf_released()
{
    if (_throwCounter == 3)
    {
        ui->wurf3->setText(QString::number(0));
        _throwCounter--;
    }
    else if ((_throwCounter == 1 or _throwCounter == 2)
             and ui->wurf2->text().toInt() != 0 and ui->wurf3->text().toInt() == 0)
    {
        ui->wurf2->setText(QString::number(0));
        _throwCounter--;
    }
    else if ((_throwCounter == 1 or _throwCounter == 0)
             and ui->wurf2->text().toInt() == 0)
    {
        ui->wurf1->setText(QString::number(0));
        _throwCounter = 0;
    }
}
