#include <QHeaderView>
#include "playermanagementWindow.h"
#include "Model/playermanagement.h"
#include "Model/player.h"
#include "QAbstractItemView"
#include <QModelIndex>



PlayermanagementWindow::PlayermanagementWindow(QWidget *parent):
    QMainWindow(parent),
 
    ui(new Ui::PlayermanagementWindow)
{
    ui->setupUi(this);
    showTabel();
    createButtons();
    createAddPlayerEdit();
    createMaxPlayer();
    setAllLayout();
    connecting();

}


PlayermanagementWindow::~PlayermanagementWindow()
{
    delete ui;
    delete _addPlayerButton;
    delete _startTournementButton;
    delete _add;
    delete _delete;
    delete _playernameLabel;
    delete _birthdayLabel;
    delete _countryLabel;
    delete _nameMaxPlayerLabel;
    delete _valueMaxPlayerLabel;
    delete _playernameEdit;
    delete _birthdayEdit;
    delete _countryEdit;
    delete _model;
    delete _modelGame;
    delete _allPlayer;
    delete _gamePlayer;

}

void PlayermanagementWindow::connecting()
{
    connect(_addPlayerButton,SIGNAL(released()), this, SLOT(addPlayer()));
    connect(_startTournementButton, SIGNAL(released()), this, SLOT(tournementName()));
    connect(_add,SIGNAL(released()), this, SLOT(giveModel()));
}
QList<QList<QString>> PlayermanagementWindow::getList()
{
 return _listAll;
}
void PlayermanagementWindow::giveModel()
{

    QAbstractItemModel* model = _allPlayer->model();
    QModelIndexList selection = _allPlayer->selectionModel()->selectedRows();


    for (QModelIndex index : selection)
    {
        QList<QString> listSingel;

        listSingel.append(model->index(index.row() , 0).data().toString());
        listSingel.append(model->index(index.row() , 1).data().toString());
        listSingel.append(model->index(index.row() , 2).data().toString());

        _listAll.append(listSingel);
    }

    qDebug()<< _listAll;
}
void PlayermanagementWindow::showTabel()
{
    _model      = new PlayerManagement;
    _allPlayer  = new TableView;
    _gamePlayer = new TableView;
    _allPlayer->setModel(_model->getDatabaseTableModel());
    _allPlayer->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _allPlayer->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _allPlayer->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

}
void PlayermanagementWindow::tournementName()
{
    TournementNamePopUp* tournementName = new TournementNamePopUp;
    tournementName->show();

}

void PlayermanagementWindow::createMaxPlayer()
{
    _nameMaxPlayerLabel = new WindowLabel("benötigte Spieler:");
    _valueMaxPlayerLabel = new WindowLabel("2");

}
void PlayermanagementWindow::createAddPlayerEdit()
{
    _playernameEdit     = new WindowEdit("Max Mustermann");    
    _birthdayEdit       = new WindowEdit("1990-01-30");
    _countryEdit        = new WindowEdit("Deutschland");

    _playernameLabel    = new WindowLabel("Spielername");    
    _birthdayLabel      = new WindowLabel("Gebrurtsdatum");
    _countryLabel       = new WindowLabel("Land");

}

void PlayermanagementWindow::createButtons()
{
    _addPlayerButton        = new WindowButton("Spieler hinzufügen");
    _startTournementButton  = new WindowButton("Turnier starten");
    _add                    = new WindowButton("->");
    _delete                 = new WindowButton("<-");
    _startTournementButton->setEnabled(false);
    _startTournementButton->setEnableStyle();
}


void PlayermanagementWindow::setAllLayout()
{
    ui->maxPlayerLayout->addWidget(_nameMaxPlayerLabel,0,Qt::AlignCenter);
    ui->maxPlayerLayout->addWidget(_valueMaxPlayerLabel,0,Qt::AlignCenter);
    ui->startTournementLayout->addWidget(_startTournementButton,0 ,Qt::AlignBottom);

    ui->addPlayerLayout->addWidget(_playernameLabel,1,0);
    ui->addPlayerLayout->addWidget(_birthdayLabel,2,0);
    ui->addPlayerLayout->addWidget(_countryLabel,3,0);


    ui->addPlayerLayout->setSpacing(2);
    ui->addPlayerLayout->setMargin(5);

    ui->addPlayerLayout->addWidget(_playernameEdit,1,1);
    ui->addPlayerLayout->addWidget(_birthdayEdit,2,1);
    ui->addPlayerLayout->addWidget(_countryEdit,3,1);


    ui->addPlayerLayout->addWidget(_addPlayerButton,5,Qt::AlignRight);

    ui->tabelViewLayout->addWidget(_allPlayer);
    ui->tabelViewLayout->addWidget(_add);
    ui->tabelViewLayout->addWidget(_gamePlayer);

}

void PlayermanagementWindow::setMaxPlayerLabel()
{
    _valueMaxPlayerLabel->setText(QString::number(_model->countMissingPlayersForNewGame()));
}


void PlayermanagementWindow::addPlayer()
{
    _valueMaxPlayerLabel->setText("0");
    _startTournementButton->setEnabled(true);

    Player* newplayer = new Player(_playernameEdit->text(), _birthdayEdit->text(), _countryEdit->text());

    _model-> addPlayerForNewGame(*newplayer);

    _allPlayer->setModel(_model->getDatabaseTableModel());
    _playernameEdit->clear();
    _birthdayEdit->clear();
    _countryEdit->clear();
}

