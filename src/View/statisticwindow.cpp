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
#include <QDateTime>
#include <QSplineSeries>
#include <QValueAxis>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>

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
    _playerStatistic = new PlayerStatistics;
    _playerManagement = new PlayerManagement;
    _gameManagement = &GameManagement::instance();

    setWindowTitle("Statistik");
    setWindowIcon(QIcon(":/img/statistic.png"));
    _createWidgets();
    _setLayout();
    _connect();

    // test pie

    QPieSeries* series = new QPieSeries(_chart);

    series->setPieSize(1);

    _dataChangesDetected();
}

/*!
 * \brief Erstelle benötigte Widgets
 *
 * In dieser Methode werden alle benötigten widgets erstellt.
 */
void StatisticWindow::_createWidgets(void)
{
    _colorLabel = new QLabel;
    _colorLabel->setStyleSheet("background-color:#550000;");
    _title = new WindowLabel("Statistik");
    _title->setMainTitleStyle();

    _chart = new DrilldownChart();
    _chart->setAnimationOptions(QChart::SeriesAnimations);
    _chart->legend()->setMarkerShape(QLegend::MarkerShapeRectangle);
    _chart->legend()->setVisible(true);
    _chart->legend()->setAlignment(Qt::AlignRight);
    _chart->series().setSharable(true);
}

/*!
 * \brief Setze layout aller widgets
 *
 * In dieser Methode werden alle Layouts gesetzt.
 */
