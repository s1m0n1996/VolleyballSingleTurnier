#ifndef VIEWER_H
#define VIEWER_H

#include <QObject>

class Viewer : public QObject
{
    Q_OBJECT
public:
    explicit Viewer(QObject *parent = nullptr);

signals:

};

#endif // VIEWER_H
