#ifndef TABELVIEW_H
#define TABELVIEW_H

#include <QWidget>
#include <QTableView>
/*!
 * \file tabelview.h
 * \brief Die Klasse TabelView erzeugt TabelViews
 * \author Lea Kesselmeier
 *
 * Erzeugt einheiltiche TabelViews mit Möglichkeiten von markierung mehrer Reihen und deren Farbwechsel
*/
class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget *parent = nullptr);

signals:

public slots:
};
#endif // TABELVIEW_H
