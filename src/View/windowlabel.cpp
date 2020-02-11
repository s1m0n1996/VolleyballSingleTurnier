#include "windowlabel.h"

WindowLabel::WindowLabel(QString text, QWidget *parent) : QLabel(parent)
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;}");
    setText(text);
}
