#include "windowbutton.h"

WindowButton::WindowButton(QString text, QWidget *parent) : QPushButton(parent)
{
    int fixSizeX = 30;
    int fixSizeY = 200;

    setFixedSize(fixSizeY,fixSizeX);
    setStyleSheet("QPushButton{"
                 "color: #dcdcdc;"
                 "background-color: #333333;"
                 "font-size: 20px;"
                 "font-family: Candara;}"
                 "QPushButton:hover{"
                 "color: #dcdcdc;"
                 "background-color: #181c27;"
                 "border-width: 1px;}"
                 "QPushButton:disabled{"
                 "background-color: #acacac;}");

    setText(text);
    setToolTip(text);
}
