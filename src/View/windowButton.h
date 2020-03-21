#ifndef WINDOWBUTTON_H
#define WINDOWBUTTON_H

#include <QPushButton>

/*!
 * \file windowbutton.h
 * \brief Die Klasse WindowButton erzeugt PushButtons
 * \author Lea Kesselmeier
 *
 * Jeder Button bekommt das selbe StyleSheet beim Erzeugen
 * Wenn der Button nicht drückbar sein soll, bekommt er ein anders StyleSheet
 * Wenn der Button ein Menü Button sein soll, bekommt er ein aderes StylseSheet und eine andere Größe
*/
class WindowButton : public QPushButton
{
    Q_OBJECT
public:
    WindowButton(QString text, QWidget *parent = nullptr);
    WindowButton(QWidget *parent,QString text = "");

    void setEnableStyle(void);
    void mainMenuStyle(void);

signals:

};

#endif // WINDOWBUTTON_H
