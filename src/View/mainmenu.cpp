/*!
* \file mainmenu.cpp
* \brief Diese Klasse ist die View Klasse des Hauptmenüs
* \author Lea Kesselmeier
*/
#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QWidget>

#include "Model/playerManagement.h"
#include "Model/sqliteConnector.h"
#include "View/mainmenu.h"
#include "View/playerManagementWindow.h"
#include "View/refereeWindow.h"
#include "View/statisticWindow.h"
#include "View/refereePopupWinningLeg.h"
#include "View/tournamentWindow.h"
#include "View/createTournamentPopup.h"
#include "View/viewerWindow.h"
#include "View/windowButton.h"
#include "View/windowLabel.h"
#include "Model/gameManagement.h"
#include "View/loadTournamentPopup.h"

#include <QDebug>
#include <QSizePolicy>

MainMenu::MainMenu(QMainWindow* parent) :
        QMainWindow(parent)
{
    _sqliteConnector = &SqliteConnector::instance();
    createWidgets();
    refreshDatabase();
    setAllLayout();
    connecting();

    setMinimumSize(700, 600);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    setButtonEnableState();
}

MainMenu::~MainMenu()
{
    delete _playerData;
    delete _loadPlayer;
    delete _newPlayer;

    delete _tournamentData;
    delete _loadTournament;
    delete _newTournament;
    delete _loadTournamentPopup;

    delete _title;

    delete _noteDatabase;
    delete _noteTournament;
    delete _tournamentName;

    delete _playermanagment;
    delete _tournament;
    delete _viewer;
    delete _referee;
}

void MainMenu::openPlayermanagmentWindow()
{
    PlayermanagementWindow* playermanagment = new PlayermanagementWindow(_playerManagementModel);
     playermanagment->showMaximized();
}

void MainMenu::openTournamentWindow()
{
    TournamentWindow* tournamentWindow = new TournamentWindow(_refereeModel, _playerManagementModel);
    tournamentWindow->showMaximized();
}

void MainMenu::openViewerWindow()
{
    ViewerWindow* viewerWindow = new ViewerWindow(_refereeModel);
    viewerWindow->showMaximized();
}

void MainMenu::openRefereeWindow()
{
    RefereeWindow* refereeWindow = new RefereeWindow(_refereeModel);
    refereeWindow->show();
}


void MainMenu::openStatisticWindow()
{
    StatisticWindow* statisticWindow = new StatisticWindow();
    statisticWindow->showMaximized();
}

/*!
 * \brief Erstellt eine Datenbank
 *
 * \param[in] void
 * \return void
 *
 * Ein Pfad wird von dem Benutzer festgelegt und dort wird eine sqlite Datenbank mit dem gewünschten Namen erzeugt
 * Wenn diese Datenbank geöffnet wurde, werden die Buttons wieder freigeschaltet und die Warnung entfernt
 */
void MainMenu::createDatabase()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                tr("Datenbank anlegen"), "",
                                                tr("Database File (*.sqlite) ;; All Files (*.*)"));

    SqliteConnector* sqliteConnector = &SqliteConnector::instance();
    sqliteConnector->createDatabase(path);

    _gameManagement->loadLastTournament();
    setTournamentName();
    setButtonEnableState();

    _noteDatabase->setVisible(false);
}

/*!
 * \brief Laden einer existierenden Datenbank
 *
 * \param[in] void
 * \return void
 *
 * Der Pfad, indem die bereits erstellten Datenbank gespeichert wurden, wird geöffnet und der Benutzer kann eine Datenbank zu öffnen auswählen
 * Wenn eine sqlite Datenbank geöffnet wurde, werden die Buttons wieder freigeschaltet und die Warnung entfernt
 */
void MainMenu::loadDatabase()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Datenbank laden"), "",
                                                tr("Database File (*.sqlite) ;; All Files (*.*)"));

    SqliteConnector* sqliteConnector = &SqliteConnector::instance();
    sqliteConnector->openDatabase(path);

    _gameManagement->loadLastTournament();
    setTournamentName();
    setButtonEnableState();
    _noteDatabase->setVisible(false);
}

