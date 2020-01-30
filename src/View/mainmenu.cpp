#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "View/playermanagment.h"
#include <QPalette>

#include <QCoreApplication>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Model/model.h"
#include "View/playermanagment.h"
#include <QPalette>
#include <QWidget>

MainMenu::MainMenu(Model* model, QMainWindow* parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu),
    _model(model)

{    
    ui->setupUi(this);
    modifiButton();
    connect(ui->playermanagmentPushButton, SIGNAL(released()), this, SLOT(openWindow()));
}
MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::openWindow()
{
    Playermanagment* playermanagment = new Playermanagment;
    playermanagment-> show();
}

void MainMenu:: modifiButton()
{
    int fixSizeX = 50;
    int fixSizeY = 250;

    ui->playermanagmentPushButton->setFixedSize(fixSizeY,fixSizeX);
    ui->playermanagmentPushButton->setStyleSheet("color: white, background-color: #3f3f3f");


    ui->tournamnetWindowPushButton->setFixedSize(fixSizeY, fixSizeX);
    ui->tournamnetWindowPushButton->setStyleSheet("background-color: #3f3f3f");

    ui->viewerPushButton->setFixedSize(fixSizeY, fixSizeX);
    ui->viewerPushButton->setStyleSheet("background-color: #3f3f3f");

    ui->refereeWindowPushButton->setFixedSize(fixSizeY, fixSizeX);
    ui->refereeWindowPushButton->setStyleSheet("background-color: #3f3f3f");


}
