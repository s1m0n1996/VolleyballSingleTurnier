#ifndef DARTBOARD_H
#define DARTBOARD_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>
#include <QWidget>

class Dartboard : public QLabel
{
    Q_OBJECT
public:
    explicit Dartboard(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int x = 0;
    int y = 0;

signals:
    void mouseReleasedOnDartboard();
    void mousePos();

private:


};

#endif // DARTBOARD_H
