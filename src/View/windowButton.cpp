/*!
* \file mainmenu.cpp
* \brief Diese Klasse ist die View Klasse des Hauptmenüs
* \author Lea Kesselmeier
*/

#include "windowButton.h"

/*!
 * \brief WindowButton::WindowButton ist Konstruktor der Klasse WindowButtons
 * \param text ist der Text, den der Button tragen soll, der Defalut ist leer
 * \param parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 * \return void
 *
 */
WindowButton::WindowButton(QString text, QWidget* parent) : QPushButton(parent)
{
    int fixSizeX = 30;
    int fixSizeY = 200;

    setFixedSize(fixSizeY,fixSizeX);

    setStyleSheet("QPushButton{"
                  "color: #dcdcdc;"
                  "background-color: #333333;"
                  "font-size: 20px;"
                  "font-family: Candara;"
                  "border-width: 20px;}"

                  "QPushButton:hover{"
                  "color: #dcdcdc;"
                  "background-color: #181c27;"
                  "border-width: 1px;}"

                  "QPushButton:disabled{"
                  "background-color: #acacac;}"
                  );

    setText(text);
    setToolTip(text);

}


WindowButton::WindowButton(QWidget* parent,QString text) : QPushButton(parent)
{
    int fixSizeX = 35;
    int fixSizeY = 225;

    setFixedSize(fixSizeY,fixSizeX);

    setStyleSheet("QPushButton{"
                  "color: #dcdcdc;"
                  "background-color: #333333;"
                  "font-size: 20px;"
                  "font-family: Candara;"
                  "border-width: 20px;}"

                  "QPushButton:hover{"
                  "color: #dcdcdc;"
                  "background-color: #181c27;"
                  "border-width: 1px;}"

                  "QPushButton:disabled{"
                  "background-color: #acacac;}"
                  );

    setText(text);
    setToolTip(text);

}
/*!
 * \brief setzt das StyleSheet, wenn der Button nicht drüchbar ist
 * \param void
 * \return void
 *
 */
void WindowButton::setEnableStyle()
{
    setStyleSheet("QPushButton{"
                  "color: #dcdcdc;"
                  "background-color: #40b946;"
                  "font-size: 20px;"
                  "font-family: Candara;}"

                  "QPushButton:hover{"
                  "color: #dcdcdc;"
                  "background-color: #339436;"
                  "font-size: 20px;"
                  "font-family: Candara;}"

                  "QPushButton:disabled{"
                  "background-color: #acacac;}"
                  );
}

/*!
 * \brief setzt Größe, StyleSheet und nicht drückbar für die MainMenuButtons
 * \param void
 * \return void
 *
 */
void WindowButton::mainMenuStyle()
{
    int fixSizeX = 50;
    int fixSizeY = 250;

    setFixedSize(fixSizeY,fixSizeX);

    setStyleSheet("QPushButton{"
                  "color: #dcdcdc;"
                  "background-color: #333333;"
                  "font-size: 22px;"
                  "font-family: Candara;"
                  "font: bold;}"

                  "QPushButton:hover{"
                  "color: #dcdcdc;"
                  "background-color: #414141;"
                  "border-width: 1px;}"

                  "QPushButton:disabled{"
                  "background-color: #acacac;}"
                  );

    setEnabled(false);
}
