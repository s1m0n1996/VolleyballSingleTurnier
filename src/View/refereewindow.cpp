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

    // Bulls-Eye berechnung
    int areaPercentage = ((_radius * 100) / _centralPointXY);
    if (areaPercentage >= 0 and areaPercentage <= 6)
    {
        scoreWithoutMultiplikator = 25;
    }

    return scoreWithoutMultiplikator;

}

void RefereeWindow::remainScore()
{

//    ui->remainScore->setText(QString::number(referee->getRemainingScore()));


    if (_throwCounter == 1 and _numberOfSubtraction == 3)
    {
        if (_player == 0)
        {
            _remainScore[0] = _remainScore[0] - ui->throw1->text().toInt();
            _numberOfSubtraction--;
        }
        else if (_player > 0)
        {
            _remainScore[1] = _remainScore[1] - ui->throw1->text().toInt();
            _numberOfSubtraction--;
        }

    }
    else if (_throwCounter == 2 and _numberOfSubtraction == 2)
    {
        if (_player == 0)
        {
            _remainScore[0] = _remainScore[0] - ui->throw2->text().toInt();
            _numberOfSubtraction--;
        }
        else if (_player > 0)
        {
            _remainScore[1] = _remainScore[1] - ui->throw2->text().toInt();
            _numberOfSubtraction--;
        }
    }
    else if (_throwCounter == 3 and _numberOfSubtraction == 1)
    {
        if (_player == 0)
        {
            _remainScore[0] = _remainScore[0] - ui->throw3->text().toInt();
            _numberOfSubtraction--;
        }
        else if (_player > 0)
        {
            _remainScore[1] = _remainScore[1] - ui->throw3->text().toInt();
            _numberOfSubtraction--;
        }
    }

    if (_player == 0)
    {
        ui->remainScore->setText(QString::number(_remainScore[0]));
    }
    else if (_player > 0)
    {
        ui->remainScore->setText(QString::number(_remainScore[1]));
    }

}


void RefereeWindow::mouseReleasedOnDartboard()
{
    int multiplikator = valueMultiplikator();
    int scoreWithoutMultiplikator = valueScoreWithoutMultiplikator();

    if (_throwCounter == 0)
    {
        _throwScore = scoreWithoutMultiplikator * multiplikator;
        ui->throw1->setText(QString::number(_throwScore));
        _throwCounter++;
    }
    else if (_throwCounter == 1)
    {
        _throwScore = scoreWithoutMultiplikator * multiplikator;
        ui->throw2->setText(QString::number(_throwScore));
        _throwCounter++;
    }
    else if (_throwCounter == 2)
    {
        _throwScore = scoreWithoutMultiplikator * multiplikator;
        ui->throw3->setText(QString::number(_throwScore));
        _throwCounter++;
    }

    remainScore();
}

//Nächster Spieler ist an der Reihe
void RefereeWindow::on_naechsterSpieler_released()
{
    _throwCounter = 0;
    _numberOfSubtraction = 3;

    if (_player == 0)
    {
        ui->remainScore->setText(QString::number(_remainScore[1]));
        _player++;
    }
    else
    {
        ui->remainScore->setText(QString::number(_remainScore[0]));
        _player = 0;
    }

    ui->throw1->setText(QString::number(0));
    ui->throw2->setText(QString::number(0));
    ui->throw3->setText(QString::number(0));

}

// Setzt den letzten Wurf zurück. Notwendig, da man sich verklicken kann
void RefereeWindow::on_undoLetzterWurf_released()
{
    if (_throwCounter == 3)
    {
        if (_player == 0)
        {
            _remainScore[0] = _remainScore[0] + ui->throw3->text().toInt();
            ui->remainScore->setText(QString::number(_remainScore[0]));
        }
        else if (_player > 0)
        {
            _remainScore[1] = _remainScore[1] + ui->throw3->text().toInt();
            ui->remainScore->setText(QString::number(_remainScore[1]));
        }

        ui->throw3->setText(QString::number(0));
        _throwCounter--;
        _numberOfSubtraction++;
    }
    else if ((_throwCounter == 1 or _throwCounter == 2)
             and ui->throw2->text().toInt() != 0 and ui->throw3->text().toInt() == 0)
    {
        if (_player == 0)
        {
            _remainScore[0] = _remainScore[0] + ui->throw2->text().toInt();
            ui->remainScore->setText(QString::number(_remainScore[0]));
        }
        else if (_player > 0)
        {
            _remainScore[1] = _remainScore[1] + ui->throw2->text().toInt();
            ui->remainScore->setText(QString::number(_remainScore[1]));
        }

        ui->throw2->setText(QString::number(0));
        _throwCounter--;
        _numberOfSubtraction++;
    }
    else if ((_throwCounter == 1 or _throwCounter == 0)
             and ui->throw2->text().toInt() == 0 and ui->throw1->text().toInt() != 0)
    {
        if (_player == 0)
        {
            _remainScore[0] = _remainScore[0] + ui->throw1->text().toInt();
            ui->remainScore->setText(QString::number(_remainScore[0]));
        }
        else if (_player > 0)
        {
            _remainScore[1] = _remainScore[1] + ui->throw1->text().toInt();
            ui->remainScore->setText(QString::number(_remainScore[1]));
        }

        ui->throw1->setText(QString::number(0));
        _throwCounter = 0;
        _numberOfSubtraction++;
    }
}
