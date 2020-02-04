#ifndef PLAYERMANAGMENT_H
#define PLAYERMANAGMENT_H

#include <QMainWindow>
#include <QPushButton>
#include "ui_playermanagment.h"
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class Playermanagment; }
QT_END_NAMESPACE

class Playermanagment : public QMainWindow
{
    Q_OBJECT

public:
    explicit Playermanagment(QWidget *parent = nullptr);
    ~Playermanagment();


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
    Ui::Playermanagment *ui;
    QPushButton* _addPlayer = nullptr;
    QPushButton* _startTournement = nullptr;
};
#endif // PLAYERMANAGMENT_H
