#ifndef DARTBOARD_H
#define DARTBOARD_H

#include <QLabel>
#include <QPixmap>
#include <QWidget>


/*!
 * \file dartboard.cpp
 * \author Nico Meyer
 * \brief Eine Klasse für die Dartscheibe beim Richter
 *
 * Zeigt eine normale Dartscheibe ein. Auf dieser Dartscheibe kann mit der Maus geklickt
 * werden und anhand der Mausposition wird dann der geworfene Wert des Dartpfeils ermittelt
 * und zurückgegebn. Die Dartscheibe lässt sich automatisch skalieren.
 *
*/
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