void StatisticWindow::_setLayout(void)
{

    QWidget* widget = new QWidget;
    setCentralWidget(widget);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QGridLayout* chartLayout = new QGridLayout;
    QVBoxLayout* chooseLayout = new QVBoxLayout;
    QChartView* chartView;

    chartView = new QChartView(_chart);
    chartView->setRenderHint(QPainter::Antialiasing);


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

/*!
 * \brief Connect Signale und Slots
 *
 * In dieser Methode werden alle Signale und Slots verbunden.
 */
void StatisticWindow::_connect(void)
{
    connect(_chooseCategoryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(_dataChangesDetected()));
    connect(_choosePlayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(_dataChangesDetected()));
    connect(_chooseTournamentComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(_dataChangesDetected()));
}

/*!
 * \brief Erstelle Dropdown Menü um die Daten auszuwählen
 *
 * In dieser Methode wird ein Drop down Menü in einer Group box erstellt, um eine Karegory der
 * angezeigten Daten auszuwählen
 */
QGroupBox* StatisticWindow::_createSelectCategoryGroupBox(void)
{
    QGroupBox* groupBox = new QGroupBox(tr("Wähle eine kategorie"));
    _chooseCategoryComboBox = new QComboBox;
    _chooseCategoryComboBox->setMinimumWidth(400);

    _chooseCategoryComboBox->addItem("Gewinner");
    _chooseCategoryComboBox->addItem("Single Würfe");
    _chooseCategoryComboBox->addItem("Double Würfe");
    _chooseCategoryComboBox->addItem("Triple Würfe");
    _chooseCategoryComboBox->addItem("Wurfhistorie");
    _chooseCategoryComboBox->addItem("Am meisten getroffenes Feld");
    _chooseCategoryComboBox->addItem("Average");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(_chooseCategoryComboBox);

    groupBox->setLayout(layout);

    return groupBox;
}

/*!
 * \brief Erstelle Dropdown Menü um die Spieler auszuwählen
 *
 * In dieser Methode wird ein Drop down Menü in einer Group box erstellt, um einen Spieler auszuwählen
 */
QGroupBox* StatisticWindow::_createSelectPlayerGroupBox(void)
{
    QGroupBox* groupBox = new QGroupBox(tr("Wähle einen Spieler"));
    _choosePlayerComboBox = new QComboBox;

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

/*!
 * \brief Erstelle Dropdown Menü um nach einem Turnier zu Filtern
 *
 * In dieser Methode wird ein Drop down Menü in einer Group box erstellt, um die Daten weiter zu filtern.
 */
QGroupBox* StatisticWindow::_createFilterGroupBox(void)
{
    QGroupBox* groupBox = new QGroupBox(tr("Wähle ein Turnier aus"));

    // create tournament combo box
    _chooseTournamentComboBox = new QComboBox;
    _chooseTournamentComboBox->setMinimumWidth(400);

    QStandardItemModel* model = new QStandardItemModel(0, 3);

    model->appendRow(new QStandardItem("Turnier Auswählen"));

    QList<QList<QString>> tournaments = _gameManagement->getSavedTournaments();
    for (QList<QString> tournament : tournaments)
    {
        QList<QStandardItem*> tournamentItems;
        tournamentItems.append(new QStandardItem(tournament[0]));
        tournamentItems.append(new QStandardItem(tournament[1]));
        tournamentItems.append(new QStandardItem(tournament[2]));

        model->appendRow(tournamentItems);
    }

    QTableView* tableView = new QTableView();
    tableView->setModel(model);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->horizontalHeader()->hide();
    tableView->verticalHeader()->hide();
    tableView->hideColumn(2);

    _chooseTournamentComboBox->setModel(model);
    _chooseTournamentComboBox->setView(tableView);


    /*// create game combo box
    _chooseGameComboBox = new QComboBox;
    _chooseGameComboBox->setMinimumWidth(400);

    _chooseGameComboBox->addItem("Spiel Auswählen");
    */

    // add combo boxes to group box
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(_chooseTournamentComboBox);
    // layout->addWidget(_chooseGameComboBox);

    groupBox->setLayout(layout);

    return groupBox;
}

// #####################################################################################################################
// combo box changed
//
// hier werden die drop down menüs nach ihrem aktuellen wert gefragt
// #####################################################################################################################

/*!
 * \brief Aktualisiere aktuell ausgewählten Spieler
 *
 * In dieser Methode wird die Membervariable des aktuellen Spielers  Aktualisiert.
 * Es wird der wert des Drop down Menüs in die Membervariable geschrieben.
 */
void StatisticWindow::_refreshSelectedPlayer()
{
    if (_selectedPlayer)
    {
        delete _selectedPlayer;
        _selectedPlayer = nullptr;
    }

    // no player selected
    if (_choosePlayerComboBox->currentIndex() == 0)
    {
        return;
    }

    QAbstractItemModel* playerModel = _choosePlayerComboBox->view()->model();

    _selectedPlayer = new Player(
            playerModel->index(_choosePlayerComboBox->currentIndex(), 0).data().toString(),
            playerModel->index(_choosePlayerComboBox->currentIndex(), 1).data().toDate(),
            playerModel->index(_choosePlayerComboBox->currentIndex(), 2).data().toString()
    );
}

/*!
 * \brief Aktueller Diagrammtyp
 *
 * \return Es wird die aktuell ausgewählte Kategorie als ENUM zurückgegeben
 *
 * In dieser Methode wird die aktuell ausgewählte kategorie des Drop down Menüs als ENUM zurückgegeben.
 */
statistic::type StatisticWindow::_getSelectedChartType(void)
{
    if (_chooseCategoryComboBox->currentText() == "Gewinner")
    {
        return statistic::type::Winner;
    }
    else if (_chooseCategoryComboBox->currentText() == "Single Würfe")
    {
        return statistic::type::Singles;
    }
    else if (_chooseCategoryComboBox->currentText() == "Double Würfe")
    {
        return statistic::type::Doubles;
    }
    else if (_chooseCategoryComboBox->currentText() == "Triple Würfe")
    {
        return statistic::type::Triples;
    }
    else if (_chooseCategoryComboBox->currentText() == "Wurfhistorie")
    {
        return statistic::type::History;
    }
    else if (_chooseCategoryComboBox->currentText() == "Am meisten getroffenes Feld")
    {
        return statistic::type::MostHittingField;
    }
    else if (_chooseCategoryComboBox->currentText() == "Average")
    {
        return statistic::type::Average;
    }
}

/*!
 * \brief Aktuell ausgewählte Turnier Id
 *
 * \return es Wird die aktuell ausgewählte Turnier Id zurückgegeben
 *
 * In dieser Methode wird die aktuell ausgewählte Turnier Id des Drop down Menüs zurückgegeben.
 */
int StatisticWindow::_getSelectedTournamentId()
{
    // no tournament selected
    if (_chooseTournamentComboBox->currentIndex() == 0)
    {
        return -1;
    }

    QAbstractItemModel* tournamentModel = _chooseTournamentComboBox->view()->model();

    return tournamentModel->index(_chooseTournamentComboBox->currentIndex(), 2).data().toInt();
}

// #####################################################################################################################
// refresh diagram
// #####################################################################################################################
/*!
 * \brief Aktualisiere Torten Diagramm
 *
 * \param[in] diagramData Map mit dem Diagrammdaten
 *
 * In dieser Methode wird das Tortendiagramm aktualisiert. Dabei dient der key, der eingegebenen Map als Beschriftung
 * und der dazugehärige Wert repräsentiert den Wert des Diagramms.
 */
void StatisticWindow::_refreshPieDiagram(const QMap<QString, double>& diagramData)
{
    QPieSeries* series = new QPieSeries(_chart);

    for (QString key : diagramData.keys())
    {
        *series << new DrilldownSlice(diagramData[key], key, series);
    }

    series->setPieSize(0.7);

    _chart->removeAllSeries();
    _chart->addSeries(series);
}

/*!
 * \brief Aktualisiere Linien Diagramm mit mehreren Linien
 *
 * \param[in] diagramData Map mit dem Diagrammdaten
 *
 * In dieser Methode wird das Liniendiagramm aktualisiert. Dabei dient der key, der eingegebenen Map als Beschriftung
 * und der dazugehärige Wert repräsentiert den Wert des Diagramms.
 */
void StatisticWindow::_refreshLineDiagram(const QMap<int, QMap<QDateTime, double>>& diagramData)
{
    int maxXPoints = 0;
    for (int& playerId : diagramData.keys())
    {
        QLineSeries* series = new QLineSeries;
        _seriesList.append(series);

        _chart->addSeries(series);

        int i = 0;
        for (QDateTime time : diagramData[playerId].keys())
        {
            series->append(QPoint(i, diagramData[playerId][time]));
            series->setName(Player(playerId).getName());
            if (i > maxXPoints)
            {
                maxXPoints = i;
            }
            i++;
        }
    }

    _chart->createDefaultAxes();
    _chart->axes(Qt::Vertical).first()->setRange(0, 60);
    _chart->axes(Qt::Horizontal).first()->setRange(0, maxXPoints);
}

// #####################################################################################################################
// detect data change
// #####################################################################################################################

/*!
 * \brief Änderung erkannt
 *
 * Diese Methode ist verbunden mit allen drop down Menüs. Wenn ein Wert geändert wird wird diese Methode aufgerufen.
 * Es werden alle drop down Menüs abgefragt und das Diagramm wird neu erstellt.
 */
void StatisticWindow::_dataChangesDetected(void)
{
    _chart->removeAllSeries();

    _refreshSelectedPlayer();

    switch (_getSelectedChartType())
    {
        case statistic::type::Winner:
            showWinnerChart();
            break;
        case statistic::type::Singles:
            showSingleChart();
            break;
        case statistic::type::Doubles:
            showDoubleChart();
            break;
        case statistic::type::Triples:
            showTripleChart();
            break;
        case statistic::type::History:
            showThrowHistoryChart();
            break;
        case statistic::type::MostHittingField:
            showMostHittingFieldsDiagram();
            break;
        case statistic::type::Average:
            showAverage();
            break;
    }
}

// #####################################################################################################################
// all diagram types
// #####################################################################################################################
/*!
 * \brief Zeige Gewinner Statistik
 *
 * In dieser Methode werden die passenden Daten der gewinner Statistik geholt.
 */
void StatisticWindow::showWinnerChart(void)
{
    if (!_selectedPlayer && _getSelectedTournamentId() < 0)
    {
        _refreshPieDiagram(_playerStatistic->getWinningStatistic());
    }
    else if (_selectedPlayer && _getSelectedTournamentId() < 0)
    {
        _refreshPieDiagram(_playerStatistic->getWinningStatistic(_selectedPlayer));
    }
    else if (!_selectedPlayer && _getSelectedTournamentId() >= 0)
    {
        _refreshPieDiagram(_playerStatistic->getWinningStatistic(_getSelectedTournamentId()));
    }
    else if (_selectedPlayer && _getSelectedTournamentId() >= 0)
    {
        _refreshPieDiagram((_playerStatistic->getWinningStatistic(_selectedPlayer, _getSelectedTournamentId())));
    }
}

/*!
 * \brief Zeige Single Statistik
 *
 * In dieser Methode werden die passenden Daten der gewinner Statistik geholt.
 */
void StatisticWindow::showSingleChart(void)
{

}

/*!
 * \brief Zeige Double Statistik
 *
 * In dieser Methode werden die passenden Daten der Double Statistik geholt.
 */
void StatisticWindow::showDoubleChart(void)
{

}

/*!
 * \brief Zeige Triple Statistik
 *
 * In dieser Methode werden die passenden Daten der Triple Statistik geholt.
 */
void StatisticWindow::showTripleChart(void)
{

}

/*!
 * \brief Zeige Historie der geworfenen Punkte
 *
 * In dieser Methode werden die passenden Daten der Average Statistik geholt.
 */
void StatisticWindow::showThrowHistoryChart(void)
{
    _chart->removeAllSeries();
    _chart->setTitle("Punkte Historie");

    if (!_selectedPlayer && _getSelectedTournamentId() < 0)
    {
        _refreshLineDiagram(_playerStatistic->getThrowHistory());
    }
    else if (_selectedPlayer && _getSelectedTournamentId() < 0)
    {
        _refreshLineDiagram(_playerStatistic->getThrowHistory(_selectedPlayer));
    }
    else if (!_selectedPlayer && _getSelectedTournamentId() >= 0)
    {
        _refreshLineDiagram(_playerStatistic->getThrowHistory(_getSelectedTournamentId()));
    }
    else if (_selectedPlayer && _getSelectedTournamentId() >= 0)
    {
        _refreshLineDiagram(_playerStatistic->getThrowHistory(_selectedPlayer, _getSelectedTournamentId()));
    }
}

void StatisticWindow::showMostHittingFieldsDiagram(void)
{
    _chart->setTitle("die 10 am Meinsten geworfenen Felder");

    if (!_selectedPlayer && _getSelectedTournamentId() < 0)
    {
        _refreshPieDiagram(_playerStatistic->get10MostHittingFields());
    }
    else if (_selectedPlayer && _getSelectedTournamentId() < 0)
    {
        _refreshPieDiagram(_playerStatistic->get10MostHittingFields(_selectedPlayer));
    }
    else if (!_selectedPlayer && _getSelectedTournamentId() >= 0)
    {
        _refreshPieDiagram(_playerStatistic->get10MostHittingFields(_getSelectedTournamentId()));
    }
    else if (_selectedPlayer && _getSelectedTournamentId() >= 0)
    {
        _refreshPieDiagram(_playerStatistic->get10MostHittingFields(_selectedPlayer, _getSelectedTournamentId()));
    }
}

void StatisticWindow::showAverage(void)
{
    _chart->setTitle("Average");

    if (!_selectedPlayer && _getSelectedTournamentId() < 0)
    {
        _refreshPieDiagram(_playerStatistic->getAverage());
    }
    else if (_selectedPlayer && _getSelectedTournamentId() < 0)
    {
        _refreshPieDiagram(_playerStatistic->getAverage(_selectedPlayer));
    }
    else if (!_selectedPlayer && _getSelectedTournamentId() >= 0)
    {
        _refreshPieDiagram(_playerStatistic->getAverage(_getSelectedTournamentId()));
    }
    else if (_selectedPlayer && _getSelectedTournamentId() >= 0)
    {
        _refreshPieDiagram(_playerStatistic->getAverage(_selectedPlayer, _getSelectedTournamentId()));
    }
}
