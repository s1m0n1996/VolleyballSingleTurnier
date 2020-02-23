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
#include "View/refereepopupwinningleg.h"
#include "View/tournamentwindow.h"
#include "View/viewerwindow.h"
#include "View/windowbutton.h"
#include "View/windowlabel.h"

// TODO: Trunier anzeigen, welches gerade geladen ist.
#include <QDebug>
#include <QSizePolicy>

MainMenu::MainMenu(Model* model, QMainWindow* parent) :
    QMainWindow(parent),
    _model(model)
{
    createWidgets();
    setwholeLayout();
    connecting();

    setMinimumSize(700,600);
    setWindowFlags(Qt::WindowMinimizeButtonHint| Qt::WindowCloseButtonHint);
}

MainMenu::~MainMenu()
{
    delete  _playerData;
    delete  _loadPlayer;
    delete  _newPlayer;

    delete  _tournamentData;
    delete  _loadTournament;
    delete  _newTournament;

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
    TournamentWindow* tournamentWindow = new TournamentWindow;
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

    SqliteConnector* sqlitConnector = &SqliteConnector::instance();
    sqlitConnector->createDatabase(path);

    _tournament->setEnabled(sqlitConnector->getDb()->isOpen());
    _playermanagment->setEnabled(sqlitConnector->getDb()->isOpen());
    _tournament->setEnabled(sqlitConnector->getDb()->isOpen());
    _viewer->setEnabled(sqlitConnector->getDb()->isOpen());
    _referee->setEnabled(sqlitConnector->getDb()->isOpen());

    _noteDatabase->setVisible(false);
}

/*!
 * \brief Laden einer existierenden Datenbank
 *
 * \param void
 * \return void
 *
 * Der Pfad, indem die bereits erstellten DAtenbank gespeichert wurden, wird geöffnet und der Benutzer kann eine Datenbank zu öffnen auswählen
 * Wenn eine sqlite Datenbank geöffnet wurde, werden die Buttons wieder freigeschaltet und die Warnung entfernt
 */
void MainMenu::loadDatabase()
{
    QString path = QFileDialog::getOpenFileName(this,
                                        tr("Datenbank laden"), "",
                                        tr("Database File (*.sqlite) ;; All Files (*.*)"));

    SqliteConnector* sqlitConnector = &SqliteConnector::instance();
    sqlitConnector->openDatabase(path);

    _tournament->setEnabled(sqlitConnector->getDb()->isOpen());
    _playermanagment->setEnabled(sqlitConnector->getDb()->isOpen());
    _tournament->setEnabled(sqlitConnector->getDb()->isOpen());
    _viewer->setEnabled(sqlitConnector->getDb()->isOpen());
    _referee->setEnabled(sqlitConnector->getDb()->isOpen());

    _noteDatabase->setVisible(false);
}

void MainMenu:: connecting()
{
    connect(_playermanagment, SIGNAL(released()), this, SLOT(openPlayermanagmentWindow()));
    connect(_tournament, SIGNAL(released()), this, SLOT(openTournamentWindow()));
    connect(_viewer, SIGNAL(released()), this, SLOT(openViewerWindow()));
    connect(_referee, SIGNAL(released()), this, SLOT(openRefereeWindow()));
    connect(_newPlayer,SIGNAL(triggered()),this, SLOT(createDatabase()));
    connect(_loadPlayer,SIGNAL(triggered()),this, SLOT(loadDatabase()));
    connect(_newTournament,SIGNAL(triggered()),this,SLOT(tournamentName()));
//    connect(_tournamentName, SIGNAL(tournamentName()), this, SLOT(setTouenamentName()));
}

void MainMenu::tournamentName()
{
    TournamentNamePopUp* _tournamentName = new TournamentNamePopUp;
    _tournamentName->show();
}


void MainMenu::setTouenamentName()
{
    _noteTournament->setText("Das Turnier LEa ist ");
}

void MainMenu::createWidgets()
{
    setWindowTitle("Hauptmenü");
    setWindowIcon(QIcon(":/img/darts.png"));

    _playerData = new QMenu();
    _playerData= menuBar()->addMenu(tr("Spielerdateien"));

    _loadPlayer = new QAction("Laden");
    _newPlayer  = new QAction("Erstellen");

    _playerData->addAction(_newPlayer);
    _playerData->addAction(_loadPlayer);

    _tournamentData = new QMenu();
    _tournamentData= menuBar()->addMenu(tr("Turnier"));

    _loadTournament  = new QAction("Laden");
    _newTournament   = new QAction("Erstellen");

    _tournamentData->addAction(_newTournament);
    _tournamentData->addAction(_loadTournament);

    _title               = new WindowLabel("Hauptmenü - DartsTurnier");
    _title->setTitleStyel();

    _noteDatabase                = new WindowLabel("Zu Beginn muss ein Spiel neu erzeugt oder geladen werden");
    _noteDatabase->setStyleSheet("QLabel{"
                          "font-size: 25px;"
                          "font-family: Candara;"
                          "color: red;}");

    _noteTournament      = new WindowLabel("");

    _playermanagment     = new WindowButton("Meldestelle");
    _playermanagment->mainMenuStyle();
    _tournament          = new WindowButton("Spielplan");
    _tournament->mainMenuStyle();
    _viewer              = new WindowButton("Zuschaueransicht");
    _viewer->mainMenuStyle();
    _referee             = new WindowButton("Richteransicht");
    _referee->mainMenuStyle();

    SqliteConnector* sqlitConnector = &SqliteConnector::instance();

    _tournament->setEnabled(sqlitConnector->getDb()->isOpen());
    _playermanagment->setEnabled(sqlitConnector->getDb()->isOpen());
    _tournament->setEnabled(sqlitConnector->getDb()->isOpen());
    _viewer->setEnabled(sqlitConnector->getDb()->isOpen());
    _referee->setEnabled(sqlitConnector->getDb()->isOpen());

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

    widget->setLayout(layout);
 }


