#include "mainmenu.h"

MainMenu::MainMenu(Model* model, QMainWindow* parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu),
    _model(model)

{
    ui->setupUi(this);
    createButton();
    setButtonsLayout();

    connecting();

    SqliteConnector* sqlitConnector = &SqliteConnector::instance();
    _tournament->setEnabled(sqlitConnector->getDb()->isOpen());
    _playermanagment->setEnabled(sqlitConnector->getDb()->isOpen());
    _tournament->setEnabled(sqlitConnector->getDb()->isOpen());
    _viewer->setEnabled(sqlitConnector->getDb()->isOpen());
    _referee->setEnabled(sqlitConnector->getDb()->isOpen());

    if  (sqlitConnector->getDb()->isOpen())
    {
        _noteDatabase->setVisible(false);
    }

    setWindowFlags(Qt::WindowMinimizeButtonHint| Qt::WindowCloseButtonHint);
    _playerManagementModel = new PlayerManagement();
    _refereeModel = new Referee();
}
MainMenu::~MainMenu()
{
    delete ui;
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

void MainMenu::loadDatabase()
{
    QString path = QFileDialog::getOpenFileName(this,
                                        tr("Datenbank laden"), "",
                                        tr("Database File (*.sqlite) ;; All Files (*.*)"));

    SqliteConnector* sqlitConnector = &SqliteConnector::instance();
    sqlitConnector->openDatabase(path);
}

void MainMenu:: connecting()
{
//    TournamentNamePopUp* _tournamentName = new TournamentNamePopUp;

    connect(_playermanagment, SIGNAL(released()), this, SLOT(openPlayermanagmentWindow()));
    connect(_tournament, SIGNAL(released()), this, SLOT(openTournamentWindow()));
    connect(_viewer, SIGNAL(released()), this, SLOT(openViewerWindow()));
    connect(_referee, SIGNAL(released()), this, SLOT(openRefereeWindow()));
    connect(_new,SIGNAL(triggered()),this, SLOT(createDatabase()));
    connect(_load,SIGNAL(triggered()),this, SLOT(loadDatabase()));
    connect(ui->labelErstellen,SIGNAL(triggered()),this,SLOT(tournamentName()));
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
void MainMenu::createButton()
{
       _noteDatabase                = new WindowLabel("Zu Beginn muss ein Spiel neu erzeugt oder geladen werden");
       _noteDatabase->setStyleSheet("QLabel{"
                            "font-size: 25px;"
                            "font-family: Candara;"
                            "color: red;}");
       _noteTournament      = new WindowLabel("");
       _playermanagment     = new MenuButton("Meldestelle");
       _tournament          = new MenuButton("Spielplan");
       _viewer              = new MenuButton("Zuschaueransicht");
       _referee             = new MenuButton("Richteransicht");
       _load                = new QAction("Laden");
       _new                 = new QAction("Neu");
       _title               = new WindowLabel("Hauptmenü - DartsTurnier");
       _title->titleStyel();
       _playerData = new QMenuBar();

}

void MainMenu::setButtonsLayout()
{
    ui->verticalLayout->addWidget(_title,0,Qt::AlignCenter);
    ui->verticalLayout->setSpacing(30);
    ui->verticalLayout->addWidget(_noteDatabase,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_noteTournament,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_playermanagment,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_tournament,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_viewer,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_referee,0,Qt::AlignCenter);
    ui->menuGame->addAction(_new);
    ui->menuGame->addAction(_load);
}


