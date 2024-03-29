#include <QAction>
#include <QAbstractItemView>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QMenuBar>
#include <QMenu>
#include <QModelIndex>

#include "Model/gameboard.h"
#include "Model/player.h"
#include "Model/playerManagement.h"

#include "View/playerManagementWindow.h"
#include "View/tabelView.h"
#include "View/tournamentWindow.h"
#include "View/windowButton.h"
#include "View/windowEdit.h"
#include "View/windowLabel.h"

/*!
 * \brief PlayermanagementWindow::PlayermanagementWindow ist Konstruktor der Klasse PlayermanagementWindow
 * \param[in] playerManagementModel ist das dazugehörige Model
 * \param[in] parent bietet die Möglichkeit ein Objekt abhänig vom rufenden Objekt zu erzeugen
 * \return void
 *
 * Das Fenster wird im Konstruktor erstellt. Ein Icon wird gesetzt, die Widgets werden gesetzt
 * und in das Layout eingefügt.
 * Die Connections werden auch im Konstruktor gesetzt
 */
PlayermanagementWindow::PlayermanagementWindow(PlayerManagement* playerManagementModel, QWidget* parent) :
        QMainWindow(parent)
{
    setWindowIcon(QIcon(":/img/favicon.png"));
    setWindowTitle("Meldestelle");

    _playerManagementModel = playerManagementModel;
    _playerManagementModel->refreshDatabasePlayerTable();
    _playerManagementModel->refreshNextGamePlayerTable();

    createWidgets();
    setAllLayout();
    connecting();

}

PlayermanagementWindow::~PlayermanagementWindow(void)
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
    delete _playernameEdit;;
    delete _countryEdit;
    delete _allPlayerTableView;
    delete _gamePlayerTableView;

}

/*!
 * \brief Anzeige der benötigten Spieler zum Turnierbeginn
 *
 * \param[in] void
 * \return void
 *
 * Das Label mit der Überschrift und der Spieleranzahl wird erzeugt, und aktualisiert
 * Der Button zum Start wird, freigeschlatet oder bleibt nicht drückbar, wenn die benötigte Spieleranzahl Null beträgt
 */
void PlayermanagementWindow::setMissingPlayersForNewTournamentLabel(void)
{
    _valueMissingPlayersLabel->setText(QString::number(_playerManagementModel->countMissingPlayersForNewGame()));
    //_startTournamentButton->setEnabled(false);
    _valueMissingPlayersLabel->setNotStartTournamentStyle();

    if (_valueMissingPlayersLabel->text() == "0")
    {
        _valueMissingPlayersLabel->setStartTournamentStyle();
        _startTournamentButton->setEnabled(true);
    }
}

/*!
 * \brief Einfügen eines neuen Spieler zu gesamten Spielerdatenbank
 *
 * \param[in] void
 * \return void
 *
 * Ein Objekt der Klasse Player wird erzeugt, dem erden die neuen Varibalen mitgegeben
 * Dieser wird dann sowohl dem aktuellen Spiel als auch der gesamten Spielerdatenabnk hinzugefügt
 * Die Edits der Namen,Geburtstag und Land werden wieder frei beschreibbar gemacht
 */
void PlayermanagementWindow::addPlayerToDatabase(void)
{
    Player* newPlayer = new Player(_playernameEdit->text(), _birthday->date(), _countryEdit->text(), _genderEdit->text());
    _playerManagementModel->addPlayerForNewGame(*newPlayer);


    _playernameEdit->clear();
    _genderEdit->clear();
    _birthday->setDate(QDate(1990, 10, 21));
    _countryEdit->clear();

    _playerManagementModel->refreshDatabasePlayerTable();

    if (!(_playerPicture->isEmpty()))
    {
        newPlayer->savePicture(*_playerPicture);
    }
    _playerPicture->clear();

}

/*!
 * \brief Ein Foto zu einem Spieler beim erstellen hinzufügen
 *
 * \param[in] void
 * \return void
 *
 * Das in ein ByteArray gespeicherte Foto wird dem gerade erzeugten Spieler sofort hinzugefügt
 */
