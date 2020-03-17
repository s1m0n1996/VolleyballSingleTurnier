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
#include <QMenuBar>
#include <QGroupBox>
#include <QCalendarWidget>


#include "View/playerManagementWindow.h"
#include "Model/playerManagement.h"
#include "Model/player.h"
#include "Model/gameboard.h"

#include "View/tabelView.h"
#include "View/tournamentWindow.h"

#include "View/windowButton.h"
#include "View/windowEdit.h"
#include "View/windowLabel.h"

PlayermanagementWindow::PlayermanagementWindow(PlayerManagement* playerManagementModel, QWidget* parent) :
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
 * \param[in] void
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
 * \param[in] void
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
                modelAll->index(index.row(), 1).data().toDate(),
                modelAll->index(index.row(), 2).data().toString()));
    }
    _allPlayerTableView->selectionModel()->clearSelection();
}

/*!
 * \brief Löschen eines oder mehrere neuen Spieler zu einem Spiel
 *
 * \param[in] void
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
                modelGame->index(index.row(), 1).data().toDate(),
                modelGame->index(index.row(), 2).data().toString()));
    }

    for (Player& player: players)
    {
        _playerManagementModel->deletePlayerForNewGame(player);
    }

    _gamePlayerTableView->selectionModel()->clearSelection();
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
void PlayermanagementWindow::addPlayerToDatabase()
{

    Player* newPlayer = new Player(_playernameEdit->text(),
                                   QDate::fromString(_birthdayEdit->text(), "yyyy-MM-dd"), _countryEdit->text());
    _playerManagementModel->addPlayerForNewGame(*newPlayer);


    _playernameEdit->clear();
    _birthdayEdit->clear();
    _countryEdit->clear();

    // reset background color
    QPalette palette = _birthdayEdit->palette();
    palette.setColor(_birthdayEdit->backgroundRole(), Qt::white);
    _birthdayEdit->setToolTip("");
    _birthdayEdit->setAutoFillBackground(true);
    _birthdayEdit->setPalette(palette);



    _playerManagementModel->refreshDatabasePlayerTable();

    if (!(_byteArray->isEmpty()))
    {

        newPlayer->savePicture(*_byteArray);

        QByteArray data = newPlayer->loadPicture();
        QPixmap pixmap;

        pixmap.loadFromData(data, "jpg");

        int w = _photoLabel->width();
        int h = _photoLabel->height();

        QTransform rotate;
        rotate.rotate(90);

        pixmap = pixmap.scaled(w, h, Qt::KeepAspectRatio);
        pixmap = pixmap.transformed(rotate);

        _photo->setPixmap(pixmap);

        _byteArray->clear();
    }

}

void PlayermanagementWindow::createDeleteMenu()
{
    QAction* deletePlayer = new QAction("Löschen", this);
    _addPhotoAction       = new QAction("Foto hinzufügen");
    connect(deletePlayer, SIGNAL(triggered()), this, SLOT(deletePlayer()));
    connect(_addPhotoAction, SIGNAL(triggered()), this, SLOT(addPhotoWithSelection()));

    QMenu* deleteMenu = new QMenu(this);
    deleteMenu->addAction(deletePlayer);
    deleteMenu->addSeparator();
    deleteMenu->addAction(_addPhotoAction);

    deleteMenu->exec(QCursor::pos());
}

void PlayermanagementWindow::createRestoreMenu()
{
    QAction* restorePlayer = new QAction("Wiederherstellen", this);
    connect(restorePlayer, SIGNAL(triggered()), this, SLOT(restorePlayer()));

    QMenu* restoreMenu = new QMenu(this);
    restoreMenu->addAction(restorePlayer);

    restoreMenu->exec(QCursor::pos());
}

void PlayermanagementWindow::deletePlayer()
{
    QAbstractItemModel* modelGame = _allPlayerTableView->model();
    QModelIndexList selectedRows = _allPlayerTableView->selectionModel()->selectedRows();
    QList<Player> players;
    for (QModelIndex index : selectedRows)
    {
        players.append(Player(
                modelGame->index(index.row(), 0).data().toString(),
                modelGame->index(index.row(), 1).data().toDate(),
                modelGame->index(index.row(), 2).data().toString()));
    }

    for (Player& player: players)
    {
        _playerManagementModel->deletePlayerFromDatabase(player);
    }

    _allPlayerTableView->selectionModel()->clearSelection();

}

void PlayermanagementWindow::restorePlayer()
{
    QAbstractItemModel* deletedPlayersModel = _deletedPlayersTableView->model();
    QModelIndexList selectedRows = _deletedPlayersTableView->selectionModel()->selectedRows();
    QList<Player> players;
    for (QModelIndex index : selectedRows)
    {
        players.append(Player(
                deletedPlayersModel->index(index.row(), 0).data().toString(),
                deletedPlayersModel->index(index.row(), 1).data().toDate(),
                deletedPlayersModel->index(index.row(), 2).data().toString()));
    }

    for (Player& player: players)
    {
        _playerManagementModel->restorePlayerFromDatabase(player);
    }

    _allPlayerTableView->selectionModel()->clearSelection();

}

void PlayermanagementWindow::addPhotoWithSelection()
{
    QAbstractItemModel* modelAll = _allPlayerTableView->model();
    QModelIndexList selectedRows = _allPlayerTableView->selectionModel()->selectedRows();
    Player* activePlayer;
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Bild laden"), "",
                                                tr("Photo File (*.png, *.jpg) ;; All Files (*.*)"));


    for (QModelIndex index : selectedRows)
    {
        activePlayer = new Player(
                modelAll->index(index.row(), 0).data().toString(),
                modelAll->index(index.row(), 1).data().toDate(),
                modelAll->index(index.row(), 2).data().toString());
    }

    // save file in database
    QFile file(path);
    if (file.exists())
    {

        file.open(QIODevice::ReadOnly);
        QByteArray byteArray = file.readAll();
        activePlayer->savePicture(byteArray);
        file.close();
    }


}

void PlayermanagementWindow::addPhotoWithButton()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Bild laden"), "",
                                                tr("Photo File (*.png, *.jpg) ;; All Files (*.*)"));

    // save file in database
    QFile file(path);
    if (file.exists())
    {

        file.open(QIODevice::ReadOnly);
        *_byteArray = file.readAll();
        file.close();
    }

}

/*!
 * \brief Erstellt den Tournierplan
 *
 * \param[in] void
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
    gameManagement->tournamentChanged();
}

void PlayermanagementWindow::connecting()
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
    connect(_birthdayEdit, SIGNAL(textChanged(
                                          const QString &)), this, SLOT(enableAddPlayerButton()));
    connect(_birthdayEdit, SIGNAL(textChanged(
                                          const QString &)), this, SLOT(setPlayerDateBackground()));
    connect(_addPlayerButton, SIGNAL(released()), this, SLOT(addPlayerToDatabase()));
    connect(_addPhoto, SIGNAL(released()), this, SLOT(addPhotoWithButton()));


    connect(_startTournamentButton, SIGNAL(released()), this, SLOT(startTournament()));

    connect(_showDeletedPlayersAction, SIGNAL(triggered()), this, SLOT(showDeletedPlayers()));
}

void PlayermanagementWindow::enableAddPlayerButton()
{
    QDate date = QDate::fromString(_birthdayEdit->text(), "yyyy-MM-dd");
    QRegExp re(R"(^\d{4}\-(0[1-9]|1[012])\-(0[1-9]|[12][0-9]|3[01])$)");

    _addPlayerButton->setEnabled(!_countryEdit->text().isEmpty() && date.isValid() &&
                                 re.exactMatch(_birthdayEdit->text()));
}

/*!
 * \brief Färbe Hintergrund rot, wenn das Datum ungültig ist
 *
 * Der hintergrunf der Datumseingabe wird rot gefärbt und es wird ein Tooltip hinzugefügt, damit man erkennt das
 * das eingegebene Datum ungültig ist.
 * Wenn das Datum gültig ist wird der Hintergrund grün gefärbt.
 */
