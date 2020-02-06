#include "windowlabel.h"

WindowLabel::WindowLabel(QString text, QWidget *parent) : QLabel(parent)
{
    setStyleSheet("QLabel{"
                  "font-size: 20px;"
                  "font-family: Candara;}");
    setText(text);
}
