#ifndef DARTBOARD_H
#define DARTBOARD_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>
#include <QWidget>
#include <QPixmap>
#include <QResizeEvent>

class Dartboard : public QLabel
{
    Q_OBJECT
public:
    explicit Dartboard(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int x = 0;
    int y = 0;


    virtual int heightForWidth( int width ) const override;
    virtual QSize sizeHint() const override;
    QPixmap scaledPixmap() const;

public slots:
    void setPixmap ( const QPixmap & );
    void resizeEvent(QResizeEvent *)override;
signals:
    void mouseReleasedOnDartboard();
    void mousePos();

private:
    QPixmap pix;

};

#endif // DARTBOARD_H
