#include "volleyball_game_plane.h"
#include "ui_volleyball_game_plane.h"

#include "Model/gameManagement.h"
#include <QTableWidgetItem>
#include <QTextEdit>

#include <QDebug>

VolleyballGamePlane::VolleyballGamePlane(CreateVolleyballGameBoard* model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VolleyballGamePlane)
{
    ui->setupUi(this);

    _volleyballGameBoardModel = model;
    _gameManagement = &GameManagement::instance();

    _connect();
    refreshCountGamesPerPlayer();
    setCountGamesPerPlayerTable();
    refreshGameBoard();
}

VolleyballGamePlane::~VolleyballGamePlane()
{
    delete ui;
}


void VolleyballGamePlane::refreshCountGamesPerPlayer()
{
    const QString sqlPrepare = R"(
SELECT pl.name, count(*) AS count
FROM game_player_list INNER JOIN player_list pl ON pl.id = game_player_list.player_id
WHERE sport_type_id = :sportTypeId
  AND game_mode_id = :gameModeId
  AND tournament_id = :tournamentId
GROUP BY player_id
ORDER BY pl.name
)";
    QSqlQuery sqlQuery;
    sqlQuery.prepare(sqlPrepare);
    sqlQuery.bindValue(":sportTypeId", _gameManagement->getSportTypeId());
    sqlQuery.bindValue(":gameModeId", _gameManagement->getGameModeId());
    sqlQuery.bindValue(":tournamentId", _gameManagement->getTournamentId());
    sqlQuery.exec();

    _countGamesPerPlayerTableModel->setQuery(sqlQuery);

    _countGamesPerPlayerTableModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    _countGamesPerPlayerTableModel->setHeaderData(1, Qt::Horizontal, tr("Spiele"));
}

void VolleyballGamePlane::setCountGamesPerPlayerTable()
{
    ui->playedGamesTable->setModel(_countGamesPerPlayerTableModel);
    ui->playedGamesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->playedGamesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void VolleyballGamePlane::createNewGame()
{
    const int cGames = ui->nNewGames->value();
    ui->nNewGames->setValue(1);

    for (int i = 0 ; i < cGames; i++)
    {
        _volleyballGameBoardModel->createOneGame();
    }
}

void VolleyballGamePlane::_connect()
{
    connect(ui->createGamesButton, SIGNAL(released()), this, SLOT(createNewGame()));
    connect(_volleyballGameBoardModel, SIGNAL(refreshGameBoard()), this, SLOT(refreshCountGamesPerPlayer()));
    connect(_volleyballGameBoardModel, SIGNAL(refreshGameBoard()), this, SLOT(refreshGameBoard()));
}

void VolleyballGamePlane::refreshGameBoard()
{
    QMap<int, QMap<int, QString>> games =  _volleyballGameBoardModel->getGames();

    ui->gamesTableWidget->clear();
    ui->gamesTableWidget->clear();

    while (ui->gamesTableWidget->columnCount() < 2)
    {
        ui->gamesTableWidget->insertColumn(0);
    }

    for (const int gameId : games.keys())
    {
        if (ui->gamesTableWidget->rowCount() < gameId)
        {
            ui->gamesTableWidget->insertRow( ui->gamesTableWidget->rowCount());
        }

        auto* teamA = new QTableWidgetItem(games[gameId][1]);
        auto* teamB = new QTableWidgetItem(games[gameId][2]);

        ui->gamesTableWidget->setItem(gameId - 1, 0, teamA);
        ui->gamesTableWidget->setItem(gameId - 1, 1, teamB);

        ui->gamesTableWidget->setRowHeight(gameId - 1, 80);
    }

    ui->gamesTableWidget->show();
}
