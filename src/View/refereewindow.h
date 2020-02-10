#ifndef REFEREEWINDOW_H
#define REFEREEWINDOW_H

#include <QMainWindow>
#include "Model/referee.h"


QT_BEGIN_NAMESPACE
namespace Ui { class RefereeWindow; }
QT_END_NAMESPACE

class RefereeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RefereeWindow(Referee* referee, QWidget *parent = nullptr);
    ~RefereeWindow();

public slots:
    int valueMultiplikator();
    int valueScoreWithoutMultiplikator();
    void mouseReleasedOnDartboard();
    void writeScore();
    void nextPlayer();
    void undoLastThrow();



private:
    Ui::RefereeWindow *ui;
    Referee* _referee                = nullptr;
    const int _miss                  = 0;
    const int _single                = 1;
    const int _double                = 2;
    const int _trible                = 3;
    int _centralPointXY              = 0;
    int _radius                      = 0;

};

#endif // REFEREEWINDOW_H



