/*!
* \file refereepopupbustleg.cpp
* \brief Diese Klasse ist ein Popup welches für den Richter dient
* \author Nico Meyer
*/

#include "refereepopupbustleg.h"

RefereePopupBustLeg::RefereePopupBustLeg(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Richteransicht");
    setWindowIcon(QIcon(":/img/referee.png"));

    _refereeInformation = new WindowLabel("Der Spieler hat sich überworfen oder nicht mit einem Doppel beendet");
    _refereeInformation->setTitleStyle();
    _refereeAction = new WindowLabel("Wenn der Wurf richtig eingegeben wurde,\n"
                                     "dann auf den Button 'Nächster Spieler' klicken\n"
                                     "ansonsten auf den Button 'Letzter Wurf rückgänig'");

    _nextPlayer = new WindowButton("Nächster Spieler");
    _undoLastThrow = new WindowButton("Letzter Wurf rückgänig");


    QGridLayout* layout = new QGridLayout;
    QHBoxLayout* buttonLayout = new QHBoxLayout;


    buttonLayout -> addWidget(_nextPlayer);
    buttonLayout -> addWidget(_undoLastThrow);


    layout -> addWidget(_refereeInformation,0,0);
    layout -> addWidget(_refereeAction,1,0);
    layout -> addLayout(buttonLayout,2,1);

    setLayout(layout);

    connect(_nextPlayer, SIGNAL(released()), this, SLOT(openRefereeBustLeg()));
    connect(_undoLastThrow, SIGNAL(released()), this, SLOT(openRefereeUndoLastThrow()));
    setWindowFlags(Qt::SubWindow);
}

void RefereePopupBustLeg::openRefereeBustLeg(void)
{
    emit playerBustLeg();
    setWindowFlags(Qt::SubWindow);
}

void RefereePopupBustLeg::openRefereeUndoLastThrow(void)
{
    emit undoLastThrow();
    setWindowFlags(Qt::SubWindow);
}