void PlayermanagementWindow::addPhotoWithButton(void)
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Bild laden"), "",
                                                tr("Photo File (*.png, *.jpg) ;; All Files (*.*)"));

    // save file in database
    QFile file(path);
    if (file.exists())
    {

        file.open(QIODevice::ReadOnly);
        *_playerPicture = file.readAll();
        file.close();
    }

}

/*!
 * \brief Einfügen eines oder mehrere neuen Spieler zu einem Spiel
 *
 * \param[in] void
 * \return void
 *
 * Die Indices der markierten Reihen werden dem Model übergeben und dort weiter verarbeitet
 * Die ausgewählten Reihen werden wieder nicht markiert angezeigt
 */
void PlayermanagementWindow::addPlayerForNewGame(void)
{
    QAbstractItemModel* modelAll = _allPlayerTableView->model();
    QModelIndexList selectedRows = _allPlayerTableView->selectionModel()->selectedRows();

    for (QModelIndex index : selectedRows)
    {
        _playerManagementModel->addPlayerForNewGame(Player(
                modelAll->index(index.row(), 0).data().toString(),
                modelAll->index(index.row(), 1).data().toDate(),
                modelAll->index(index.row(), 2).data().toString(),
                modelAll->index(index.row(), 3).data().toString()

                ));
    }
    _allPlayerTableView->selectionModel()->clearSelection();
}

/*!
 * \brief Löschen eines oder mehrere neuen Spieler zu einem Spiel
 *
 * \param[in] void
 * \return void
 *
 * Die Indices der markierten Reihen werden dem Model übergeben und dort weiter verarbeitet
 * Die ausgewählten Reihen werden wieder nicht markiert angezeigt
 */
void PlayermanagementWindow::dropPlayerForNewGame(void)
{
    QAbstractItemModel* modelGame = _gamePlayerTableView->model();
    QModelIndexList selectedRows = _gamePlayerTableView->selectionModel()->selectedRows();
    QList<Player> players;
    for (QModelIndex index : selectedRows)
    {
        players.append(Player(
                modelGame->index(index.row(), 0).data().toString(),
                modelGame->index(index.row(), 1).data().toDate(),
                modelGame->index(index.row(), 2).data().toString(),
                modelGame->index(index.row(), 3).data().toString()
                ));
    }

    for (Player& player: players)
    {
        _playerManagementModel->deletePlayerForNewGame(player);
    }

    _gamePlayerTableView->selectionModel()->clearSelection();
}

/*!
 * \brief Löschen eines Spielers
 *
 * \param[in] void
 * \return void
 *
 * Alle markierten Splaten werden einem neuen Player Objekt übergebn, welcher dann über eine Funtkion aus der
 * Model gelöscht wird
 */
void PlayermanagementWindow::deletePlayer(void)
{
    QAbstractItemModel* modelGame = _allPlayerTableView->model();
    QModelIndexList selectedRows = _allPlayerTableView->selectionModel()->selectedRows();
    QList<Player> players;
    for (QModelIndex index : selectedRows)
    {
        players.append(Player(
                modelGame->index(index.row(), 0).data().toString(),
                modelGame->index(index.row(), 1).data().toDate(),
                modelGame->index(index.row(), 2).data().toString(),
                modelGame->index(index.row(), 3).data().toString()
                ));
    }

    for (Player& player: players)
    {
        _playerManagementModel->deletePlayerFromDatabase(player);
    }

    _allPlayerTableView->selectionModel()->clearSelection();

}

/*!
 * \brief Wiederherstellen eines gelöschten Spielers
 *
 * \param[in] void
 * \return void
 *
 *
 */
void PlayermanagementWindow::restorePlayer(void)
{
    QAbstractItemModel* deletedPlayersModel = _deletedPlayersTableView->model();
    QModelIndexList selectedRows = _deletedPlayersTableView->selectionModel()->selectedRows();
    QList<Player> players;
    for (QModelIndex index : selectedRows)
    {
        players.append(Player(
                deletedPlayersModel->index(index.row(), 0).data().toString(),
                deletedPlayersModel->index(index.row(), 1).data().toDate(),
                deletedPlayersModel->index(index.row(), 2).data().toString(),
                deletedPlayersModel->index(index.row(), 3).data().toString()
                ));
    }

    for (Player& player: players)
    {
        _playerManagementModel->restorePlayerFromDatabase(player);
    }

    _allPlayerTableView->selectionModel()->clearSelection();
}

