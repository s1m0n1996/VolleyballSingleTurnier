/*!
* \file refereepopupwinningleg.cpp
* \brief Diese Klasse ist ein Popup welches für den Richter dient
* \author Nico Meyer
*/
#include <QGridLayout>

#include "Model/referee.h"
#include "Model/player.h"

#include "View/refereePopupWinningLeg.h"
#include "View/windowButton.h"
#include "View/windowEdit.h"
#include "View/windowLabel.h"
#include "View/winnerpopup.h"

/*!
 * \brief RefereepopupWinningLeg::RefereepopupWinningLeg ist Konstruktor der Klasse RefereepopupWinningLeg
 * \param[in] parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 * \return void
 *
 * Erstellt das Popup als unter Fenster
 */
RefereepopupWinningLeg::RefereepopupWinningLeg(QWidget* parent) : QWidget(parent)
{
    setWindowIcon(QIcon(":/img/referee.png"));
    setWindowTitle("Richteransicht");
    setWindowFlags(Qt::SubWindow);

    createWidgets();

    setAllLayout();

    connecting();
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

void RefereepopupWinningLeg::createWidgets(void)
{
    _refereeInformation = new WindowLabel("Der Spieler hat das Leg gewonnen,\n");
    _refereeInformation->setTitleStyle();
    _refereeAction = new WindowLabel("Wenn der Wurf richtig eingegeben wurde 'Leg gewonnen',\n"
                                     "ansonsten 'Letzten Wurf rückgänig'");

    _legWin = new WindowButton("Leg gewonnen");
    _undoLastThrow = new WindowButton("Letzten Wurf rückgänig");

}

void RefereepopupWinningLeg::setAllLayout(void)
{

    QGridLayout* layout = new QGridLayout;
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    buttonLayout->addWidget(_legWin);
    buttonLayout->addWidget(_undoLastThrow);


    layout->addWidget(_refereeInformation, 0, 0);
    layout->addWidget(_refereeAction, 1, 0);
    layout->addLayout(buttonLayout, 2, 1);

    setLayout(layout);
}

void RefereepopupWinningLeg::connecting(void)
{

    connect(_legWin, SIGNAL(released()), this, SLOT(openRefereeWinningLeg()));
    connect(_undoLastThrow, SIGNAL(released()), this, SLOT(openRefereeUndoLastThrow()));
}
