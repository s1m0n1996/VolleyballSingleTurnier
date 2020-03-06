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
#include <QGroupBox>
#include <QStandardItem>
#include <QTableView>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

#include "View/windowbutton.h"
#include "View/windowedit.h"
#include "View/windowlabel.h"
#include "Model/playermanagement.h"
#include "Model/player.h"
#include "Model/playerstatistics.h"

StatisticWindow::StatisticWindow(QWidget* parent) :
        QMainWindow(parent)
{
    _playerStatistic = new Playerstatistics;
    _playerManagement = new PlayerManagement;

    setWindowTitle("Statistik");
    setWindowIcon(QIcon(":/img/statistic.png"));
    _createWidgets();
    _setLayout();
    _connect();

    // test pie

    QPieSeries* series = new QPieSeries(_chart);

    series->append("Spieler 1", 10);
    series->append("Spieler 2", 20);
    series->append("Spieler 3", 40);
    series->append("Spieler 4", 50);
    series->append("Spieler 5", 34);

    series->setPieSize(1);

    _chart->removeAllSeries();
    _chart->addSeries(series);
}

void StatisticWindow::_createWidgets(void)
{
    _colorLabel = new QLabel;
    _colorLabel->setStyleSheet("background-color:#550000;");
    _title = new WindowLabel("Statistik");
    _title->setMainTitleStyle();

    _choosePlayername = new WindowLabel("Wähle einen Spieler");
    _comboBox = new QComboBox;
    _comboBox->setMaximumHeight(50);
    _comboBox->setMinimumWidth(400);
    _comboBox->setStyleSheet("QComboBox{"
                             "font-size: 20px;"
                             "font-family: Candara;}");

    _chart = new QChart();
    _chart->setAnimationOptions(QChart::SeriesAnimations);
}

void StatisticWindow::_setLayout(void)
{

    QWidget* widget = new QWidget;
    setCentralWidget(widget);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QGridLayout* chartLayout = new QGridLayout;
    QVBoxLayout* chooseLayout = new QVBoxLayout;
    QChartView* chartView;

    chartView = new QChartView(_chart);


    mainLayout->addWidget(_colorLabel);
    mainLayout->addWidget(_title);

    chartLayout->addWidget(chartView, 0, 1);

    chooseLayout->addWidget(_createSelectCategoryGroupBox());
    chooseLayout->addWidget(_createSelectPlayerGroupBox());
    chooseLayout->addWidget(_createFilterGroupBox());

    chooseLayout->addStretch(1);

    chartLayout->addLayout(chooseLayout, 0, 0);

    mainLayout->addLayout(chartLayout);

    widget->setLayout(mainLayout);
}

void StatisticWindow::_connect(void)
{
    /*
    QObject::connect(_winners, SIGNAL(clicked()), this, SLOT(showWinnerChart()));
    QObject::connect(_singlePoint, SIGNAL(clicked()), this, SLOT(showSingleChart()));
    QObject::connect(_doublePoint, SIGNAL(clicked()), this, SLOT(showDoubleChart()));
    QObject::connect(_triplePoint, SIGNAL(clicked()), this, SLOT(showTripleChart()));
    QObject::connect(_average, SIGNAL(clicked()), this, SLOT(showAverageChart()));

    QObject::_connect(_comboBox, SIGNAL(currentTextChanged(
                                               const QString &)), this, SLOT(getPlayerList(
                                                                                     const QString &)));
*/
}

void StatisticWindow::_dataChangesDetected()
{
    /*
    if (_winners->isChecked())
    {
        showWinnerChart();
    } else if (_singlePoint->isChecked())
    {
        showSingleChart();
    } else if (_doublePoint->isChecked())
    {
        showDoubleChart();
    } else if (_triplePoint)
    {
        showTripleChart();
    }
     */
}

/*
void StatisticWindow::createChart() const
{
    QStringList parameterliste = _comboBox->currentText().split("         ");
    Player player(parameterliste[0], QDate::fromString(parameterliste[1], "yyyy-MM-dd"), parameterliste[2]);
    qDebug() << parameterliste;
    qDebug() << _playerStatistic->gamesWonAndLost(player);
    QList<int> _list = _playerStatistic->gamesWonAndLost(player);

    QPieSeries* series = new QPieSeries(_chart);
    series->append("Gewonnen", _list[0] + 1);
    series->append("Verloren", _list[1] + 2);

    series->setPieSize(0.4);
    _chart->addSeries(series);
}*/

