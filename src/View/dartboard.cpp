#include <QMouseEvent>
#include "dartboard.h"
#include "ui_dialogdartboard.h"
#include <QImage>
#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QEnterEvent>


Dartboard::Dartboard(QWidget *parent) : QLabel(parent)
{
}

void Dartboard::mouseMoveEvent(QMouseEvent *event)
{
    x = event->x();
    y = event->y();
    emit mousePos();
}

void Dartboard::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleasedOnDartboard();
}

