#include "tabelview.h"
#include <QHeaderView>

TableView::TableView(QWidget* parent) : QTableView(parent)
{


     setAlternatingRowColors(true);
     setStyleSheet("color: black;"
                   "background-color: white;"
                   "selection-color: black;"
                   "selection-background-color: #40b946;"
                   "alternate-background-color: #F2F2F2;"
                   "background-color: white;"
                   "font: Candara");

     setSelectionBehavior(QAbstractItemView::SelectRows);
     setSelectionMode(QAbstractItemView::MultiSelection);
     //selectionModel()->selectedRows();
     horizontalHeader()->setFont(QFont("Candara", 15));




}
