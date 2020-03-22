#include <QGridLayout>

#include "View/refereePopupBustLeg.h"
#include <View/tournamentWindow.h>
#include "View/windowButton.h"
#include "View/windowEdit.h"
#include "View/windowLabel.h"


/*!
 * \brief RefereePopupBustLeg::RefereePopupBustLeg ist Konstruktor der Klasse RefereePopupBustLeg
 * \param[in] parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 * \return void
 *
 * Erstellt das Popup als unter Fenster
 */
RefereePopupBustLeg::RefereePopupBustLeg(QWidget* parent) : QWidget(parent)
{
    setWindowIcon(QIcon(":/img/referee.png"));
    setWindowTitle("Richteransicht");

    setWindowFlags(Qt::SubWindow);

    createWidgets();

    setAllLayout();

    connecting();

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

void RefereePopupBustLeg::createWidgets(void)
{
    _refereeInformation = new WindowLabel("Der Spieler hat das Leg falsch beendet");

    _refereeInformation->setTitleStyle();
    _refereeAction = new WindowLabel("Wenn der Wurf richtig eingegeben wurde 'Nächster Spieler'\n"
                                     "ansonsten 'Letzter Wurf rückgänig'");

    _nextPlayer = new WindowButton("Nächster Spieler");
    _undoLastThrow = new WindowButton("Letzter Wurf rückgänig");
}

void RefereePopupBustLeg::setAllLayout(void)
{

    QGridLayout* layout = new QGridLayout;
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    buttonLayout->addWidget(_nextPlayer);
    buttonLayout->addWidget(_undoLastThrow);

    layout->addWidget(_refereeInformation, 0, 0);
    layout->addWidget(_refereeAction, 1, 0);
    layout->addLayout(buttonLayout, 2, 1);

    setLayout(layout);
}

void RefereePopupBustLeg::connecting(void)
{
    connect(_nextPlayer, SIGNAL(released()), this, SLOT(openRefereeBustLeg()));
    connect(_undoLastThrow, SIGNAL(released()), this, SLOT(openRefereeUndoLastThrow()));
}
