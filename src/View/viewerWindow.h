#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QJsonObject>
#include <QFile>
#include <QGraphicsScene>


#include "Model/referee.h"
#include "Model/viewer.h"
#include "View/dartboardViewer.h"

namespace Ui {
class ViewerWindow;
}
class ViewerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ViewerWindow(Referee* referee, Viewer* viewer = nullptr, QWidget *parent = nullptr);
    ~ViewerWindow();

public slots:
    void writeScore(void);
    void scoreIsUnder170InLeg(void);
    void possibleWayToFinishLeg(void);
    void remainingThrowsAreZeroInLeg(void);

private:
    Ui::ViewerWindow *ui;
    Referee* _referee;
    Viewer* _viewer;
    QList<QString> _listWithHowToFinishLeg;
    QGraphicsScene* scene = new QGraphicsScene;

    DartboardViewer* _dartboard = new DartboardViewer(_referee);

    void connecting(void);
    void setPhoto(Player playerA, Player playerB);
    void setAktionPlayer1(void);
    void setAktionPlayer2(void);
    void modifiWidgets(void);

    int _player1 = 0;
    int _player2 = 0;
    int _aktivePlayer = 0;

};

#endif // VIEWERWINDOW_H
