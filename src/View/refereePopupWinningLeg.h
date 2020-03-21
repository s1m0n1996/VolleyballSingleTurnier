#ifndef REFEREEPOPUPWINNINGLEG_H
#define REFEREEPOPUPWINNINGLEG_H

#include <QWidget>
#include <QMainWindow>

class WindowButton;
class WindowLabel;

/*!
* \file refereepopupwinningleg.h
* \brief Diese Klasse ist ein Popup welches für den Richter dient
* \author Nico Meyer
*/

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
    void createWidgets(void);
    void setAllLayout(void);
    void connecting(void);

    WindowLabel* _refereeInformation = nullptr;
    WindowLabel* _refereeAction = nullptr;
    WindowButton* _legWin = nullptr;
    WindowButton* _undoLastThrow = nullptr;

};

#endif // REFEREEPOPUPWINNINGLEG_H



