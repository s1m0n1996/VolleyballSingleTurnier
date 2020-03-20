#ifndef DARTBOARD_H
#define DARTBOARD_H

#include <QLabel>
#include <QPixmap>
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

    virtual int heightForWidth( int width ) const override;
    virtual QSize sizeHint(void) const override;
    QPixmap scaledPixmap(void) const;

public slots:
    void setPixmap (const QPixmap &);
    void resizeEvent(QResizeEvent *)override;
signals:
    void mouseReleasedOnDartboard(void);
    void mousePos(void);

private:
    QPixmap pix;

};

#endif // DARTBOARD_H
