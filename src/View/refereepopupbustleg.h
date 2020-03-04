/*!
* \file refereepopupbustleg.h
* \brief Diese Klasse ist ein Popup welches für den Richter dient
* \author Nico Meyer
*/

#ifndef REFEREEPOPUPBUSTLEG_H
#define REFEREEPOPUPBUSTLEG_H

#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <View/tournamentwindow.h>
#include "View/windowbutton.h"
#include "View/windowedit.h"
#include "View/windowlabel.h"
#include <QDebug>

class RefereePopupBustLeg : public QWidget
{
    Q_OBJECT
public:
    explicit RefereePopupBustLeg(QWidget *parent = nullptr);

signals:
    void playerBustLeg(void);
    void undoLastThrow(void);

public slots:
    void openRefereeBustLeg(void);
    void openRefereeUndoLastThrow(void);

private:
    WindowLabel* _refereeInformation = nullptr;
    WindowLabel* _refereeAction = nullptr;
    WindowButton* _nextPlayer = nullptr;
    WindowButton* _undoLastThrow = nullptr;

};

#endif // REFEREEPOPUPBUSTLEG_H
