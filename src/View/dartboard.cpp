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
    setStyleSheet("border:1px solid white");

    setScaledContents(true);
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





void Dartboard::setPixmap ( const QPixmap & p)
{
    pix = p;
    QLabel::setPixmap(scaledPixmap());
}

int Dartboard::heightForWidth( int width ) const
{
    return pix.isNull() ? this->height() : ((qreal)pix.height()*width)/pix.width();
}

QSize Dartboard::sizeHint() const
{
    int w = this->width();
    return QSize( w, heightForWidth(w) );
}

QPixmap Dartboard::scaledPixmap() const
{
    return pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void Dartboard::resizeEvent(QResizeEvent * e)
{
    if(!pix.isNull())
        QLabel::setPixmap(scaledPixmap());
}