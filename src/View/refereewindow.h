#ifndef REFEREEWINDOW_H
#define REFEREEWINDOW_H

#include <QMainWindow>

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
    void mouseCurrentPos();
    void mouseReleasedOnDartboard();
    void on_naechsterSpieler_released();
    void on_undoLetzterWurf_released();

    int valueMultiplikator();
    int valueScoreWithoutMultiplikator();

private:
    Ui::RefereeWindow *ui;
    const int _miss                  = 0;
    const int _single                = 1;
    const int _double                = 2;
    const int _trible                = 3;
    int _throwScore                  = 0;
    int _mittelpunktXY               = 0;
    int _throwCounter                = 0;
    int _radius                      = 0;



};

#endif // REFEREEWINDOW_H