/*!
 * \brief Ein Foto zu einem Spieler über Auswahl in der Tabelle hinzufügen
 *
 * \param[in] void
 * \return void
 *
 * Durch makieren eines Spielers wird das ausgewählte Foto, welches in einem Bytearray gespeichert ist, diesem Spieler hinzugefügt
 */
void PlayermanagementWindow::addPhotoWithSelection(void)
{
    QAbstractItemModel* modelAll = _allPlayerTableView->model();
    QModelIndexList selectedRows = _allPlayerTableView->selectionModel()->selectedRows();
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Bild laden"), "",
                                                tr("Photo File (*.png, *.jpg) ;; All Files (*.*)"));

    if (!selectedRows.isEmpty())
    {
        Player player(
                modelAll->index(selectedRows[0].row(), 0).data().toString(),
                modelAll->index(selectedRows[0].row(), 1).data().toDate(),
                modelAll->index(selectedRows[0].row(), 2).data().toString(),
                modelAll->index(selectedRows[0].row(), 3).data().toString()
        );
        // save file in database
        QFile file(path);
        if (file.exists())
        {
            file.open(QIODevice::ReadOnly);
            QByteArray byteArray = file.readAll();
            player.savePicture(byteArray);
            file.close();
        }
    }
}

/*!
 * \brief Erstellt den Tournierplan
 *
 * \param[in] void
 * \return void
 *
 * Diese Methode startet das eigentiche Turnier. Dadurh wird der Spielplan erstellt und das Spiel geht in den
 * Zustand, das es gestartet ist.
 */
void PlayermanagementWindow::startTournament(void)
{
    //Gameboard gameBoard(_playerManagementModel->getPlayersForNextGame());
    window()->close();
    GameManagement* gameManagement = &GameManagement::instance();
    gameManagement->tournamentChanged();
}

void PlayermanagementWindow::enableAddPlayerButton(void)
{
    _addPlayerButton->setEnabled(!_countryEdit->text().isEmpty() && !_genderEdit->text().isEmpty());
}

void PlayermanagementWindow::showDeletedPlayers(void)
{
    _deletedPlayersTableView->show();
}

void PlayermanagementWindow::createDeleteMenu(void)
{
    QAction* deletePlayer = new QAction("Löschen", this);
    _addPhotoAction = new QAction("Foto hinzufügen");
    connect(deletePlayer, SIGNAL(triggered()), this, SLOT(deletePlayer()));
    connect(_addPhotoAction, SIGNAL(triggered()), this, SLOT(addPhotoWithSelection()));

    QMenu* deleteMenu = new QMenu(this);
    deleteMenu->addAction(deletePlayer);
    deleteMenu->addSeparator();
    deleteMenu->addAction(_addPhotoAction);

    deleteMenu->exec(QCursor::pos());
}

void PlayermanagementWindow::createRestoreMenu(void)
{
    QAction* restorePlayer = new QAction("Wiederherstellen", this);
    connect(restorePlayer, SIGNAL(triggered()), this, SLOT(restorePlayer()));

    QMenu* restoreMenu = new QMenu(this);
    restoreMenu->addAction(restorePlayer);

    restoreMenu->exec(QCursor::pos());
}

