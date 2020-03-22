#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QMainWindow>
#include <QObject>

class DartboardViewer;
class Player;
class QGraphicsScene;
class Referee;
class Viewer;

namespace Ui {
class ViewerWindow;
}
class ViewerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ViewerWindow(Referee* referee, Viewer* viewer = nullptr, QWidget *parent = nullptr);
    ~ViewerWindow(void);

public slots:
    void writeScore(void);
    void scoreIsUnder170InLeg(void);
    void possibleWayToFinishLeg(void);
    void remainingThrowsAreZeroInLeg(void);

private:
    void connecting(void);
    void setPhoto(Player playerA, Player playerB);
    void setAktionPlayer1(void);
    void setAktionPlayer2(void);
    void modifiWidgets(void);

    Ui::ViewerWindow *ui;
    Referee* _referee;
    Viewer* _viewer;
    QList<QString> _listWithHowToFinishLeg;
    QGraphicsScene* _scene = nullptr;
    DartboardViewer* _dartboard = nullptr;

    int _player1 = 0;
    int _player2 = 0;
    int _aktivePlayer = 0;

};

#endif // VIEWERWINDOW_H
