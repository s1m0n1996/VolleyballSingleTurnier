#include <QHeaderView>

#include "View/tabelView.h"


/*!
 * \brief TabelView::TabelView ist Konstruktor der Klasse TabelView
 * \param[in] parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 * \return void
 *
 * Man kann Reihen makrieren, dabei werden die grün einegfärbt
 * Man kann mehrere Reihen auswählen
 */
TableView::TableView(QWidget* parent) : QTableView(parent)
{
    setAlternatingRowColors(true);
    setStyleSheet("color: black;"
                  "background-color: white;"
                  "selection-color: black;"
                  "selection-background-color: #40b946;"
                  "alternate-background-color: #F2F2F2;"
                  "background-color: white;"
                  "font: Arial");

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::MultiSelection);
    horizontalHeader()->setFont(QFont("Arial", 15));
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
    viewport()->setAttribute(Qt::WA_Hover, true);
}
