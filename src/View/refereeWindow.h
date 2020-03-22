/*!
* \file refereewindow.h
* \brief Diese Klasse ist das View für den Richter
* \author Nico Meyer
*/

#ifndef REFEREEWINDOW_H
#define REFEREEWINDOW_H

#include <QMainWindow>

#include "Model/referee.h"

#include "View/refereePopupWinningLeg.h"
#include "View/refereePopupBustLeg.h"
class Viewer;
class WinnerPopup;


QT_BEGIN_NAMESPACE
namespace Ui { class RefereeWindow; }
QT_END_NAMESPACE


/*!
* \file refereewindow.cpp
* \brief Diese Klasse ist die View Klasse des Richters
* \author Nico Meyer
*
* Es wird ein Fenster erstellt, wo der Richter auf einer Dartscheibe den geworfenen Wurf
* anklicken kann. Es ist möglich diesen Wurf wieder rückgängig zumachen.
*
*/
class RefereeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RefereeWindow(Referee* referee, RefereepopupWinningLeg* popupWinningLeg = nullptr,
                           RefereePopupBustLeg* popupBustLeg = nullptr, QWidget *parent = nullptr);
    ~RefereeWindow(void);

private slots:
    void mouseCurrentPos(void);
    void mouseReleasedOnDartboard(void);
    void writeScore(void);
    void nextPlayer(void);
    void undoLastThrow(void);
    void writeNextPlayer(void);
    void playerBust(void);
    void playerWinsLeg(void);
    void nextPlayerAfterWinningLeg(void);
    void tournamentIsWon(void);


private:
    void connecting(void);
    void modifiWidgets(void);

    int valueMultiplikator(void);
    int valueScoreWithoutMultiplikator(void);

    Ui::RefereeWindow *ui;
    Referee* _referee;
    Viewer* _viewer;
    RefereepopupWinningLeg* _popupWinningLeg;
    RefereePopupBustLeg* _popupBustLeg;
    WinnerPopup* _winnerPopup;
    GameManagement* _gameManagement;


    const int _miss                     = 0;
    const int _single                   = 1;
    const int _double                   = 2;
    const int _trible                   = 3;
    int _centralPointXY                 = 0;
    int _radius                         = 0;
    int _valueMultiplikator             = 0;
    int _valueScoreWithoutMultiplikator = 0;

};

#endif // REFEREEWINDOW_H



