/*!
* \file refereepopupwinningleg.h
* \brief Diese Klasse ist ein Popup welches für den Richter dient
* \author Nico Meyer
*/

#ifndef REFEREEPOPUPWINNINGLEG_H
#define REFEREEPOPUPWINNINGLEG_H

#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <View/tournementwindow.h>
#include "View/windowbutton.h"
#include "View/windowedit.h"
#include "View/windowlabel.h"
#include <QDebug>

class RefereepopupWinningLeg : public QWidget
{
    Q_OBJECT
public:
    explicit RefereepopupWinningLeg(QWidget *parent = nullptr);

signals:
    void playerWonLeg(void);
    void undoLastThrow(void);

public slots:
    void openRefereeWinningLeg(void);
    void openRefereeUndoLastThrow(void);

private:
    WindowLabel* _refereeInformation = nullptr;
    WindowLabel* _refereeAction = nullptr;
    WindowButton* _legWin = nullptr;
    WindowButton* _undoLastThrow = nullptr;

};

#endif // REFEREEPOPUPWINNINGLEG_H



