#include "windowedit.h"
#include <QRegExp>
#include <QRegExpValidator>

WindowEdit::WindowEdit(QString placeholder, QWidget *parent) : QLineEdit(parent)
{
    setPlaceholderText(placeholder);
    setFixedWidth(300);
    setStyleSheet("QLineEdit{"
               "font-size: 20px;"
               "font-family: Candara;}");
    QRegularExpression nummber ("\\d");
    QRegularExpression letter ("[a-zA-Z]");
    QRegExpValidator* validator = new QRegExpValidator();



    if (placeholder == "Max Mustermann")
    {
        QRegExp re("[a-zA-Z ]*");
        validator->setRegExp(re);
    }
    else if (placeholder == "Deutschland")
    {
        QRegExp re("[a-zA-Z]*");
        validator->setRegExp(re);
    }
    else if (placeholder == "1990-01-30")
    {
        QRegExp re("(\\d{4})(\\-)(\\d{2})(\\-)(\\d{2})");
        validator->setRegExp(re);
        //CAN: Abfrage anch realem Datum
    }
    setValidator(validator);


}
