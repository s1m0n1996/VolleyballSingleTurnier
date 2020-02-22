#ifndef WINDOWEDIT_H
#define WINDOWEDIT_H

#include <QLineEdit>
/*!
 * \file windowedit.h
 * \brief Die Klasse WondoEdit erzeugt Edits
 * \author Lea Kesselmeier
 *
 * Jedes Edit bekommt das selbe StyleSheet und die selbe Größe beim Erzeugen
 * Wenn nur ein Name vom Benutzter eingegeben werden darf, können nur Buchstaben und Leerzeichen eingegebn werden
 * Wenn nur ein Datum vom Benutzter eingegeben werden darf, können nur Nummern und Querstriche eingegeben werden
*/
class WindowEdit: public QLineEdit
{
public:
    WindowEdit(QString placeholder, QWidget *parent = nullptr);
};

#endif // WINDOWEDIT_H