void PlayermanagementWindow::createWidgets(void)
{
    _title = new WindowLabel("Meldestelle");
    _title->setMainTitleStyle();

    _allPlayerLabel = new WindowLabel("gesamte Spieler");
    _allPlayerLabel->setTitleStyle();

    _gamePlayerLabel = new WindowLabel("aktuelle Spieler im Turnier");
    _gamePlayerLabel->setTitleStyle();

    showTable();
    _addPlayerForNewTournament = new WindowButton("");
    _addPlayerForNewTournament->setIcon(QIcon(":/img/right.png"));
    _addPlayerForNewTournament->setIconSize(QSize(65, 65));

    _deletePlayerForNewTournament = new WindowButton("");
    _deletePlayerForNewTournament->setIcon(QIcon(":/img/left.png"));
    _deletePlayerForNewTournament->setIconSize(QSize(65, 65));


    _nameMissingPlayersLabel = new WindowLabel("Anzahl Spieler:");
    _valueMissingPlayersLabel = new WindowLabel(
            QString::number(_playerManagementModel->countMissingPlayersForNewGame()));
    _valueMissingPlayersLabel->setNotStartTournamentStyle();

    _addPlayer = new QGroupBox();
    _addPlayer->setTitle("Spieler zum aktuellen Spiel hinzufügen:");
    _addPlayer->setStyleSheet("QGroupBox{"
                              "font-size: 25px;"
                              "font-family: Arial;} ");


    _playernameEdit = new WindowEdit("Max Mustermann", DataType::name);
    _playernameEdit->setMaxLength(20);

    _genderEdit = new WindowEdit("m/w", DataType::name);
    _genderEdit->setMaxLength(20);

    _countryEdit = new WindowEdit("DE", DataType::country);
    _countryEdit->setMaxLength(3);

    _playernameLabel = new WindowLabel("Spielername");
    _birthdayLabel = new WindowLabel("Geburtsdatum");
    _countryLabel = new WindowLabel("Land");
    _photoLabel = new WindowLabel("Foto");
    _genderLabel = new WindowLabel("Geschlecht");


    _birthday = new QDateEdit();
    _birthday->setCalendarPopup(true);
    _birthday->setStyleSheet("QDateEdit::drop-down {"
                             "image:url(:/img/calendar.png);"
                             "spacing:5px;"
                             "width:60px;"
                             "height:25px;"
                             "subcontrol-position: right top;"
                             "subcontrol-origin:margin; }"
                             "QDateEdit{"
                             "font-size: 20px;"
                             "font-family: Arial;"
                             "spacing: 5px;}"
                             "QCalendarWidget{"
                             "font-size: 20px;"
                             "font-family: Arial;}");
    _birthday->setDate(QDate(1990, 10, 21));

    _addPhoto = new WindowButton("Foto hinzufügen");
    _addPhoto->setIcon(QIcon(":/img/addPhoto.png"));
    _addPhoto->setIconSize(QSize(33, 33));

    _addPlayerButton = new WindowButton("Spieler hinzufügen");
    _addPlayerButton->setIcon(QIcon(":/img/addPlayer.png"));
    _addPlayerButton->setIconSize(QSize(40, 40));
    _addPlayerButton->setEnabled(false);


    _startTournamentButton = new WindowButton("Turnier starten");
    //_startTournamentButton->setEnabled(false);
    _startTournamentButton->setEnableStyle();
    _startTournamentButton->setIcon(QIcon(":/img/createDart.png"));
    _startTournamentButton->setIconSize(QSize(65, 65));
    _startTournamentButton->setFixedSize(250, 40);


    _menuDelete = new QMenu();
    _menuDelete = menuBar()->addMenu(tr("Spieler"));

    _showDeletedPlayersAction = new QAction("Gelöschte Spieler");


    _menuDelete->addAction(_showDeletedPlayersAction);


    if (_valueMissingPlayersLabel->text() == "0")
    {
        _valueMissingPlayersLabel->setStartTournamentStyle();
        _startTournamentButton->setEnabled(true);
    }

    _playerPicture = new QByteArray();
}

