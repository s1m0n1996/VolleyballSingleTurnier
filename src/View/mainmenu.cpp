#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "View/playermanagment.h"
#include <QPalette>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);

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
