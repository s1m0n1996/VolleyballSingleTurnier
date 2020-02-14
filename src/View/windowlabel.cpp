#include "windowlabel.h"

WindowLabel::WindowLabel(QString text, QWidget *parent) : QLabel(parent)
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;}");
    setText(text);
}

void WindowLabel:: setNotStartTournementStyle()
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;"
                  "color: red;}");
}

void WindowLabel:: setStartTournementStyle()
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;}");
}
