#ifndef DARTBOARD_H
#define DARTBOARD_H

#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class Dartboard : public QLabel
{
    Q_OBJECT
public:
    explicit Dartboard(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void valueRingOfDartboard();
    void valueSectionOfDartboard();
    void fieldValue();

    int x, y;
    int throwCounter = 0;

signals:
    void mouseReleasedOnDartboard();
    void mousePos();

private:
    int _diogonal = 0;


};

#endif // DARTBOARD_H
