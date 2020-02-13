#include <QHeaderView>
#include "playermanagementWindow.h"
#include "Model/playermanagement.h"
#include "Model/player.h"
#include "QAbstractItemView"
#include <QModelIndex>



PlayermanagementWindow::PlayermanagementWindow(PlayerManagement* playerManagementModel, QWidget *parent):
    QMainWindow(parent),
 
    ui(new Ui::PlayermanagementWindow)
{
    _playerManagementModel = playerManagementModel;
    _playerManagementModel->refreshDatabasePlayerTable();
    _playerManagementModel->refreshNextGamePlayerTable();

    ui->setupUi(this);
    showTable();
    createButtons();
    createAddPlayerEdit();
    createMissingPlayersForNewTournamentLabel();
    setAllLayout();
    connecting();

}


PlayermanagementWindow::~PlayermanagementWindow()
{
    delete ui;
    delete _addPlayerButton;
    delete _startTournementButton;
    delete _addPlayerForNewTournament;
    delete _deletePlayerForNewTournament;
    delete _playernameLabel;
    delete _birthdayLabel;
    delete _countryLabel;
    delete _nameMissingPlayersLabel;
    delete _valueMissingPlayersLabel;
    delete _playernameEdit;
    delete _birthdayEdit;
    delete _countryEdit;
    delete _allPlayerTableView;
    delete _gamePlayerTableView;

}

void PlayermanagementWindow::connecting()
{
    connect(_addPlayerButton,SIGNAL(released()), this, SLOT(addPlayerToDatabase()));
    connect(_startTournementButton, SIGNAL(released()), this, SLOT(tournamentName()));
    connect(_addPlayerForNewTournament, SIGNAL(released()), this, SLOT(addPlayerForNewGame()));
    connect(_deletePlayerForNewTournament, SIGNAL(released()), this, SLOT(dropPlayerForNewGame()));
    connect(_playerManagementModel, SIGNAL(valueChanged()), this, SLOT(setMissingPlayersForNewTournamentLabel()));
}


void PlayermanagementWindow::dropPlayerForNewGame()
{
    QAbstractItemModel* modelAll = _gamePlayerTableView->model();
    QModelIndexList selectedRows = _gamePlayerTableView->selectionModel()->selectedRows();

    for (QModelIndex index : selectedRows)
    {
        _playerManagementModel->dropPlayerForNewGame(Player(
                modelAll->index(index.row() , 0).data().toString(),
                modelAll->index(index.row() , 1).data().toString(),
                modelAll->index(index.row() , 2).data().toString()));
    }
}
void PlayermanagementWindow::addPlayerForNewGame()
{
    QAbstractItemModel* modelAll = _allPlayerTableView->model();
    QModelIndexList selectedRows = _allPlayerTableView->selectionModel()->selectedRows();

    for (QModelIndex index : selectedRows)
    {
        _playerManagementModel->addPlayerForNewGame(Player(
                modelAll->index(index.row() , 0).data().toString(),
                modelAll->index(index.row() , 1).data().toString(),
                modelAll->index(index.row() , 2).data().toString()));
    }
}

void PlayermanagementWindow::showTable()
{
    _allPlayerTableView  = new TableView;
    _gamePlayerTableView = new TableView;

    _allPlayerTableView->setModel(_playerManagementModel->getDatabaseTableModel());
    _allPlayerTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _allPlayerTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _allPlayerTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    _gamePlayerTableView->setModel(_playerManagementModel->getNextGamePlayerTableModel());
    _gamePlayerTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _gamePlayerTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _gamePlayerTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void PlayermanagementWindow::tournamentName()
{
    TournementNamePopUp* tournamentName = new TournementNamePopUp;
    tournamentName->show();

}

void PlayermanagementWindow::createMissingPlayersForNewTournamentLabel()
{
    _nameMissingPlayersLabel = new WindowLabel("benötigte Spieler:");
    _valueMissingPlayersLabel = new WindowLabel(QString::number(_playerManagementModel->countMissingPlayersForNewGame()));
    _valueMissingPlayersLabel->setStyleSheet("QLabel{"
                                        "font-size: 25px;"
                                        "font-family: Candara;"
                                        "color: red;}");

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
    _addPlayerForNewTournament      = new WindowButton("->");
    _deletePlayerForNewTournament   = new WindowButton("<-");
    _startTournementButton->setEnabled(false);
    _startTournementButton->setEnableStyle();
}


void PlayermanagementWindow::setAllLayout()
{
    ui->maxPlayerLayout->addWidget(_nameMissingPlayersLabel, 0, Qt::AlignCenter);
    ui->maxPlayerLayout->addWidget(_valueMissingPlayersLabel, 0, Qt::AlignCenter);
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

    QVBoxLayout* addDeleteLayout = new QVBoxLayout;

    ui->tabelViewLayout->addWidget(_allPlayerTableView);
    ui->tabelViewLayout->addLayout(addDeleteLayout);
    addDeleteLayout->addWidget(_addPlayerForNewTournament);
    addDeleteLayout->addWidget(_deletePlayerForNewTournament);
    ui->tabelViewLayout->addWidget(_gamePlayerTableView);

}

void PlayermanagementWindow::setMissingPlayersForNewTournamentLabel()
{
    _valueMissingPlayersLabel->setText(QString::number(_playerManagementModel->countMissingPlayersForNewGame()));
}


void PlayermanagementWindow::addPlayerToDatabase()
{
    _valueMissingPlayersLabel->setText("0");
    _startTournementButton->setEnabled(true);

    //Player* newplayer = new Player(_playernameEdit->text(), _birthdayEdit->text(), _countryEdit->text());

    //_playerManagementModel-> addPlayerForNewGame(*newplayer);


    _playernameEdit->clear();
    _birthdayEdit->clear();
    _countryEdit->clear();

    _playerManagementModel->refreshDatabasePlayerTable();
}