void PlayermanagementWindow::setPlayerDateBackground()
{
    QDate date = QDate::fromString(_birthdayEdit->text(), "yyyy-MM-dd");

    QPalette palette = _birthdayEdit->palette();

    if (date.isValid())
    {
        palette.setColor(_birthdayEdit->backgroundRole(), Qt::green);
        _birthdayEdit->setToolTip("");
    }
    else
    {
        palette.setColor(_birthdayEdit->backgroundRole(), Qt::red);
        _birthdayEdit->setToolTip("Ungültiges Datum");
    }

    _birthdayEdit->setAutoFillBackground(true);
    _birthdayEdit->setPalette(palette);
}

void PlayermanagementWindow::createWidges()
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


    _nameMissingPlayersLabel = new WindowLabel("noch zu benötigte Spieler:");
    _valueMissingPlayersLabel = new WindowLabel(
            QString::number(_playerManagementModel->countMissingPlayersForNewGame()));
    _valueMissingPlayersLabel->setNotStartTournamentStyle();

    _addPlayer = new QGroupBox();
    _addPlayer->setTitle("Spieler zum aktuellen Spiel hinzufügen:");
    _addPlayer->setStyleSheet("QGroupBox{"
                              "font-size: 25px;"
                              "font-family: Candara;} ");


    _playernameEdit = new WindowEdit("Max Mustermann", DataType::name);
    _playernameEdit->setMaxLength(20);
    _birthdayEdit = new WindowEdit("1990-01-30", DataType::date);
    _countryEdit = new WindowEdit("DE", DataType::country);
    _countryEdit->setMaxLength(3);

    _playernameLabel    = new WindowLabel("Spielername");
    _birthdayLabel      = new WindowLabel("Geburtsdatum");
    _countryLabel       = new WindowLabel("Land");
    _photoLabel         = new WindowLabel("Foto");

    _photo = new WindowLabel("");
    _calendar = new QCalendarWidget;
    _calendarButton= new WindowButton("");

    _addPhoto = new WindowButton("Foto hinzufügen");
    _addPhoto->setIcon(QIcon(":/img/addPhoto.png"));
    _addPhoto->setIconSize(QSize(33, 33));

    _addPlayerButton = new WindowButton("Spieler hinzufügen");
    _addPlayerButton->setIcon(QIcon(":/img/addPlayer.png"));
    _addPlayerButton->setIconSize(QSize(40, 40));
    _addPlayerButton->setEnabled(false);


    _startTournamentButton = new WindowButton("Turnier starten");
    _startTournamentButton->setEnabled(false);
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

    _byteArray = new QByteArray();
}

