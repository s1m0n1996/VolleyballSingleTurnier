/*!
* \file windowedit.cpp
* \brief Diese Klasse besitzt die Informationen zu den Edits
* \author Lea Kesselmeier
*/
#include "windowlabel.h"
/*!
 * \brief WindowLabel::WindowLabel ist Konstruktor der Klasse WindowLabel
 * \param text ist der Text, den das Label anzeigen soll, der Default ist leer
 * \param parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 * \return void
 *
 * Normalen Stlye wird im Konstsruktor festgelegt
 * verschiedene Versionen werden in Funktionen festgelegt
 */
WindowLabel::WindowLabel(QString text, QWidget *parent) : QLabel(parent)
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Candara;}");
    setText(text);
}

WindowLabel::WindowLabel(QWidget *parent, QString text) : QLabel(parent)
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

void WindowLabel::setTitleStyel()
{
    setStyleSheet("QLabel{"
                  "font-size: 45px;"
                  "font-family: Candara;"
                  "border-bottom-width: 1px;"
                  "border-bottom-style: solid;"
                  "border-radius: 0px;"
                  "font-weight: bold;}");

    setAlignment(Qt::AlignCenter);
}
