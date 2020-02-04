#include "menubutton.h"


MenuButton::MenuButton(QString text, QWidget *parent) : QPushButton(parent)
{

    int fixSizeX = 50;
    int fixSizeY = 250;    

    setFixedSize(fixSizeY,fixSizeX);
    setStyleSheet("QPushButton:!pressed{"
                 "color: #dcdcdc;"
                 "background-color: #3f3f3f;"
                 "font-size: 20px;"
                 "font-family: Bahnschrift Light;"
                 "font: bold};"
                 "QPushButton:hover{"
                 "color: #dcdcdc;"
                 "background-color: #515151;"
                 "border-width: 1px};");

    setText(text);
    setToolTip(text);



}
