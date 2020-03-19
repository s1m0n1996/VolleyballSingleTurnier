/*!
* \file winnerpopup.cpp
* \brief Diese Klasse erzeugt ein Fenster mit dem Namen des Gewinner eines Spieles und eines Turniers
* \author Lea Kesselmeier
*/
#include "winnerpopup.h"
#include "View/windowButton.h"
#include "View/windowLabel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <utility>

WinnerPopup::WinnerPopup(QString winnerName, QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Gewinner");
    setWindowIcon(QIcon(":/img/crowns.png"));
    setMinimumSize(700, 600);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    setStyleSheet("background: white;");
    createWidgets(std::move(winnerName));
    setwholeLayout();
    connect(_okButton, SIGNAL(released()), this, SLOT(close()));
}

WinnerPopup::~WinnerPopup()
{

}

void WinnerPopup::createWidgets(QString winnerName)
{
    _title = new WindowLabel("Der Gewinner des Spiels ist");
    _title->setWinnerStyle();

    _winner = new WindowLabel(std::move(winnerName));
    _winner->setWinnerStyle();

    _crown = new WindowLabel("");
    _crown->setPixmap(QPixmap(":/img/crowns.png").scaled(300,300,Qt::KeepAspectRatio));

    _okButton = new WindowButton("OK");

}

void WinnerPopup::setwholeLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* layoutButton = new QHBoxLayout;


    layoutButton->addWidget(_okButton,Qt::AlignRight, Qt::AlignRight);

    layout->addWidget(_title, 0, Qt::AlignCenter);
    layout->addWidget(_crown, 1, Qt::AlignCenter);
    layout->addWidget(_winner,2, Qt::AlignCenter);
    layout->addLayout(layoutButton);
    setLayout(layout);
}

void WinnerPopup::setWinnerLeg()
{
    _title->setText("Der Gewinner des Legs ist");
}

void WinnerPopup::setWinnerTournament()
{
    _title->setText("Der Gewinner des Turniers ist");
}

void WinnerPopup::connecting()
{
    connect(_okButton,SIGNAL(released()), this , SLOT(closeRefereeWindow()));
}

void WinnerPopup::closeRefereeWindow()
{
    emit clickedOk();
    close();
}