void MainMenu::connecting()
{
    connect(_playermanagment, SIGNAL(released()), this, SLOT(openPlayermanagmentWindow()));
    connect(_tournament, SIGNAL(released()), this, SLOT(openTournamentWindow()));
    connect(_viewer, SIGNAL(released()), this, SLOT(openViewerWindow()));
    connect(_referee, SIGNAL(released()), this, SLOT(openRefereeWindow()));
    connect(_statitsic, SIGNAL(released()), this, SLOT(openStatisticWindow()));
    connect(_newPlayer, SIGNAL(triggered()), this, SLOT(createDatabase()));
    connect(_loadPlayer, SIGNAL(triggered()), this, SLOT(loadDatabase()));
    connect(_newTournament, SIGNAL(triggered()), this, SLOT(createTournament()));
    connect(_loadTournament, SIGNAL(triggered()), this, SLOT(loadTournament()));
    connect(_sqliteConnector, SIGNAL(databaseChanged()), this, SLOT(refreshDatabase()));
}

void MainMenu::setButtonEnableState()
{
    SqliteConnector* sqliteConnector = &SqliteConnector::instance();
    _playermanagment->setEnabled(sqliteConnector->isDatabaseOpen() && !_gameManagement->isTournamentStarted()
                                 && (_gameManagement->getTournamentId() >= 0));

    _tournamentData->setEnabled(sqliteConnector->isDatabaseOpen());
    _noteTournament->setVisible(sqliteConnector->isDatabaseOpen());
    _statitsic->setEnabled(sqliteConnector->isDatabaseOpen());

    const bool showGameButtons = sqliteConnector->isDatabaseOpen() && _gameManagement->isTournamentStarted();
    _tournament->setEnabled(showGameButtons);
    _viewer->setEnabled(showGameButtons and !_gameManagement->isTournamentFinished());
    _referee->setEnabled(showGameButtons and !_gameManagement->isTournamentFinished());
}

void MainMenu::createTournament()
{
    _tournamentName = new CreateTournamentPopUp;
    _tournamentName->show();
}

void MainMenu::loadTournament()
{
    _loadTournamentPopup = new LoadTournamentPopup;
    _loadTournamentPopup->show();
}

void MainMenu::setTournamentName()
{
    if (_gameManagement->getTournamentId() < 0)
    {
        _noteTournament->setText("Bitte zuerst ein Turnier erstellen oder Laden");
        _noteTournament->setStyleSheet("QLabel{"
                                       "font-size: 25px;"
                                       "font-family: Arial Nova Light;"
                                       "color: red;}");
    } else
    {
        _noteTournament->setText("Aktuelles Turnier " + _gameManagement->getTournamentName()
                                 + " vom " + _gameManagement->getTournamentDate());
        _noteTournament->setStyleSheet("QLabel{"
                                       "font-size: 25px;"
                                       "font-family: Arial Nova Light;"
                                       "color: black;}");
    }
}

