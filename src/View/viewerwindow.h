#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QMainWindow>

#include "Model/referee.h"

namespace Ui {
class ViewerWindow;
}
class ViewerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ViewerWindow(Referee* referee ,QWidget *parent = nullptr);

public slots:
    void writeScore();

private:
    Ui::ViewerWindow *ui;
    Referee* _referee;
    int _player1 = 0;
    int _player2 = 0;
    int _aktivePlayer = 0;

};

#endif // VIEWERWINDOW_H
