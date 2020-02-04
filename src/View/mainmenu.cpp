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
void MainMenu:: connecting()
{
    connect(_playermanagment, SIGNAL(clicked()), this, SLOT(openPlayermanagmentWindow()));
    connect(_tournement, SIGNAL(clicked()), this, SLOT(openTournementWindow()));
    connect(_viewer, SIGNAL(clicked()), this, SLOT(openViewerWindow()));
    connect(_referee, SIGNAL(clicked()), this, SLOT(openRefereeWindow()));
    connect(_new,SIGNAL(triggered()),this, SLOT(createDatabase()));
    connect(_load,SIGNAL(triggered()),this, SLOT(loadDatabase()));
}
void MainMenu::openPlayermanagmentWindow()
{
    Playermanagment* playermanagment = new Playermanagment;
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
    RefereeWindow* refereeWindow = new RefereeWindow;
    refereeWindow->show();
}


void MainMenu::createButton()
{
        _playermanagment    = new MenuButton("Meldestelle");
       _tournement          = new MenuButton("Spielplan");
       _viewer              = new MenuButton("Zuschaueransicht");
       _referee             = new MenuButton("Richteransicht");
       _load                = new QAction("Laden");
       _new                 = new QAction("Neu");

}

void MainMenu::setButtonsLayout()
{
    ui->verticalLayout->setSpacing(30);
    ui->verticalLayout->addWidget(_playermanagment,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_tournement,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_viewer,0,Qt::AlignCenter);
    ui->verticalLayout->addWidget(_referee,0,Qt::AlignCenter);
    ui->menuGame->addAction(_new);
    ui->menuGame->addAction(_load);

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
