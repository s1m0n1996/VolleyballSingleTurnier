#include "volleyball_game_plane.h"
#include "ui_volleyball_game_plane.h"

#include "Model/gameManagement.h"

#include <QDebug>

VolleyballGamePlane::VolleyballGamePlane(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VolleyballGamePlane)
{
    ui->setupUi(this);
    _gameManagement = &GameManagement::instance();
    refreshCountGamesPerPlayer();
    setCountGamesPerPlayerTable();

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
ORDER BY count
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
