#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QWidget>
#include <QPushButton>

class MenuButton : public QPushButton
{
public:
     explicit MenuButton(QString text, QWidget *parent = nullptr);
};

#endif // MENUBUTTON_H
