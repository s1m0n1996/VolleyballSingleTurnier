/*!
* \file refereewindow.h
* \brief Diese Klasse ist das View für den Richter
* \author Nico Meyer
*/


#ifndef REFEREEWINDOW_H
#define REFEREEWINDOW_H

#include <QMainWindow>
#include "Model/referee.h"
#include "Model/viewer.h"
#include "View/refereePopupWinningLeg.h"
#include "View/refereePopupBustLeg.h"
#include "View/winnerpopup.h"


QT_BEGIN_NAMESPACE
namespace Ui { class RefereeWindow; }
QT_END_NAMESPACE

class RefereeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RefereeWindow(Referee* referee, RefereepopupWinningLeg* popupWinningLeg = nullptr,
                           RefereePopupBustLeg* popupBustLeg = nullptr, QWidget *parent = nullptr);
    ~RefereeWindow();

private slots:
    void mouseCurrentPos();
    void mouseReleasedOnDartboard();
    void writeScore();
    void nextPlayer();
    void undoLastThrow();
    void writeNextPlayer();
    void playerBust();
    void playerWinsLeg();
    void nextPlayerAfterWinningLeg();
    void tournamentIsWon();


private:
    Ui::RefereeWindow *ui;
    Referee* _referee;
    Viewer* _viewer;
    RefereepopupWinningLeg* _popupWinningLeg;
    RefereePopupBustLeg* _popupBustLeg;
    WinnerPopup* _winnerPopup;
    GameManagement* _gameManagement;
    void connecting(void);
    void modifiWidgets(void);

    int valueMultiplikator();
    int valueScoreWithoutMultiplikator();

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



