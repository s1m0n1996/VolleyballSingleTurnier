/*!
* \file refereepopupbustleg.cpp
* \brief Diese Klasse ist ein Popup welches für den Richter dient
* \author Nico Meyer
*/

#include "refereepopupbustleg.h"

RefereePopupBustLeg::RefereePopupBustLeg(QWidget *parent) : QWidget(parent)
{

    _refereeInformation = new WindowLabel("Der Spieler hat siche überworfen oder nicht mit einem Doppel beendet.");
    _refereeAction = new WindowLabel("Wenn der Wurf richtig eingegeben wurde dann auf den Button 'Nächster Spieler' klicken"
                                     " ansonsten auf den Button 'Undo letzter Wurf'");

    _naechsterSpieler = new WindowButton("Nächster Spieler");
    _undoLastThrow = new WindowButton("UNDO letzter Wurf");


    QGridLayout* layout = new QGridLayout;

    layout -> addWidget(_refereeInformation,0,0);
    layout -> addWidget(_refereeAction,1,0);
    layout -> addWidget(_naechsterSpieler,2,0);
    layout -> addWidget(_undoLastThrow,2,1);
    setLayout(layout);

    connect(_naechsterSpieler, SIGNAL(released()), this, SLOT(openRefereeBustLeg()));
    connect(_undoLastThrow, SIGNAL(released()), this, SLOT(openRefereeUndoLastThrow()));
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);
}

void RefereePopupBustLeg::openRefereeBustLeg(void)
{
    emit playerBustLeg();
    setWindowFlags(Qt::Window);
}

void RefereePopupBustLeg::openRefereeUndoLastThrow(void)
{
    emit undoLastThrow();
    setWindowFlags(Qt::Window);
}
