/*!
* \file refereepopupwinningleg.cpp
* \brief Diese Klasse ist ein Popup welches für den Richter dient
* \author Nico Meyer
*/

#include "refereepopupwinningleg.h"

RefereepopupWinningLeg::RefereepopupWinningLeg(QWidget *parent) : QWidget(parent)
{

    _refereeInformation = new WindowLabel("Der Spieler hat das Leg gewonnen. Wenn der Wurf richtig eingegeben wurde.");
    _refereeAction = new WindowLabel("Wenn der Wurf richtig eingegeben wurde dann auf den Button 'Leg gewonnen' klicken"
                                     " ansonsten auf den Button 'Undo letzter Wurf'");

    _legWin = new WindowButton("Leg gewonnen");
    _undoLastThrow = new WindowButton("UNDO letzter Wurf");


    QGridLayout* layout = new QGridLayout;

    layout -> addWidget(_refereeInformation,0,0);
    layout -> addWidget(_refereeAction,1,0);
    layout -> addWidget(_legWin,2,0);
    layout -> addWidget(_undoLastThrow,2,1);
    setLayout(layout);

    connect(_legWin, SIGNAL(released()), this, SLOT(openRefereeWinningLeg()));
    connect(_undoLastThrow, SIGNAL(released()), this, SLOT(openRefereeUndoLastThrow()));
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);
}

void RefereepopupWinningLeg::openRefereeWinningLeg(void)
{
    emit playerWonLeg();
    setWindowFlags(Qt::Window);
}

void RefereepopupWinningLeg::openRefereeUndoLastThrow(void)
{
    emit undoLastThrow();
    setWindowFlags(Qt::Window);
}
