#include "windowlabel.h"

WindowLabel::WindowLabel(QString text, QWidget *parent) : QLabel(parent)
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;}");
    setText(text);
}

void WindowLabel:: setNotStartTournamentStyle()
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;"
                  "color: red;}");
}

void WindowLabel:: setStartTournamentStyle()
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;}");
}
