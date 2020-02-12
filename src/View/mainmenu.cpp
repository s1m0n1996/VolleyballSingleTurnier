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


}
MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::openPlayermanagmentWindow()
{
    PlayermanagementWindow* playermanagment = new PlayermanagementWindow;
    playermanagment->showMaximized();
}
void MainMenu::openTournementWindow()
{
    TournementWindow* tournementWindow = new TournementWindow;
    tournementWindow->show();
}
void MainMenu::openViewerWindow()
{
    ViewerWindow* viewerWindow = new ViewerWindow;
    viewerWindow->showMaximized();
}

void MainMenu::openRefereeWindow()
{
    Referee* referee = new Referee;
    RefereeWindow* refereeWindow = new RefereeWindow(referee);
    refereeWindow->show();
}

void MainMenu::createDatabase()
{
    QString path = QFileDialog::getSaveFileName(this,
                                        tr("Datenbank anlegen"), "",
                                        tr("Database File (*.sqlite) ;; All Files (*.*)"));

    SqliteConnector* sqlitConnector = &SqliteConnector::instance();
    sqlitConnector->createDatabase(path);
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
    connect(_playermanagment, SIGNAL(released()), this, SLOT(openPlayermanagmentWindow()));
    connect(_tournement, SIGNAL(released()), this, SLOT(openTournementWindow()));
    connect(_viewer, SIGNAL(released()), this, SLOT(openViewerWindow()));
    connect(_referee, SIGNAL(released()), this, SLOT(openRefereeWindow()));
    connect(_new,SIGNAL(triggered()),this, SLOT(createDatabase()));
    connect(_load,SIGNAL(triggered()),this, SLOT(loadDatabase()));
}

void MainMenu::createButton()
{
       _note                = new WindowLabel("Zu Beginn muss ein Spiel neu erzeugt oder geladen werden");
       _note->setStyleSheet("QLabel{"
                            "font-size: 25px;"
                            "font-family: Candara;"
                            "color: red;}");
       _playermanagment     = new MenuButton("Meldestelle");
       _tournement          = new MenuButton("Spielplan");
       _viewer              = new MenuButton("Zuschaueransicht");
       _referee             = new MenuButton("Richteransicht");
       _load                = new QAction("Laden");
       _new                 = new QAction("Neu");

}

void MainMenu::setButtonsLayout()
{

    ui->verticalLayout->setSpacing(30);
    ui->verticalLayout->addWidget(_note,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_playermanagment,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_tournement,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_viewer,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_referee,0,Qt::AlignCenter);
    ui->menuGame->addAction(_new);
    ui->menuGame->addAction(_load);

}


