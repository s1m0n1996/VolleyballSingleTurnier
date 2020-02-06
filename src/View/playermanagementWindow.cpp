
#include "playermanagementWindow.h"



PlayermanagementWindow::PlayermanagementWindow(QWidget *parent):
    QMainWindow(parent),
 
    ui(new Ui::PlayermanagementWindow)
{
    ui->setupUi(this);
    createButtons();
    createAddPlayerEdit();
    createMaxPlayer();
    setAllLayout();
    connecting();
}


PlayermanagementWindow::~PlayermanagementWindow()
{
    delete ui;
}

void PlayermanagementWindow:: connecting()
{
    connect(_startTournementButton, SIGNAL(released()), this, SLOT(test));
    connect(_addPlayerButton,SIGNAL(released()), this, SLOT(addPlayer()));
}
void PlayermanagementWindow::test()
{
   _valueMaxPlayerLabel->setText("0");

}

void PlayermanagementWindow::createMaxPlayer()
{
    _nameMaxPlayerLabel = new WindowLabel("benötigte Spieler:");
    _valueMaxPlayerLabel = new WindowLabel("2");

}
void PlayermanagementWindow::createAddPlayerEdit()
{
    _playernameEdit     = new WindowEdit("Max Mustermann");
    _countryEdit        = new WindowEdit("Deutschland");
    _birthdayEdit       = new WindowEdit("1900-01-01");

    _playernameLabel    = new WindowLabel("Spielername");
    _countryLabel       = new WindowLabel("Land");
    _birthdayLabel      = new WindowLabel("Gebrurtsdatum");

}

void PlayermanagementWindow::createButtons()
{
    _addPlayerButton        = new WindowButton("Spieler hinzufügen");
    _startTournementButton  = new WindowButton("Turnier starten");
    _startTournementButton->setEnabled(false);
    _startTournementButton->setStyleSheet("QPushButton{"
                                          "color: #dcdcdc;"
                                          "background-color: #40b946;"
                                          "font-size: 20px;"
                                          "font-family: Candara;}"
                                          "QPushButton:disabled{"
                                          "background-color: #acacac;}");
}


void PlayermanagementWindow::setAllLayout()
{
    ui->maxPlayerLayout->addWidget(_nameMaxPlayerLabel);
    ui->maxPlayerLayout->addWidget(_valueMaxPlayerLabel,0,Qt::AlignCenter);
    ui->startTournementLayout->addWidget(_startTournementButton,0 ,Qt::AlignBottom);
    ui->addPlayerLayout->addWidget(_playernameLabel,1,0);
    ui->addPlayerLayout->addWidget(_countryLabel,2,0);
    ui->addPlayerLayout->addWidget(_birthdayLabel,3,0);

    ui->addPlayerLayout->setSpacing(2);
    ui->addPlayerLayout->setMargin(5);

    ui->addPlayerLayout->addWidget(_playernameEdit,1,1);
    ui->addPlayerLayout->addWidget(_countryEdit,2,1);
    ui->addPlayerLayout->addWidget(_birthdayEdit,3,1);

    ui->addPlayerLayout->addWidget(_addPlayerButton,5,Qt::AlignRight);
}

void PlayermanagementWindow::setMaxPlayerLabel()
{
    int playerNumber= 1;
    if (round(log(playerNumber)/log(2)) == playerNumber)
    {
        _valueMaxPlayerLabel->setText("0");
        _startTournementButton->setEnabled(true);
    }
    else
    {
        int neededPlayerNumber = 4;
        _valueMaxPlayerLabel->setText(QString::number(neededPlayerNumber));
    }

}


void PlayermanagementWindow::addPlayer()
{
    _valueMaxPlayerLabel->setText("0");
    _startTournementButton->setEnabled(true);
}

