/*!
* \file mainwindow.cpp
* \brief mainwindow
* \author Nico Meyer
*/

#include <QCoreApplication>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "mainwindow.h"
#include "Model/model.h"

MainWindow::MainWindow(Model* model, QWidget* parent) :
    QWidget(parent),
    _model(model)
{
    QVBoxLayout* layoutCompleteWindow = new QVBoxLayout;
    QHBoxLayout* layoutTopLine = new QHBoxLayout;
    QLabel* headline = new QLabel("Darts Turnier");
    QLabel* game = new QLabel("Spiel/Datei:");
    QPushButton* load = new QPushButton("Laden");
    QPushButton* save = new QPushButton("Speichern");
    QPushButton* gameBoard = new QPushButton("Zuschauer Ansicht");
    QPushButton* meldestelle = new QPushButton("Meldestelle");
    QPushButton* referee = new QPushButton("Punkterichter");
    QPushButton* tournament = new QPushButton("Spielplan");

    gameBoard->setFixedWidth(250);
    meldestelle->setFixedWidth(250);
    referee->setFixedWidth(250);
    tournament->setFixedWidth(250);
    load->setFixedWidth(125);
    save->setFixedWidth(125);
    game->setFixedWidth(125);

    QFont fontHeadline("Arial", 25, QFont::Bold);
    QFont fontGame("Arial", 12);
    headline->setFont(fontHeadline);

    layoutTopLine->addWidget(game);
    layoutTopLine->addWidget(save);
    layoutTopLine->addWidget(load);
    layoutTopLine->setSpacing(0);

    //layoutCompleteWindow->setContentsMargins(0,0,0,1250); //Position bei einem großem Fenster festlegen
    //layoutCompleteWindow->setMargin(250); // Mitte?

    setLayout(layoutCompleteWindow);
    layoutCompleteWindow->addWidget(headline);
    layoutCompleteWindow->addLayout(layoutTopLine);
    layoutCompleteWindow->addWidget(meldestelle);
    layoutCompleteWindow->addWidget(gameBoard);
    layoutCompleteWindow->addWidget(referee);
    layoutCompleteWindow->addWidget(tournament);


}
MainWindow::~MainWindow()
{
}

void
MainWindow::accept(void)
{

}

void
MainWindow::update(void)
{

}

void
MainWindow::newWindow(void)
{

}
