/*!
* \file windowedit.cpp
* \brief Diese Klasse besitzt die Informationen zu den Edits
* \author Lea Kesselmeier
*/

#include <QRegExp>
#include <QRegExpValidator>

#include "View/windowEdit.h"


/*!
 * \brief WindowEdit::WindowEdit ist Konstruktor der Klasse WindowEdit
 * \param[in] placeholder ist der Text, den das Edit anzeigen soll, der Default ist leer
 * \param[in] parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 * \return void
 *
 * Je nachdem welches playeholder es ist, können nur bestimmte Zeichen eingegben werden, um Fehler zu minimieren
 */
WindowEdit::WindowEdit(QString placeholder, DataType dataType, QWidget* parent) : QLineEdit(parent)
{
    setPlaceholderText(placeholder);
    setFixedWidth(300);

    setStyleSheet("QLineEdit{"
               "font-size: 20px;"
               "font-family: Arial Nova Light;}");

    QRegExpValidator* validator = new QRegExpValidator();

    if (dataType == DataType::name)
    {
        QRegExp re("[a-zA-Z ]*");
        validator->setRegExp(re);
    }
    else if (dataType == DataType::country)
    {
        QRegExp re("[a-zA-Z]*");
        validator->setRegExp(re);
    }
    else if (dataType == DataType::date)
    {
        QRegExp re(R"(^\d{4}\-(0[1-9]|1[012])\-(0[1-9]|[12][0-9]|3[01])$)");
        validator->setRegExp(re);
    }

    setValidator(validator);
}

void WindowEdit::setTournamentPopUpStyle()
{
    QRegExpValidator* validator = new QRegExpValidator();
    QRegExp re("[a-zA-Z 0-9]*");
    validator->setRegExp(re);
    setValidator(validator);
}
