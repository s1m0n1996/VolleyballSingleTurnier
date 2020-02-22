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
                  "font-family: Candara;"
                  "border-width: 20px;}"
                  "QPushButton:hover{"
                  "color: #dcdcdc;"
                  "background-color: #181c27;"
                  "border-width: 1px;}"
                  "QPushButton:disabled{"
                  "background-color: #acacac;}");

    setText(text);
    setToolTip(text);


}

void WindowButton::setEnableStyle()
{

    setStyleSheet("QPushButton{"
                  "color: #dcdcdc;"
                  "background-color: #40b946;"
                  "font-size: 20px;"
                  "font-family: Candara;}"
                  "QPushButton:disabled{"
                  "background-color: #acacac;}"
                  "QPushButton:hover{"
                  "color: #dcdcdc;"
                  "background-color: #339436;"
                  "font-size: 20px;"
                  "font-family: Candara;}");
}

void WindowButton::mainMenuStyle()
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

    setEnabled(false);
}
