#include "windowlabel.h"

WindowLabel::WindowLabel(QString text, QWidget *parent) : QLabel(parent)
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;}");
    setText(text);
}

void WindowLabel::setNotStartTournamentStyle()
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;"
                  "color: red;}");
}

void WindowLabel::setStartTournamentStyle()
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;}");
}

void WindowLabel::titleStyel()
{
    setStyleSheet("QLabel{"
                  "font-size: 45px;"
                  "font-family: Candara;"
                  "border-bottom-width: 1px;"
                  "border-bottom-style: solid;"
                  "border-radius: 0px;"
                  "font-weight: bold;}");
}
