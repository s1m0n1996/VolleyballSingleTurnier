#ifndef WINDOWLABEL_H
#define WINDOWLABEL_H

#include <QLabel>

class WindowLabel: public QLabel
{
    Q_OBJECT
public:
    explicit WindowLabel(QString text, QWidget *parent = nullptr);
    void setNotStartTournementStyle(void);
    void setStartTournementStyle(void);


public:
    WindowLabel();
};

#endif // WINDOWLABEL_H
