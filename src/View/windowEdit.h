#ifndef WINDOWEDIT_H
#define WINDOWEDIT_H

#include <QLineEdit>

enum DataType {name, country, date};

/*!
 * \file windowedit.h
 * \brief Die Klasse WindoEdit erzeugt Edits
 * \author Lea Kesselmeier
 *
 * Jedes Edit bekommt das selbe StyleSheet und die selbe Größe beim Erzeugen
 * Wenn nur ein Name vom Benutzter eingegeben werden darf, können nur Buchstaben und Leerzeichen eingegebn werden
 * Wenn nur ein Land vom Benutzter eingegeben werden darf, können nur Buchstaben und Leerzeichen eingegebn werden
 * Wenn nur ein Datum vom Benutzter eingegeben werden darf, können nur Nummern und Querstriche eingegeben werden
 */
class WindowEdit: public QLineEdit
{
public:
    WindowEdit(QString placeholder, DataType dataType, QWidget *parent = nullptr);

    void setTournamentPopUpStyle(void);
};

#endif // WINDOWEDIT_H
