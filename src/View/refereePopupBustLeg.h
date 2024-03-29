#ifndef REFEREEPOPUPBUSTLEG_H
#define REFEREEPOPUPBUSTLEG_H

#include <QWidget>
#include <QMainWindow>

class WindowButton;
class WindowLabel;

/*!
* \file refereepopupbustleg.cpp
* \brief Diese Klasse ist ein Popup welches für den Richter dient
* \author Nico Meyer
*/
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
    void createWidgets(void);
    void setAllLayout(void);
    void connecting(void);

    WindowLabel* _refereeInformation = nullptr;
    WindowLabel* _refereeAction = nullptr;
    WindowButton* _nextPlayer = nullptr;
    WindowButton* _undoLastThrow = nullptr;

};

#endif // REFEREEPOPUPBUSTLEG_H