void MainMenu::createWidgets()
{
    setWindowIcon(QIcon(":/img/darts.png"));

    _playerData = new QMenu();
    _playerData = menuBar()->addMenu(tr("Datei"));

    _loadPlayer = new QAction("Öffnen");
    _newPlayer = new QAction("Neu");

    _playerData->addAction(_newPlayer);
    _playerData->addAction(_loadPlayer);

    _tournamentData = new QMenu();
    _tournamentData = menuBar()->addMenu(tr("Turnier"));

    _loadTournament = new QAction("Öffnen");
    _newTournament = new QAction("Neu");

    _tournamentData->addAction(_newTournament);
    _tournamentData->addAction(_loadTournament);

    _tournamentData->addAction(_newTournament);
    _tournamentData->addAction(_loadTournament);


    _title = new WindowLabel("Hauptmenü - DartsTurnier");
    _title->setMainTitleStyle();

    _noteDatabase = new WindowLabel("Zu beginn muss eine Datei erstellt oder geladen werden");
    _noteDatabase->setStyleSheet("QLabel{"
                                 "font-size: 25px;"
                                 "font-family: Arial Nova Light;"
                                 "color: red;}");

    _noteTournament = new WindowLabel("Aktuelles Turnier");

    _playermanagment = new WindowButton("Meldestelle");
    _playermanagment->mainMenuStyle();
    _playermanagment->setIcon(QIcon(":/img/addPlayer.png"));
    _playermanagment->setIconSize(QSize(65, 65));

    _tournament = new WindowButton("Spielplan");
    _tournament->mainMenuStyle();
    _tournament->setIcon(QIcon(":/img/gameplan.png"));
    _tournament->setIconSize(QSize(65, 65));

    _viewer = new WindowButton("Zuschaueransicht");
    _viewer->mainMenuStyle();
    _viewer->setIcon(QIcon(":/img/viewer.png"));
    _viewer->setIconSize(QSize(60, 60));

    _referee = new WindowButton("Richteransicht");
    _referee->mainMenuStyle();
    _referee->setIcon(QIcon(":/img/referee.png"));
    _referee->setIconSize(QSize(65, 65));

    _statitsic = new WindowButton("Statistik");
    _statitsic->mainMenuStyle();
    _statitsic->setIcon(QIcon(":/img/statistic.png"));
    _statitsic->setIconSize(QSize(65, 65));

    SqliteConnector* sqlitConnector = &SqliteConnector::instance();

    _tournament->setEnabled(sqlitConnector->isDatabaseOpen());
    _playermanagment->setEnabled(sqlitConnector->isDatabaseOpen());
    _tournament->setEnabled(sqlitConnector->isDatabaseOpen());
    _viewer->setEnabled(sqlitConnector->isDatabaseOpen());
    _referee->setEnabled(sqlitConnector->isDatabaseOpen());
    _statitsic->setEnabled(sqlitConnector->isDatabaseOpen());

    if (sqlitConnector->isDatabaseOpen())
    {
        _noteDatabase->setVisible(false);
    }

}

void MainMenu::setAllLayout()
{
    QWidget* widget = new QWidget;
    setCentralWidget(widget);
    widget->setStyleSheet("background: white;");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(_title, 0, Qt::AlignCenter);
    layout->setSpacing(30);
    layout->addWidget(_noteDatabase, 0, Qt::AlignCenter);
    layout->addWidget(_noteTournament, 0, Qt::AlignCenter);
    layout->addWidget(_playermanagment, 0, Qt::AlignCenter);
    layout->addWidget(_tournament, 0, Qt::AlignCenter);
    layout->addWidget(_viewer, 0, Qt::AlignCenter);
    layout->addWidget(_referee, 0, Qt::AlignCenter);
    layout->addWidget(_statitsic, 0, Qt::AlignCenter);

    widget->setLayout(layout);
}

void MainMenu::refreshDatabase()
{
    if (!_sqliteConnector->isDatabaseOpen())
    {
        return;
    }

    setWindowTitle("Datei: " + _sqliteConnector->getDatabaseName());

    _gameManagement = &GameManagement::instance();
    connect(_gameManagement, SIGNAL(tournamentChanged()), this, SLOT(setButtonEnableState()));
    connect(_gameManagement, SIGNAL(tournamentChanged()), this, SLOT(setTournamentName()));
    setTournamentName();

    if (!_playerManagementModel)
    {
        delete _playerManagementModel;
    }
    _playerManagementModel = new PlayerManagement();

    if (!_refereeModel)
    {
        delete _refereeModel;
    }
    _refereeModel = new Referee();
    connect(_refereeModel, SIGNAL(tournamentFinished()), this, SLOT(setButtonEnableState()));
}
