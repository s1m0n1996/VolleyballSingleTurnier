#include <QMouseEvent>
#include "dartboard.h"
#include "View/refereewindow.h"
#include "ui_refereewindow.h"
#include <QImage>
#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QEnterEvent>


Dartboard::Dartboard(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("border:none");
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

