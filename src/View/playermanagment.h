#ifndef PLAYERMANAGMENT_H
#define PLAYERMANAGMENT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Playermanagment; }
QT_END_NAMESPACE

class Playermanagment : public QMainWindow
{
    Q_OBJECT

public:
    Playermanagment(QWidget *parent = nullptr);
    ~Playermanagment();
    //void addPlayer(string playername);

signals:

private slots:
   void setMaxPlayerLabel(int playerNumber);

private:
    Ui::Playermanagment *ui;
};
#endif // PLAYERMANAGMENT_H
