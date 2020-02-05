#ifndef PLAYERMANAGMENT_H
#define PLAYERMANAGMENT_H

#include <QMainWindow>
#include <QPushButton>
#include "ui_playermanagementWindow.h"
#include <cmath>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class PlayermanagementWindow; }
QT_END_NAMESPACE

class PlayermanagementWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayermanagementWindow(QWidget *parent = nullptr);
    ~PlayermanagementWindow();


signals:
public slots:
    void addPlayer(void);
    void setMaxPlayerLabel(void);   
    void test(void);


private slots:


private:
    void connecting();
    void createButtons(void);
    void setButtonLayout(void);
    Ui::PlayermanagementWindow *ui;
    QPushButton* _addPlayer = nullptr;
    QPushButton* _startTournement = nullptr;
};
#endif // PLAYERMANAGMENT_H
