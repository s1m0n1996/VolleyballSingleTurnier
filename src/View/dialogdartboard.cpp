#include "View/dialogdartboard.h"
#include "ui_dialogdartboard.h"
#include "View/dartboard.h"


DialogDartboard::DialogDartboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDartboard)
{
    ui->setupUi(this);
    connect(ui->DartboardView,SIGNAL(mousePos()),this, SLOT(mouseCurrentPos()));
    connect(ui->DartboardView,SIGNAL(mouseReleasedOnDartboard()),this, SLOT(mouseReleasedOnDartboard()));
}

DialogDartboard::~DialogDartboard()
{
    delete ui;
}

void DialogDartboard::mouseCurrentPos()
{
    ui->Koordinaten->setText(QString("X = %1, Y = %2").arg(ui->DartboardView->x)
                             .arg(ui->DartboardView->y));
}

void DialogDartboard::mouseReleasedOnDartboard()
{
    // Man müsste eine Liste erstellen, wo alle Felder der Dartscheibe mit Pixel angaben versehen sind,
    // diese Liste muss dann mit einer Schleife durchgelaufen werden und sobald die Pixel in dem Bereich liegen
    // muss der Text bei dem entsprechendem Wurf angepasst werden.

    if (ui->DartboardView->x > 320 and ui->DartboardView->x < 390 and
            ui->DartboardView->y > 115 and ui->DartboardView->y < 185 and
            ui->DartboardView->throwCounter == 0)
    {
        ui->DartboardView->throwCounter++;
        ui->wurf1->setText(QString::number(20));
    }
    else if (ui->DartboardView->x > 320 and ui->DartboardView->x < 390 and
             ui->DartboardView->y > 115 and ui->DartboardView->y < 185 and
             ui->DartboardView->throwCounter == 1)
    {
        ui->DartboardView->throwCounter++;
        ui->wurf2->setText(QString::number(20));
    }
    else if (ui->DartboardView->x > 320 and ui->DartboardView->x < 390 and
             ui->DartboardView->y > 115 and ui->DartboardView->y < 185 and
             ui->DartboardView->throwCounter == 2)
    {
        ui->wurf3->setText(QString::number(20));
    }
}

//next player's turn
void DialogDartboard::on_naechsterSpieler_released()
{
    ui->DartboardView->throwCounter = 0;
    ui->wurf1->setText(QString::number(0));
    ui->wurf2->setText(QString::number(0));
    ui->wurf3->setText(QString::number(0));
}

// UNDO-function resets the last throw.
void DialogDartboard::on_undoLetzterWurf_released()
{
    if (ui->DartboardView->throwCounter == 2 and ui->wurf3->text().toInt() != 0 )
    {
        ui->wurf3->setText(QString::number(0));
        ui->DartboardView->throwCounter--;
    }
    else if ((ui->DartboardView->throwCounter == 1 or ui->DartboardView->throwCounter == 2)
             and ui->wurf2->text().toInt() != 0 and ui->wurf3->text().toInt() == 0)
    {
        ui->wurf2->setText(QString::number(0));
        ui->DartboardView->throwCounter--;
    }
    else if ((ui->DartboardView->throwCounter == 1 or ui->DartboardView->throwCounter == 0)
             and ui->wurf2->text().toInt() == 0)
    {
        ui->wurf1->setText(QString::number(0));
        ui->DartboardView->throwCounter = 0;
    }
}
