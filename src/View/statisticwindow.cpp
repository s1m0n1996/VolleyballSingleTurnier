#include "statisticwindow.h"
#include <QIcon>
#include <QLabel>

#include <QHeaderView>
#include <QAction>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QMenu>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QWidget>

#include "View/windowbutton.h"
#include "View/windowedit.h"
#include "View/windowlabel.h"

StatisticWindow::StatisticWindow( QWidget *parent):
 QWidget(parent)
{
    setWindowTitle("Statistik");
    setWindowIcon(QIcon(":/img/statistic.png"));

    _colorLabel         = new QLabel;
    _colorLabel->setStyleSheet("background-color:#550000;");
    _title = new WindowLabel("Statistik");
    _title->setTitleStyel();
    _name1 = new WindowLabel("Name1");
    _wert1 = new WindowLabel("Wert1");

    _name2 = new WindowLabel("Name2");
    _wert2 = new WindowLabel("Wert3");

    _name3 = new WindowLabel("Name3");
    _wert3 = new WindowLabel("Wert3");


    QVBoxLayout* mainLayout             = new QVBoxLayout;
    QGridLayout* labelLayout            = new QGridLayout;


    mainLayout->addWidget(_colorLabel);
    mainLayout->addWidget(_title);
    mainLayout->setSpacing(100);



    labelLayout->addWidget(_name1,0,0);
    labelLayout->addWidget(_wert1,0,1);

    labelLayout->addWidget(_name2,1,0);
    labelLayout->addWidget(_wert2,1,1);

    labelLayout->addWidget(_name3,2,0);
    labelLayout->addWidget(_wert3,2,1);

    mainLayout->addLayout(labelLayout);


    setLayout(mainLayout);

}
