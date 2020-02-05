#ifndef REFEREEWINDOW_H
#define REFEREEWINDOW_H

#include <QMainWindow>
#include "Model/referee.h"

namespace Ui {
class RefereeWindow;
}

class RefereeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RefereeWindow(QWidget *parent = nullptr);
    ~RefereeWindow();

private slots:
    void mouseReleasedOnDartboard();
    void on_naechsterSpieler_released();
    void on_undoLetzterWurf_released();
    void remainScore();

    int valueMultiplikator();
    int valueScoreWithoutMultiplikator();

private:
    Ui::RefereeWindow *ui;
    Referee* referee = new Referee;
    const int _miss                  = 0;
    const int _single                = 1;
    const int _double                = 2;
    const int _trible                = 3;
    int _throwScore                  = 0;
    int _centralPointXY              = 0;
    int _throwCounter                = 0;
    int _radius                      = 0;
    int _numberOfSubtraction         = 3;
    int _remainScore[2]      = {501,501};
    int _player                      = 0;



};

#endif // REFEREEWINDOW_H



