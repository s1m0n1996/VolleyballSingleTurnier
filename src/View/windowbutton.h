#ifndef WINDOWBUTTON_H
#define WINDOWBUTTON_H

#include <QPushButton>

class WindowButton : public QPushButton
{
    Q_OBJECT
public:
    explicit WindowButton(QString text, QWidget *parent = nullptr);
    void setEnableStyle(void);
    void mainMenuStyle(void);

signals:

};

#endif // WINDOWBUTTON_H
