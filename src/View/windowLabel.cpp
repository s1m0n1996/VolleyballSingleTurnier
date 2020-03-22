#include "windowLabel.h"

/*!
 * \brief WindowLabel::WindowLabel ist Konstruktor der Klasse WindowLabel
 * \param[in] text ist der Text, den das Label anzeigen soll, der Default ist leer
 * \param[in] parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 * \return void
 *
 * Normalen Stlye wird im Konstsruktor festgelegt.
 * verschiedene Versionen werden in Funktionen festgelegt.
 */
WindowLabel::WindowLabel(QString text, QWidget* parent) : QLabel(parent)
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Arial Nova Light;}");
    setText(text);
}

/*!
 * \brief WindowLabel::WindowLabel ist Konstruktor der Klasse WindowLabel
 * \param[in] parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 * \param[in] text ist der Text, den das Label anzeigen soll, der Default ist leer
 * \return void
 *
 *Dieser kosntruktor wurde für die Widgets, die in der ui erzeugt werden erstellt, denn diese haben eines parent
 */
WindowLabel::WindowLabel(QWidget* parent, QString text) : QLabel(parent)
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Arial Nova Light;}");
    setText(text);
}

void WindowLabel::setNotStartTournamentStyle(void)
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Arial Nova Light;"
                  "color: red;}");
}

void WindowLabel::setStartTournamentStyle(void)
{
    setStyleSheet("QLabel{"
                  "font-size: 30px;"
                  "font-family: Arial Nova Light;}");
}

void WindowLabel::setTitleStyle(void)
{
    setStyleSheet("QLabel{"
                  "font-size: 30px;"
                  "font-family: Arial Nova Light;}");

    setAlignment(Qt::AlignCenter);

}

void WindowLabel::setMainTitleStyle(void)
{
    setStyleSheet("QLabel{"
                  "font-size: 55px;"
                  "font-family: Arial Nova Light;"
                  "border-bottom-width: 1px;"
                  "border-bottom-style: solid;"
                  "border-radius: 0px;"
                  "font-weight: bold;}");

    setAlignment(Qt::AlignCenter);

}

void WindowLabel::setBold(void)
{
    setStyleSheet("QLabel{"
                  "font-size: 25px;"
                  "font-family: Arial Nova Light;"
                  "font-weight: bold;}");
}

void WindowLabel::setWinnerStyle(void)
{
    setStyleSheet("QLabel{"
                  "font-size: 40px;"
                  "font-family: Candara;"
                  "font-weight: bold;}");

}
