/*!
* \file refereepopupwinningleg.cpp
* \brief Diese Klasse ist ein Popup welches für den Richter dient
* \author Nico Meyer
*/

#include "refereePopupWinningLeg.h"
#include "View/WinnerPopup.h"
#include "Model/referee.h"
#include <Model/player.h>

RefereepopupWinningLeg::RefereepopupWinningLeg(QWidget *parent) : QWidget(parent)
{


    _refereeInformation = new WindowLabel("Der Spieler hat das Leg gewonnen,\n"
                                          "wenn der Wurf richtig eingegeben wurde");
    _refereeInformation->setTitleStyle();
    _refereeAction = new WindowLabel("Wenn der Wurf richtig eingegeben wurde,\n"
                                     "dann auf den Button 'Leg gewonnen' klicken\n"
                                     "ansonsten auf den Button 'Letzten Wurf rückgänig machen'");

    _legWin = new WindowButton("Leg gewonnen");
    _undoLastThrow = new WindowButton("Letzten Wurf rückgänig machen");


    QGridLayout* layout = new QGridLayout;
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    buttonLayout -> addWidget(_legWin);
    buttonLayout -> addWidget(_undoLastThrow);


    layout -> addWidget(_refereeInformation,0,0);
    layout -> addWidget(_refereeAction,1,0);
    layout ->  addLayout(buttonLayout,2,1);

    setLayout(layout);

    connect(_legWin, SIGNAL(released()), this, SLOT(openRefereeWinningLeg()));
    connect(_undoLastThrow, SIGNAL(released()), this, SLOT(openRefereeUndoLastThrow()));
    setWindowFlags(Qt::SubWindow);
}

void RefereepopupWinningLeg::openRefereeWinningLeg(void)
{
    emit playerWonLeg();
    setWindowFlags(Qt::SubWindow);
}

void RefereepopupWinningLeg::openRefereeUndoLastThrow(void)
{
    emit undoLastThrow();
    setWindowFlags(Qt::SubWindow);
}