Player StatisticWindow::_getSelectedPlayer()
{
    QStringList playerParameters = _comboBox->currentText().split("         ");
    return Player(playerParameters[0], QDate::fromString(playerParameters[1], "yyyy-MM-dd"), playerParameters[2]);
}

QGroupBox* StatisticWindow::_createSelectCategoryGroupBox(void)
{
    QGroupBox* groupBox = new QGroupBox(tr("Wähle eine kategorie"));
    _chooseCategoryComboBox = new QComboBox;
    _chooseCategoryComboBox->setMinimumWidth(400);

    _chooseCategoryComboBox->addItem("Gewinner");
    _chooseCategoryComboBox->addItem("Single Würfe");
    _chooseCategoryComboBox->addItem("Double Würfe");
    _chooseCategoryComboBox->addItem("Triple Würfe");
    _chooseCategoryComboBox->addItem("Average");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(_chooseCategoryComboBox);

    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox* StatisticWindow::_createSelectPlayerGroupBox(void)
{
    QGroupBox* groupBox = new QGroupBox(tr("Wähle einen Spieler"));
    _choosePlayerComboBox = new QComboBox;
    //_choosePlayerComboBox->addItem("Alle Spieler");


    //QStandardItemModel *model = new QStandardItemModel( 0, 2);
    QStandardItemModel* model = new QStandardItemModel(0, 3);

    model->appendRow(new QStandardItem("Alle Spieler"));

    QList<Player> availablePlayers = _playerManagement->getAllStoredPlayers();
    for (Player& player : availablePlayers)
    {
        QList<QStandardItem*> playerItems;
        playerItems.append(new QStandardItem(player.getName()));
        playerItems.append(new QStandardItem(player.getBirthday().toString("yyyy-MM-dd")));
        playerItems.append(new QStandardItem(player.getCountry()));

        model->appendRow(playerItems);
    }

    QTableView* tableView = new QTableView();
    tableView->setModel(model);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->horizontalHeader()->hide();
    tableView->verticalHeader()->hide();

    _choosePlayerComboBox->setModel(model);
    _choosePlayerComboBox->setView(tableView);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(_choosePlayerComboBox);

    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox* StatisticWindow::_createFilterGroupBox(void)
{
    QGroupBox* groupBox = new QGroupBox(tr("Filter"));

    // create tournament combo box
    _chooseTournamentComboBox = new QComboBox;
    _chooseTournamentComboBox->setMinimumWidth(400);

    _chooseTournamentComboBox->addItem("Turnier Auswählen");

    // create game combo box
    _chooseGameComboBox = new QComboBox;
    _chooseGameComboBox->setMinimumWidth(400);

    _chooseGameComboBox->addItem("Turnier Auswählen");

    // add combo boxes to group box
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(_chooseTournamentComboBox);
    layout->addWidget(_chooseGameComboBox);

    groupBox->setLayout(layout);

    return groupBox;
}

void StatisticWindow::_refreshDiagram(const QMap<QString, int> diagramData)
{
    QPieSeries* series = new QPieSeries(_chart);

    for (QString key : diagramData.keys())
    {
        series->append(key, diagramData[key]);
    }

    series->setPieSize(1);

    _chart->removeAllSeries();
    _chart->addSeries(series);
}

// #####################################################################################################################
// all diagram types
// #####################################################################################################################
void StatisticWindow::showWinnerChart(void)
{
    _refreshDiagram(_playerStatistic->gamesWonAndLost(_getSelectedPlayer()));
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
    /*
    _chart->removeAllSeries();
    _chart->setTitle("Durchschnittspunktzahl");

    QLineSeries* series = new QLineSeries(_chart);

    if (_selectLegRadio->isChecked())
    {

    }
    series->append(0, 1);
    series->append(1, 2);
    series->append(2, 3);

    _chart->addSeries(series);
     */
}