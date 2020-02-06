#ifndef PLAYERMANAGMENT_H
#define PLAYERMANAGMENT_H

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include "ui_playermanagementWindow.h"
#include "windowbutton.h"
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
    void createAddPlayerEdit();
    void createButtons(void);
    void setButtonLayout(void);
    Ui::PlayermanagementWindow *ui;
    WindowButton* _addPlayerButton = nullptr;
    WindowButton* _startTournementButton = nullptr;
    QLabel* _playernameLabel = nullptr;
    QLabel* _bithdayLabel = nullptr;
    QLabel* _countryLabel = nullptr;
    QLineEdit* _playernameEdit = nullptr;
    QLineEdit* _birthdayEdit = nullptr;
    QLineEdit* _countryEdit = nullptr;
};
#endif // PLAYERMANAGMENT_H
