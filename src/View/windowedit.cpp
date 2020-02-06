#include "windowedit.h"

WindowEdit::WindowEdit(QString text, QWidget *parent) : QLineEdit(parent)
{
    setPlaceholderText(text);
    setFixedWidth(300);
    setStyleSheet("QLineEdit{"
               "font-size: 20px;"
               "font-family: Candara;}");

}
