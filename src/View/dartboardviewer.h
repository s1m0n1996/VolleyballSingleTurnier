#ifndef DARTBOARDVIEWER_H
#define DARTBOARDVIEWER_H

#include <QGraphicsItem>
#include "Model/referee.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class DartboardViewer: public QGraphicsObject
{
public:
    DartboardViewer(Referee* referee, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    Referee* _referee;
    QList<int> _allThrows = {0,0,0,0};
};

#endif // DARTBOARDVIEWER_H