void PlayermanagementWindow::showTable(void)
{
    _allPlayerTableView = new TableView;
    _gamePlayerTableView = new TableView;
    _deletedPlayersTableView = new TableView;

    _allPlayerTableView->setModel(_playerManagementModel->getDatabaseTableModel());
    _allPlayerTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _allPlayerTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _allPlayerTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    _gamePlayerTableView->setModel(_playerManagementModel->getNextGamePlayerTableModel());
    _gamePlayerTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _gamePlayerTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _gamePlayerTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    _deletedPlayersTableView->setModel(_playerManagementModel->getDeletedPlayerTableModel());
    _deletedPlayersTableView->setWindowTitle("Gelöschte Spieler");
    _deletedPlayersTableView->setMinimumWidth(500);
    _deletedPlayersTableView->setMinimumHeight(200);
    _deletedPlayersTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _deletedPlayersTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _deletedPlayersTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void PlayermanagementWindow::setAllLayout(void)
{
    QWidget* widget = new QWidget;
    setCentralWidget(widget);
    widget->setStyleSheet("background: white;");

    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* titleTabelViewLayout = new QHBoxLayout;
    QVBoxLayout* maxPlayerLayout = new QVBoxLayout;
    QHBoxLayout* tabelViewLayout = new QHBoxLayout;
    QVBoxLayout* addDeleteLayout = new QVBoxLayout;
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    QVBoxLayout* labelLayout = new QVBoxLayout;
    QVBoxLayout* editLayout = new QVBoxLayout;
    QGridLayout* addPlayerLayout = new QGridLayout;
    QGridLayout* tournamentStartLayout = new QGridLayout;

    mainLayout->addWidget(_title);

    titleTabelViewLayout->addWidget(_allPlayerLabel, 0, Qt::AlignHCenter);
    titleTabelViewLayout->addWidget(_gamePlayerLabel, 0, Qt::AlignHCenter);

    tabelViewLayout->addWidget(_allPlayerTableView);
    tabelViewLayout->addLayout(addDeleteLayout);
    tabelViewLayout->addWidget(_gamePlayerTableView);

    addDeleteLayout->addWidget(_addPlayerForNewTournament);
    addDeleteLayout->addWidget(_deletePlayerForNewTournament);

    maxPlayerLayout->addWidget(_nameMissingPlayersLabel, 0, Qt::AlignCenter);
    maxPlayerLayout->addWidget(_valueMissingPlayersLabel, 0, Qt::AlignCenter);

    labelLayout->addWidget(_playernameLabel);
    labelLayout->addWidget(_birthdayLabel);
    labelLayout->addWidget(_countryLabel);
    labelLayout->addWidget(_photoLabel);
    labelLayout->addWidget(_genderLabel);

    editLayout->addWidget(_playernameEdit);
    editLayout->addWidget(_birthday);
    editLayout->addWidget(_countryEdit);
    editLayout->addWidget(_addPhoto);
    editLayout->addWidget(_genderEdit);

    addPlayerLayout->addLayout(labelLayout, 0, 0);
    addPlayerLayout->addLayout(editLayout, 0, 1);
    addPlayerLayout->addWidget(_addPlayerButton, 2, Qt::AlignRight);

    _addPlayer->setLayout(addPlayerLayout);

    tournamentStartLayout->addWidget(_startTournamentButton, Qt::AlignRight, Qt::AlignBottom, Qt::AlignBottom);

    bottomLayout->addWidget(_addPlayer);
    bottomLayout->addStretch();
    bottomLayout->addLayout(tournamentStartLayout);

    mainLayout->addLayout(maxPlayerLayout);
    mainLayout->addLayout(titleTabelViewLayout);
    mainLayout->addLayout(tabelViewLayout);
    mainLayout->addLayout(bottomLayout);

    widget->setLayout(mainLayout);
}

void PlayermanagementWindow::connecting(void)
{
    connect(_playerManagementModel, SIGNAL(valueChanged()), this, SLOT(setMissingPlayersForNewTournamentLabel()));

    connect(_addPlayerForNewTournament, SIGNAL(released()), this, SLOT(addPlayerForNewGame()));
    connect(_deletePlayerForNewTournament, SIGNAL(released()), this, SLOT(dropPlayerForNewGame()));
    connect(_allPlayerTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createDeleteMenu()));
    connect(_deletedPlayersTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createRestoreMenu()));

    connect(_playernameEdit, SIGNAL(textChanged(
                                            const QString &)), this, SLOT(enableAddPlayerButton()));
    connect(_countryEdit, SIGNAL(textChanged(
                                         const QString &)), this, SLOT(enableAddPlayerButton()));
    connect(_genderEdit, SIGNAL(textChanged(
                                         const QString &)), this, SLOT(enableAddPlayerButton()));

    connect(_addPlayerButton, SIGNAL(released()), this, SLOT(addPlayerToDatabase()));
    connect(_addPhoto, SIGNAL(released()), this, SLOT(addPhotoWithButton()));


    connect(_startTournamentButton, SIGNAL(released()), this, SLOT(startTournament()));

    connect(_showDeletedPlayersAction, SIGNAL(triggered()), this, SLOT(showDeletedPlayers()));
}
