/*!
* \file windowedit.cpp
* \brief Diese Klasse besitzt die Informationen zu den Edits
* \author Lea Kesselmeier
*/
#include "windowedit.h"
#include <QRegExp>
#include <QRegExpValidator>

/*!
 * \brief WindowEdit::WindowEdit ist Konstruktor der Klasse WindowEdit
 * \param placeholder ist der Text, den das Edit anzeigen soll, der Default ist leer
 * \param parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 * \return void
 *
 * Je nachdem welches playeholder es ist, können nur bestimmte Zeichen eingegben werden, um Fehler zu minimieren
 */
WindowEdit::WindowEdit(QString placeholder, QWidget* parent) : QLineEdit(parent)
{
    setPlaceholderText(placeholder);
    setFixedWidth(300);

    setStyleSheet("QLineEdit{"
               "font-size: 20px;"
               "font-family: Candara;}");

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
        QRegExp re(R"(^\d{4}\-(0[1-9]|1[012])\-(0[1-9]|[12][0-9]|3[01])$)");
        validator->setRegExp(re);
        //CAN: Abfrage anch realem Datum
    }

    setValidator(validator);
}
