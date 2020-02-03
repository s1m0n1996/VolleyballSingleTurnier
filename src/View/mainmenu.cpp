#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "View/playermanagment.h"
#include "View/viewerwindow.h"
#include "View/refereewindow.h"
#include "View/tournementwindow.h"
#include <QPalette>

#include <QCoreApplication>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Model/model.h"
#include <QPalette>
#include <QWidget>
#include <QLabel>

MainMenu::MainMenu(Model* model, QMainWindow* parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu),
    _model(model)

{
//    int playermanagmentnumber = 1;
//    int tournementnumber = 2;
//    int viewernumber = 3;
//    int refereenumber = 4;

    ui->setupUi(this);
    createButton();
    setButtonsLayout();
    connect(_playermanagment, SIGNAL(clicked()), this, SLOT(openPlayermanagmentWindow()));
    connect(_tournement, SIGNAL(clicked()), this, SLOT(openTournementWindow()));
    connect(_viewer, SIGNAL(clicked()), this, SLOT(openViewerWindow()));
    connect(_referee, SIGNAL(clicked()), this, SLOT(openRefereeWindow()));

}
MainMenu::~MainMenu()
{
    delete ui;
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
//void MainMenu::openWindow()
//{
//    int number = 1;
//    switch (number)
//        {
//        case 1 :{
//                Playermanagment* playermanagment = new Playermanagment;
//                playermanagment->show();
//    }
//                break;
//        case 2 :{
//                TournementWindow* tournementWindow = new TournementWindow;
//                tournementWindow->show();
//    }
//                break;
//        case 3 :{
//                ViewerWindow* viewerWindow = new ViewerWindow;
//                viewerWindow->show();
//    }
//                break;
//        case 4 :{
//                RefereeWindow* refereeWindow = new RefereeWindow;
//                refereeWindow->show();
//    }
//                break;

//                }
//}

void MainMenu:: createButton()
{
    _playermanagment = new MenuButton("Meldestelle");
       _tournement = new MenuButton("Spielplan");
       _viewer = new MenuButton("Zuschaueransicht");
       _referee = new MenuButton("Richteransicht");
    _loadSave = new QLabel("Spiel:");
       _load = new QPushButton("Laden");
       _save = new QPushButton("Speichern");

       _loadSave -> setStyleSheet("font-size: 20px; font-family: Bahnschrift Light; font: bold");
       _load->setStyleSheet("Border: false; text-decoration: underline; font-size: 20px; font-family: Bahnschrift Light; font: bold");
       _save->setStyleSheet("Border: false; text-decoration: underline; font-size: 20px; font-family: Bahnschrift Light; font: bold");
}


void MainMenu:: setButtonsLayout()
{
    ui->loadSaveGridLayout->addWidget(_loadSave,0,2);
    ui->loadSaveGridLayout->addWidget(_load,1,Qt::AlignLeft);
    ui->loadSaveGridLayout->addWidget(_save,1,Qt::AlignRight);

    ui->buttonsLayout->setSpacing(30);
    ui->buttonsLayout->addWidget(_playermanagment,0,Qt::AlignCenter);
    ui->buttonsLayout->addWidget(_tournement,0,Qt::AlignCenter);
    ui->buttonsLayout->addWidget(_viewer,0,Qt::AlignCenter);
    ui->buttonsLayout->addWidget(_referee,0,Qt::AlignCenter);

}
