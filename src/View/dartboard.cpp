#include <QEnterEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>

#include "View/dartboard.h"
#include "View/refereeWindow.h"
#include "ui_refereewindow.h"


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
    Q_UNUSED(event);
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
    Q_UNUSED(e);
    if(!pix.isNull())
        QLabel::setPixmap(scaledPixmap());
}