void PlayermanagementWindow::showTable()
{
    _allPlayerTableView  = new TableView;
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

void PlayermanagementWindow::setAllLayout()
{
    QWidget* widget = new QWidget;
    setCentralWidget(widget);

    QVBoxLayout* mainLayout             = new QVBoxLayout;
    QHBoxLayout* titleTabelViewLayout   = new QHBoxLayout;
    QVBoxLayout* maxPlayerLayout        = new QVBoxLayout;
    QHBoxLayout* tabelViewLayout        = new QHBoxLayout;
    QVBoxLayout* addDeleteLayout        = new QVBoxLayout;        
    QHBoxLayout* bottomLayout           = new QHBoxLayout;
    QVBoxLayout* labelLayout            = new QVBoxLayout;
    QVBoxLayout* editLayout             = new QVBoxLayout;
    QGridLayout* addPlayerLayout        = new QGridLayout;
    QGridLayout* tournamentStartLayout  = new QGridLayout;


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

    editLayout->addWidget(_playernameEdit);
    editLayout->addWidget(_birthdayEdit);
    editLayout->addWidget(_calendar);
    editLayout->addWidget(_calendarButton);
    editLayout->addWidget(_countryEdit);
    editLayout->addWidget(_addPhoto);
    editLayout->addWidget(_photo);

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

void PlayermanagementWindow::showDeletedPlayers(void)
{
    _deletedPlayersTableView->show();
}
