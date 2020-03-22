#ifndef DARTBOARDVIEWER_H
#define DARTBOARDVIEWER_H

#include <QGraphicsItem>
#include "Model/referee.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

/*!
 * \file dartboardviewer.cpp
 * \author Nico Meyer
 * \brief Eine Klasse für die Dartscheibe beim Zuschauer
 *
 * Wenn vom Model ein Signal kommt das etwas geworfen wurde, dann färbt sich
 * die Dartscheibe an der Stelle ein.
 *
*/
class DartboardViewer: public QGraphicsObject
{
public:
    DartboardViewer(Referee* referee, QGraphicsItem *parent = nullptr);
    QRectF boundingRect(void) const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    Referee* _referee;
    QList<int> _allThrows = {0,0,0,0};
    QList<int> _allMulti = {-1,-1,-1,-1};
};

#endif // DARTBOARDVIEWER_H
