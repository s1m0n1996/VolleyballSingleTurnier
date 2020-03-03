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

#include "Model/model.h"
#include "Model/playermanagement.h"
#include "Model/sqliteConnector.h"
#include "View/mainmenu.h"
#include "View/playermanagementWindow.h"
#include "View/refereewindow.h"
#include "View/statisticwindow.h"
#include "View/refereepopupwinningleg.h"
#include "View/tournamentwindow.h"
#include "View/tournamentnamepopup.h"
#include "View/viewerwindow.h"
#include "View/windowbutton.h"
#include "View/windowlabel.h"
#include "Model/gameManagement.h"
#include "View/loadTournamentPopup.h"

#include <QDebug>
#include <QSizePolicy>

MainMenu::MainMenu(Model* model, QMainWindow* parent) :
    QMainWindow(parent),
    _model(model)
{
    _gameManagement = &GameManagement::instance();
    createWidgets();
    setwholeLayout();
    connecting();

    setMinimumSize(700,600);
    setWindowFlags(Qt::WindowMinimizeButtonHint| Qt::WindowCloseButtonHint);
    setTournamentName();
    setButtonEnableState();
}

MainMenu::~MainMenu()
{
    delete  _playerData;
    delete  _loadPlayer;
    delete  _newPlayer;

    delete  _tournamentData;
    delete  _loadTournament;
    delete  _newTournament;
    delete _loadTournamentPopup;

    delete  _title;

    delete  _noteDatabase;
    delete  _noteTournament;
    delete  _tournamentName;

    delete  _playermanagment;
    delete  _tournament;
    delete  _viewer;
    delete  _referee;
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
 * \param void
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
 * \param void
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

void MainMenu:: connecting()
{
    connect(_playermanagment, SIGNAL(released()), this, SLOT(openPlayermanagmentWindow()));
    connect(_tournament, SIGNAL(released()), this, SLOT(openTournamentWindow()));
    connect(_viewer, SIGNAL(released()), this, SLOT(openViewerWindow()));
    connect(_referee, SIGNAL(released()), this, SLOT(openRefereeWindow()));    
    connect(_statitsic,SIGNAL(released()), this , SLOT(openStatisticWindow()));
    connect(_newPlayer,SIGNAL(triggered()),this, SLOT(createDatabase()));
    connect(_loadPlayer,SIGNAL(triggered()),this, SLOT(loadDatabase()));
    connect(_newTournament,SIGNAL(triggered()),this,SLOT(createTournament()));
    connect(_loadTournament, SIGNAL(triggered()), this, SLOT(loadTournament()));
    connect(_gameManagement, SIGNAL(tournamentChanged()), this, SLOT(setTournamentName()));
    connect(_gameManagement, SIGNAL(tournamentChanged()), this, SLOT(setButtonEnableState()));
}

void MainMenu::setButtonEnableState()
{
    SqliteConnector* sqliteConnector = &SqliteConnector::instance();
    _playermanagment->setEnabled(sqliteConnector->getDb()->isOpen() && !_gameManagement->isTournamentStarted()
    && (_gameManagement->getTournamentId() >= 0));


    bool showGameButtons = sqliteConnector->getDb()->isOpen() && _gameManagement->isTournamentStarted();
    _tournament->setEnabled(showGameButtons);
    _viewer->setEnabled(showGameButtons);
    _referee->setEnabled(showGameButtons);
}

void MainMenu::createTournament()
{
    _tournamentName = new TournamentNamePopUp;
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
                                                     "font-family: Candara;"
                                                     "color: red;}");
    }
    else
    {
        _noteTournament->setText("Aktuelles Turnier " + _gameManagement->getTournamentName()
                                 + " vom " + _gameManagement->getTournamentDate());
        _noteTournament->setStyleSheet("QLabel{"
                                       "font-size: 25px;"
                                       "font-family: Candara;"
                                       "color: black;}");
    }
}

void MainMenu::createWidgets()
{
    setWindowTitle("Hauptmenü");
    setWindowIcon(QIcon(":/img/darts.png"));

    _playerData = new QMenu();
    _playerData = menuBar()->addMenu(tr("Datei"));

    _loadPlayer = new QAction("Öffnen");
    _newPlayer  = new QAction("Neu");

    _playerData->addAction(_newPlayer);
    _playerData->addAction(_loadPlayer);

    _tournamentData = new QMenu();
    _tournamentData= menuBar()->addMenu(tr("Turnier"));

    _loadTournament  = new QAction("Öffnen");
    _newTournament   = new QAction("Neu");

    _tournamentData->addAction(_newTournament);
    _tournamentData->addAction(_loadTournament);


    _title               = new WindowLabel("Hauptmenü - DartsTurnier");
    _title->setTitleStyel();

    _noteDatabase                = new WindowLabel("Zu Beginn muss ein Spiel neu erzeugt oder geladen werden");
    _noteDatabase->setStyleSheet("QLabel{"
                          "font-size: 25px;"
                          "font-family: Candara;"
                          "color: red;}");

    _noteTournament      = new WindowLabel("Aktuelles Turnier");

    _playermanagment     = new WindowButton("Meldestelle");
    _playermanagment->mainMenuStyle();
    _playermanagment->setIcon(QIcon(":/img/addPlayer.png"));
    _playermanagment->setIconSize(QSize(65,65));

    _tournament          = new WindowButton("Spielplan");
    _tournament->mainMenuStyle();
    _tournament->setIcon(QIcon(":/img/gameplan.png"));
    _tournament->setIconSize(QSize(65,65));

    _viewer              = new WindowButton("Zuschaueransicht");
    _viewer->mainMenuStyle();
    _viewer->setIcon(QIcon(":/img/viewer.png"));
    _viewer->setIconSize(QSize(60,60));

    _referee             = new WindowButton("Richteransicht");
    _referee->mainMenuStyle();
    _referee->setIcon(QIcon(":/img/referee.png"));
    _referee->setIconSize(QSize(65,65));

    _statitsic            = new WindowButton("Statistik");
    _statitsic->mainMenuStyle();
    _statitsic->setIcon(QIcon(":/img/statistic.png"));
    _statitsic->setIconSize(QSize(65,65));

    SqliteConnector* sqlitConnector = &SqliteConnector::instance();

    _tournament->setEnabled(sqlitConnector->getDb()->isOpen());
    _playermanagment->setEnabled(sqlitConnector->getDb()->isOpen());
    _tournament->setEnabled(sqlitConnector->getDb()->isOpen());
    _viewer->setEnabled(sqlitConnector->getDb()->isOpen());
    _referee->setEnabled(sqlitConnector->getDb()->isOpen());
    _statitsic->setEnabled(sqlitConnector->getDb()->isOpen());

    if (sqlitConnector->getDb()->isOpen())
    {
        _noteDatabase->setVisible(false);
    }

    _playerManagementModel = new PlayerManagement();
    _refereeModel = new Referee();
}

void MainMenu::setwholeLayout()
{
    QWidget* widget= new QWidget;
    setCentralWidget(widget);


    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(_title,0,Qt::AlignCenter);
    layout->setSpacing(30);
    layout->addWidget(_noteDatabase,0,Qt::AlignCenter);
    layout->addWidget(_noteTournament,0,Qt::AlignCenter);
    layout->addWidget(_playermanagment,0,Qt::AlignCenter);
    layout->addWidget(_tournament,0,Qt::AlignCenter);
    layout->addWidget(_viewer,0,Qt::AlignCenter);
    layout->addWidget(_referee,0,Qt::AlignCenter);
    layout->addWidget(_statitsic,0,Qt::AlignCenter);

    widget->setLayout(layout);
 }


