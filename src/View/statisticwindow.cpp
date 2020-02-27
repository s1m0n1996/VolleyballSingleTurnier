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
#include <QComboBox>
#include <QRadioButton>
#include <QList>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_CHARTS_USE_NAMESPACE

#include "View/windowbutton.h"
#include "View/windowedit.h"
#include "View/windowlabel.h"
#include "Model/playermanagement.h"
#include "Model/player.h"
#include "Model/playerstatistics.h"

StatisticWindow::StatisticWindow( QWidget *parent):
 QMainWindow(parent)
{
    setWindowTitle("Statistik");
    setWindowIcon(QIcon(":/img/statistic.png"));
    createWidgets();
    setLayout();
    connect();


}


void StatisticWindow::createWidgets(void)
{
    _colorLabel         = new QLabel;
    _colorLabel->setStyleSheet("background-color:#550000;");
    _title = new WindowLabel("Statistik");
    _title->setTitleStyel();

    _choosePlayername = new WindowLabel("Wähle einen Spieler");
    _comboBox = new QComboBox;
    _comboBox->setMaximumHeight(50);
    _comboBox->setMinimumWidth(300);
    _comboBox->setStyleSheet("QComboBox{"
                             "font-size: 20px;"
                             "font-family: Candara;}");

    _playerstatistic  = new Playerstatistics;
    _playermanagment = new PlayerManagement;


    _categorie = new WindowLabel("Wähle eine Kategorie");
    _winners = new QRadioButton("Gewinner");
    _winners->setStyleSheet("font-size: 20px;"
                       "font-family: Candara;");
    _winners->setChecked(true);
    _singlePoint = new QRadioButton("Single Würfe");
    _singlePoint->setStyleSheet("font-size: 20px;"
                       "font-family: Candara;");
    _doublePoint = new QRadioButton("Double Würfel");
    _doublePoint->setStyleSheet("font-size: 20px;"
                       "font-family: Candara;");
    _triplePoint = new QRadioButton("Triple Würfe");
    _triplePoint->setStyleSheet("font-size: 20px;"
                       "font-family: Candara;");
    _average     = new QRadioButton("Gewinner");
    _average->setStyleSheet("font-size: 20px;"
                       "font-family: Candara;");



    QList<Player> playerList = _playermanagment->getAllStoredPlayers();

    for (Player& player : playerList)
    {
        const QString name = player.getName();
        const QString birthday = player.getBirthday().toString("yyyy-MM-dd");
        const QString country = player.getCountry();
        _comboBox->addItem(name + "         " + birthday+"         " + country);
    }
}

void StatisticWindow::setLayout(void)
{

    QWidget* widget = new QWidget;
    setCentralWidget(widget);
    QVBoxLayout* mainLayout             = new QVBoxLayout;
    QGridLayout* chartLayout            = new QGridLayout;
    QVBoxLayout* chooseLayout           = new QVBoxLayout;
    QChartView *chartView;

    chartView = new QChartView(createChart());


    mainLayout->addWidget(_colorLabel);
    mainLayout->addWidget(_title);

    chartLayout->addWidget(chartView,0,1);

    chooseLayout->addWidget(_choosePlayername);
    chooseLayout->addWidget(_comboBox);
    chooseLayout->addStretch(1);
    chooseLayout->addWidget(_categorie);
    chooseLayout->addWidget(_winners);
    chooseLayout->addWidget(_singlePoint);
    chooseLayout->addWidget(_doublePoint);
    chooseLayout->addWidget(_triplePoint);
    chooseLayout->addWidget(_average);
    chooseLayout->addStretch(1);



    chartLayout->addLayout(chooseLayout,0,0);

    mainLayout->addLayout(chartLayout);

    widget->setLayout(mainLayout);
}

void StatisticWindow::connect(void)
{
    QObject::connect(_winners,SIGNAL(clicked()),this,SLOT(showWinnerChart()));
    QObject::connect(_singlePoint,SIGNAL(clicked()),this,SLOT(showSingleChart()));
    QObject::connect(_doublePoint,SIGNAL(clicked()),this,SLOT(showDoubleChart()));
    QObject::connect(_triplePoint,SIGNAL(clicked()),this,SLOT(showTripleChart()));
    QObject::connect(_average,SIGNAL(clicked()),this,SLOT(showAverageChart()));

    QObject::connect(_comboBox,SIGNAL(currentTextChanged(const QString &)), this, SLOT(getPlayerList(const QString &)));

}

void StatisticWindow::getPlayerList(const QString &text)
{
    qDebug() << text;
    QStringList parameterliste = text.split("         ");
    qDebug()<<parameterliste[1];
    Player player(parameterliste[0], QDate::fromString(parameterliste[1],"yyyy-MM-dd"),parameterliste[2]);
    _listWinner = _playerstatistic->gamesWonAndLost(player);
    _listeAverage = _playerstatistic->averageOfAllLegs(player);

}
void StatisticWindow::loadAllPlayersFromDatabase(void)
{

}

QChart* StatisticWindow::createChart() const
{
//    _playerstatistic
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");

    QPieSeries *series = new QPieSeries(chart);
    series->append("Gewonnen", 30);
    series->append("Verloren", 3);


    series->setPieSize(0.4);
    chart->addSeries(series);

    return chart;
}

void StatisticWindow::showWinnerChart(void)
{

}
void StatisticWindow::showSingleChart(void)
{

}
void StatisticWindow::showDoubleChart(void)
{

}
void StatisticWindow::showTripleChart(void)
{

}
void StatisticWindow::showAverageChart(void)
{

}
