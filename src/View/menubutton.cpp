/*!
* \file menubutton.cpp
* \brief Diese Klasse ist beihnaltet die Informationen zu den Menübutton
* \author Lea Kesselmeier
*/
#include "menubutton.h"


MenuButton::MenuButton(QString text, QWidget *parent) : QPushButton(parent)
{

    int fixSizeX = 50;
    int fixSizeY = 250;

    setFixedSize(fixSizeY,fixSizeX);
    setStyleSheet("QPushButton:disabled{"
                  "background-color: #acacac;}"
                  "QPushButton:!hover{"
                   "color: #dcdcdc;"
                   "background-color: #333333;"
                   "font-size: 22px;"
                   "font-family: Candara;"
                   "font: bold;}"
                   "QPushButton:hover{"
                   "color: #dcdcdc;"
                   "background-color: #414141;"
                   "border-width: 1px;}");

    setText(text);
    setToolTip(text);
    setEnabled(false);

}


