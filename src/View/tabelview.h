#ifndef TABELVIEW_H
#define TABELVIEW_H

#include <QWidget>
#include <QTableView>

class TableView : public QTableView
{
    Q_OBJECT
public:
    explicit TableView(QWidget *parent = nullptr);

signals:

public slots:
};
#endif // TABELVIEW_H
