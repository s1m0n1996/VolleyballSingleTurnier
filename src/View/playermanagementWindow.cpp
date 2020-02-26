/*!
* \file playermanagment.cpp
* \brief Diese Klasse ist die View Klasse der Meldestelle
* \author Lea Kesselmeier
*/
#include <QHeaderView>
#include <QAction>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QMenu>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QFileDialog>

#include "View/playermanagementWindow.h"
#include "Model/playermanagement.h"
#include "Model/player.h"
#include "Model/gameboard.h"

#include "View/tabelview.h"
#include "View/tournamentwindow.h"

#include "View/windowbutton.h"
#include "View/windowedit.h"
#include "View/windowlabel.h"

PlayermanagementWindow::PlayermanagementWindow(PlayerManagement* playerManagementModel, QWidget *parent):
    QMainWindow(parent)
{
    _playerManagementModel = playerManagementModel;
    _playerManagementModel->refreshDatabasePlayerTable();
    _playerManagementModel->refreshNextGamePlayerTable();

    createWidges();
    setAllLayout();
    connecting();
    setWindowIcon(QIcon(":/img/darts.png"));
    setWindowTitle("Meldestelle");
}

PlayermanagementWindow::~PlayermanagementWindow()
{
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

/*!
 * \brief Anzeige der benötigten Spieler zum Turnierbeginn
 *
 * \param void
 * \return void
 *
 * Das Label mit der Überschrift und der Spieleranzahl wird erzeugt, und aktualisiert
 * Der Button zum Start wird, freigeschlatet oder bleibt nicht drückbar, wenn die benötigte Spieleranzahl Null beträgt
 */
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

/*!
 * \brief Einfügen eines oder mehrere neuen Spieler zu einem Spiel
 *
 * \param void
 * \return void
 *
 * Die Inices der markierten Reihen werden dem Model übergeben und dort weiter verarbeitet
 * Die ausgewählten Reihen werden wieder nicht markiert angezeigt
 */
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

/*!
 * \brief Löschen eines oder mehrere neuen Spieler zu einem Spiel
 *
 * \param void
 * \return void
 *
 * Die Inices der markierten Reihen werden dem Model übergeben und dort weiter verarbeitet
 * Die ausgewählten Reihen werden wieder nicht markiert angezeigt
 */
void PlayermanagementWindow::dropPlayerForNewGame()
{
    QAbstractItemModel* modelGame = _gamePlayerTableView->model();
    QModelIndexList selectedRows = _gamePlayerTableView->selectionModel()->selectedRows();
    QList<Player> players;
    for (QModelIndex index : selectedRows)
    {
        players.append(Player(
                modelGame->index(index.row(), 0).data().toString(),
                modelGame->index(index.row(), 1).data().toString(),
                modelGame->index(index.row(), 2).data().toString()));
    }

    for (Player& player: players)
    {
        _playerManagementModel->dropPlayerForNewGame(player);
    }

    _gamePlayerTableView->selectionModel()->clearSelection();
}

/*!
 * \brief Einfügen eines neuen Spieler zu gesamten Spielerdatenbank
 *
 * \param void
 * \return void
 *
 * Ein Objekt der Klasse Player wird erzeugt, dem erden die neuen Varibalen mitgegeben
 * Dieser wird dann sowohl dem aktuellen Spiel als auch der gesamten Spielerdatenabnk hinzugefügt
 * Die Edits der Namen,Geburtstag und Land werden wieder frei beschreibbar gemacht
 */
void PlayermanagementWindow::addPlayerToDatabase()
{

    Player* newplayer = new Player(_playernameEdit->text(), _birthdayEdit->text(), _countryEdit->text());
    _playerManagementModel->addPlayerForNewGame(*newplayer);

    _playernameEdit->clear();
    _birthdayEdit->clear();
    _countryEdit->clear();

    _playerManagementModel->refreshDatabasePlayerTable();

}

void PlayermanagementWindow::createDeleteMenu()
{
        QAction* deletePlayer  = new QAction("Löschen",this);
        connect(deletePlayer,SIGNAL(triggered()),this,SLOT(deletePlayer()));

        QMenu* deleteMenu = new QMenu( this);
        deleteMenu->addAction(deletePlayer);
        deleteMenu->exec(QCursor::pos());
}


void PlayermanagementWindow::deletePlayer()
{

}
void PlayermanagementWindow::addPhoto()
{
    QString path = QFileDialog::getOpenFileName(this,
                                        tr("Bild laden"), "",
                                        tr("Photo File (*.png) ;; All Files (*.*)"));


}

/*!
 * \brief Erstellt den Tournierplan
 *
 * \param void
 * \return void
 *
 * Diese methode erstellt startet das Turnier. Dadurh wird der Spielplan erstellt und das Spiel geht in den
 * Zustand, das es gestartet ist.
 */
void PlayermanagementWindow::startTournament()
{
    Gameboard gameBoard(_playerManagementModel->getPlayersForNextGame());
    window()->close();
    GameManagement* gameManagement = &GameManagement::instance();
    gameManagement->setIsTournamentStarted();
}

void PlayermanagementWindow::connecting()
{
    connect(_playerManagementModel, SIGNAL(valueChanged()), this, SLOT(setMissingPlayersForNewTournamentLabel()));

    connect(_addPlayerForNewTournament, SIGNAL(released()), this, SLOT(addPlayerForNewGame()));
    connect(_deletePlayerForNewTournament, SIGNAL(released()), this, SLOT(dropPlayerForNewGame()));
    connect(_allPlayerTableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(createDeleteMenu()));


    connect(_addPlayerButton, SIGNAL(released()), this, SLOT(addPlayerToDatabase()));
    connect(_addPhoto,SIGNAL(released()),this, SLOT(addPhoto()));

    connect(_startTournamentButton, SIGNAL(released()), this, SLOT(startTournament()));
}

void PlayermanagementWindow::createWidges()
{
    _colorLabel         = new QLabel;
    _colorLabel->setStyleSheet("background-color:#550000;");
    _title = new WindowLabel("Meldestelle");
    _title->setTitleStyel();

    _allPlayerLabel = new WindowLabel("gesamte Spieler");

    _gamePlayerLabel = new WindowLabel("aktuelle Spieler im Turnier");

    showTable();
    _addPlayerForNewTournament      = new WindowButton("");
    _addPlayerForNewTournament->setIcon(QIcon(":/img/right.png"));
    _addPlayerForNewTournament->setIconSize(QSize(65,65));

    _deletePlayerForNewTournament   = new WindowButton("");
    _deletePlayerForNewTournament->setIcon(QIcon(":/img/left.png"));
    _deletePlayerForNewTournament->setIconSize(QSize(65,65));



    _nameMissingPlayersLabel = new WindowLabel("benötigte Spieler:");
    _valueMissingPlayersLabel = new WindowLabel(
    QString::number(_playerManagementModel->countMissingPlayersForNewGame()));
    _valueMissingPlayersLabel->setNotStartTournamentStyle();

    _playernameEdit     = new WindowEdit("Max Mustermann");
    _birthdayEdit       = new WindowEdit("1990-01-30");
    _countryEdit        = new WindowEdit("Deutschland");

    _playernameLabel    = new WindowLabel("Spielername");
    _birthdayLabel      = new WindowLabel("Geburtsdatum");
    _countryLabel       = new WindowLabel("Land");
    _photo              = new WindowLabel("Foto");

    _addPhoto = new WindowButton("Foto hinzufügen");
    _addPhoto->setIcon(QIcon(":/img/addPhoto.png"));
    _addPhoto->setIconSize(QSize(33,33));

    _addPlayerButton        = new WindowButton("Spieler hinzufügen");
    _addPlayerButton->setIcon(QIcon(":/img/addPlayer.png"));
    _addPlayerButton->setIconSize(QSize(40,40));

    _startTournamentButton  = new WindowButton("Turnier starten");
    _startTournamentButton->setEnabled(false);
    _startTournamentButton->setEnableStyle();
    _startTournamentButton->setIcon(QIcon(":/img/createDart.png"));
    _startTournamentButton->setIconSize(QSize(65,65));

    if (_valueMissingPlayersLabel->text() == "0")
    {
        _valueMissingPlayersLabel->setStartTournamentStyle();
        _startTournamentButton->setEnabled(true);
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

void PlayermanagementWindow::setAllLayout()
{
    QWidget* widget= new QWidget;
    setCentralWidget(widget);

    QVBoxLayout* addDeleteLayout        = new QVBoxLayout;
    QVBoxLayout* maxPlayerLayout        = new QVBoxLayout;
    QVBoxLayout* mainLayout             = new QVBoxLayout;
    QGridLayout* addPlayerLayout        = new QGridLayout;
    QHBoxLayout* tabelViewLayout        = new QHBoxLayout;
    QHBoxLayout* titleTabelViewLayout   = new QHBoxLayout;


    mainLayout->addWidget(_colorLabel);
    mainLayout->addWidget(_title);

    titleTabelViewLayout->addWidget(_allPlayerLabel,0,Qt::AlignHCenter);
    titleTabelViewLayout->addWidget(_gamePlayerLabel,0,Qt::AlignHCenter);

    tabelViewLayout->addWidget(_allPlayerTableView);
    tabelViewLayout->addLayout(addDeleteLayout);
    tabelViewLayout->addWidget(_gamePlayerTableView);

    addDeleteLayout->addWidget(_addPlayerForNewTournament);
    addDeleteLayout->addWidget(_deletePlayerForNewTournament);

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

    mainLayout->addLayout(maxPlayerLayout);
    mainLayout->addLayout(titleTabelViewLayout);
    mainLayout->addLayout(tabelViewLayout);
    mainLayout->addLayout(addPlayerLayout);

    widget->setLayout(mainLayout);
}

