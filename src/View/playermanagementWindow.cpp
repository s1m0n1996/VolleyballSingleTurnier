#include <QHeaderView>
#include "playermanagementWindow.h"
#include "Model/playermanagement.h"
#include "Model/player.h"
#include "QAbstractItemView"
#include <QModelIndex>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>


PlayermanagementWindow::PlayermanagementWindow(PlayerManagement* playerManagementModel, QWidget *parent):
    QMainWindow(parent), 
    ui(new Ui::PlayermanagementWindow)
{
    _playerManagementModel = playerManagementModel;
    _playerManagementModel->refreshDatabasePlayerTable();
    _playerManagementModel->refreshNextGamePlayerTable();

    ui->setupUi(this);
    showTable();
    createMissingPlayersForNewTournamentLabel();
    createButtons();
    createAddPlayerEdit();
    setAllLayout();
    connecting();
    setWindowIcon(QIcon(":/img/darts.png"));

}

PlayermanagementWindow::~PlayermanagementWindow()
{
    delete ui;
    delete _addPlayerButton;
    delete _startTournamentButton;
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

void PlayermanagementWindow::addPlayerForNewGame()
{
    QAbstractItemModel* modelAll = _allPlayerTableView->model();
    QModelIndexList selectedRows = _allPlayerTableView->selectionModel()->selectedRows();

    for (QModelIndex index : selectedRows)
    {
        _playerManagementModel->addPlayerForNewGame(Player(
                modelAll->index(index.row(), 0).data().toString(),
                modelAll->index(index.row(), 1).data().toString(),
                modelAll->index(index.row(), 2).data().toString()));
    }
    _allPlayerTableView->selectionModel()->clearSelection();
}

void PlayermanagementWindow::addPlayerToDatabase()
{
    _valueMissingPlayersLabel->setText("0");
    _startTournamentButton->setEnabled(true);

    Player* newplayer = new Player(_playernameEdit->text(), _birthdayEdit->text(), _countryEdit->text());
    _playerManagementModel->addPlayerForNewGame(*newplayer);

    _playernameEdit->clear();
    _birthdayEdit->clear();
    _countryEdit->clear();

    _playerManagementModel->refreshDatabasePlayerTable();

}

void PlayermanagementWindow::connecting()
{
    connect(_addPlayerButton, SIGNAL(released()), this, SLOT(addPlayerToDatabase()));
    connect(_startTournamentButton, SIGNAL(released()), this, SLOT(tournamentName()));
    connect(_addPlayerForNewTournament, SIGNAL(released()), this, SLOT(addPlayerForNewGame()));
    connect(_deletePlayerForNewTournament, SIGNAL(released()), this, SLOT(dropPlayerForNewGame()));
    connect(_playerManagementModel, SIGNAL(valueChanged()), this, SLOT(setMissingPlayersForNewTournamentLabel()));
}

void PlayermanagementWindow::createAddPlayerEdit()
{
    _playernameEdit     = new WindowEdit("Max Mustermann");
    _birthdayEdit       = new WindowEdit("1990-01-30");
    _countryEdit        = new WindowEdit("Deutschland");

    _playernameLabel    = new WindowLabel("Spielername");
    _birthdayLabel      = new WindowLabel("Geburtsdatum");
    _countryLabel       = new WindowLabel("Land");
    _photo              = new WindowLabel("Foto");
    _colorLabel         = new QLabel;
    _colorLabel->setStyleSheet("background-color:#550000;");
    _title = new WindowLabel("Meldestelle");
    _title->setTitleStyel();


}

void PlayermanagementWindow::createButtons()
{
    _addPlayerButton        = new WindowButton("Spieler hinzufügen");
    _startTournamentButton  = new WindowButton("Turnier starten");

    _addPhoto = new WindowButton("Foto hinzufügen");
    _addPhoto->setIcon(QIcon(":/img/add-user.png"));
    _addPhoto->setIconSize(QSize(65,65));

    _addPlayerForNewTournament      = new WindowButton("->");
    _deletePlayerForNewTournament   = new WindowButton("<-");
    _startTournamentButton->setEnabled(false);
    _startTournamentButton->setEnableStyle();
    _startTournamentButton->setIcon(QIcon(":/img/dart (1).png"));
    _startTournamentButton->setIconSize(QSize(65,65));

    if (_valueMissingPlayersLabel->text() == "0")
    {
        _valueMissingPlayersLabel->setStartTournamentStyle();
        _startTournamentButton->setEnabled(true);
    }

}

void PlayermanagementWindow::createMissingPlayersForNewTournamentLabel()
{
    _nameMissingPlayersLabel = new WindowLabel("benötigte Spieler:");
    _valueMissingPlayersLabel = new WindowLabel(
    QString::number(_playerManagementModel->countMissingPlayersForNewGame()));
    _valueMissingPlayersLabel->setNotStartTournamentStyle();
}

void PlayermanagementWindow::dropPlayerForNewGame()
{
    // FIXME: wenn man mehrere spieler aufeinmal entfernt wird jeder 2. nicht gelösch
    QAbstractItemModel* modelGame = _gamePlayerTableView->model();
    QModelIndexList selectedRows = _gamePlayerTableView->selectionModel()->selectedRows();
    for (QModelIndex index : selectedRows)
    {
        _playerManagementModel->dropPlayerForNewGame(Player(
                modelGame->index(index.row(), 0).data().toString(),
                modelGame->index(index.row(), 1).data().toString(),
                modelGame->index(index.row(), 2).data().toString()));
    }

    _gamePlayerTableView->selectionModel()->clearSelection();
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

void PlayermanagementWindow::setAllLayout()
{
    QWidget* widget= new QWidget;
    setCentralWidget(widget);

    QVBoxLayout* addDeleteLayout    = new QVBoxLayout;
    QVBoxLayout* maxPlayerLayout    = new QVBoxLayout;
    QVBoxLayout* mainLayout         = new QVBoxLayout;
    QGridLayout* addPlayerLayout    = new QGridLayout;
    QHBoxLayout* tabelViewLayout    = new QHBoxLayout;


    mainLayout->addWidget(_colorLabel);
    mainLayout->addWidget(_title);

    maxPlayerLayout->addWidget(_nameMissingPlayersLabel, 0, Qt::AlignCenter);
    maxPlayerLayout->addWidget(_valueMissingPlayersLabel, 0, Qt::AlignCenter);

    addPlayerLayout->addWidget(_playernameLabel, 1, 0);
    addPlayerLayout->addWidget(_birthdayLabel, 2, 0);
    addPlayerLayout->addWidget(_countryLabel, 3, 0);


    addPlayerLayout->setSpacing(2);
    addPlayerLayout->setMargin(5);
    addPlayerLayout->addWidget(_playernameEdit, 1, 1);
    addPlayerLayout->addWidget(_birthdayEdit, 2, 1);
    addPlayerLayout->addWidget(_countryEdit, 3, 1);
    addPlayerLayout->addWidget(_photo,4,0);
    addPlayerLayout->addWidget(_addPhoto,4,2);
    addPlayerLayout->addWidget(_addPlayerButton, 6, Qt::AlignRight);
    addPlayerLayout->addWidget(_startTournamentButton,6,0);


    tabelViewLayout->addWidget(_allPlayerTableView);
    tabelViewLayout->addLayout(addDeleteLayout);
    tabelViewLayout->addWidget(_gamePlayerTableView);


    addDeleteLayout->addWidget(_addPlayerForNewTournament);
    addDeleteLayout->addWidget(_deletePlayerForNewTournament);

    tabelViewLayout->addLayout(addDeleteLayout);

    mainLayout->addLayout(tabelViewLayout);
    mainLayout->addLayout(maxPlayerLayout);
    mainLayout->addLayout(addPlayerLayout);

    widget->setLayout(mainLayout);
}

void PlayermanagementWindow::setMissingPlayersForNewTournamentLabel()
{
    _valueMissingPlayersLabel->setText(QString::number(_playerManagementModel->countMissingPlayersForNewGame()));
    _startTournamentButton->setEnabled(false);
    _valueMissingPlayersLabel->setNotStartTournamentStyle();

    if (_valueMissingPlayersLabel->text() == "0")
    {
        _valueMissingPlayersLabel->setStartTournamentStyle();
        _startTournamentButton->setEnabled(true);
    }
}

void PlayermanagementWindow::tournamentName()
{
    TournamentWindow* tournamentWindow2 = new TournamentWindow;
    tournamentWindow2->showMaximized();

}


