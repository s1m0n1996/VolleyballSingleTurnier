#ifndef WINDOWBUTTON_H
#define WINDOWBUTTON_H

#include <QPushButton>

class WindowButton : public QPushButton
{
    Q_OBJECT
public:
    explicit WindowButton(QString text, QWidget *parent = nullptr);
    void setEnableStyle();

signals:

};

#endif // WINDOWBUTTON_H
