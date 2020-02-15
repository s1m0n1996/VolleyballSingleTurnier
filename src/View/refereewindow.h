#ifndef REFEREEWINDOW_H
#define REFEREEWINDOW_H

#include <QMainWindow>
#include "Model/referee.h"
#include "View/refereepopupwinningleg.h"
#include "View/refereepopupbustleg.h"


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

public slots:
    int valueMultiplikator();
    int valueScoreWithoutMultiplikator();
    void mouseCurrentPos();
    void mouseReleasedOnDartboard();
    void writeScore();
    void nextPlayer();
    void undoLastThrow();
    void writeNextPlayer();
    void gameStart();
    void playerBust();
    void playerWinsLeg();



private:
    Ui::RefereeWindow *ui;
    Referee* _referee;
    RefereepopupWinningLeg* _popupWinningLeg;
    RefereePopupBustLeg* _popupBustLeg;
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



