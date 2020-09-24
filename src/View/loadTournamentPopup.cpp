#include <QComboBox>
#include <QHeaderView>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QTableView>

#include "Model/gameManagement.h"

#include "View/loadTournamentPopup.h"
#include "View/windowButton.h"
#include "View/windowLabel.h"

/*!
 * \brief Konstruktor
 * \param[in] parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 *
 */
LoadTournamentPopup::LoadTournamentPopup()
{
    _gameManagement = &GameManagement::instance();
    _createWidgets();
    _setAllLayout();
    _connect();
}

/*!
 * \brief Destruktor
 *
 * Löscht alle erzeugten objekte auf dem Heap.
 */
LoadTournamentPopup::~LoadTournamentPopup()
{
    delete _comboBox;
    delete _informationLabel;
    delete _loadButton;
};

/*!
 * \brief Erstelle widgets
 *
 * In dieser Methode werden alle widget objekte erzeugt.
 */
void LoadTournamentPopup::_createWidgets(void)
{
    setWindowTitle("Turnier laden");
    setWindowIcon(QIcon(":/img/favicon.png"));

    _informationLabel = new WindowLabel("Bitte ein Turnier auswählen");
    _informationLabel->setTitleStyle();
    _informationLabel->setMaximumHeight(100);

    _comboBox = new QComboBox;
    _comboBox->setMinimumHeight(50);
    _comboBox->setMaximumHeight(100);
    _comboBox->setMinimumWidth(300);
    _comboBox->setStyleSheet("QComboBox{"
                             "font-size: 30px;"
                             "font-family: Arial;}");

    _loadButton = new WindowButton("Turnier laden");


    _loadAllTournamentsFromDatabase();
}

/*!
 * \brief Setze Layout
 *
 * In dieser Methode wird das Layout für das komplette Fester gesetzt.
 */
void LoadTournamentPopup::_setAllLayout(void)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_informationLabel);
    mainLayout->addWidget(_comboBox);

    QHBoxLayout* footerLayout = new QHBoxLayout;
    footerLayout->setSpacing(1);
    footerLayout->addWidget(_loadButton, 0, Qt::AlignRight);

    mainLayout->addLayout(footerLayout);
    QSpacerItem* test = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->addSpacerItem(test);

    setLayout(mainLayout);

    setFixedWidth(size().width());

}

/*!
 * \brief Verbinde
 *
 * In dieser Methode werden buttons mit funktionen verbunden
 */
void LoadTournamentPopup::_connect(void)
{
    connect(_loadButton, SIGNAL(released()), this, SLOT(_loadTournamentForGame()));
}

/*!
 * \brief Lade alle Tourniere der Aktuellen aus der Datenbank
 *
 * In dieser Methode werden alle verfügbaren Tourniere der aktuellen Sportart geladen
 * und der comboBox hinzugefügt
 */
void LoadTournamentPopup::_loadAllTournamentsFromDatabase(void)
{
    QStandardItemModel* model = new QStandardItemModel(0, 3);

    QList<QList<QString>> savedTournaments = _gameManagement->getSavedTournaments();

    for (QList<QString>& tournament : savedTournaments)
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
    tableView->setShowGrid(false);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    _comboBox->setModel(model);
    _comboBox->setView(tableView);
}

/*!
 * \brief Lade ausgewähltes Tournier
 *
 * In dieser Methode wird das ausgewählte Turnier geladen.
 * Dadurch werden die Membervariablen der Singleton-Klasse GameboardManagement mit dem des Aktuellen Toutnieres
 * überschrieben, und das Programm holt sich die Daten für das jetzt ausgewählte Tournier.
 */
void LoadTournamentPopup::_loadTournamentForGame(void)
{
    QAbstractItemModel* tournamentModel = _comboBox->view()->model();

    _gameManagement->loadOtherTournament(tournamentModel->index(_comboBox->currentIndex(), 2).data().toInt());
    window()->close();
}
