#ifndef VOLLEYBALL_GAME_PLANE_H
#define VOLLEYBALL_GAME_PLANE_H

#include <QMainWindow>
#include <QtSql/QSqlQueryModel>
#include <Model/create_volleyball_game_board.h>
#include "View/volleyball_game_plane.h"

class GameManagement;

namespace Ui {
class VolleyballGamePlane;
}

class VolleyballGamePlane : public QMainWindow
{
    Q_OBJECT

public:
    explicit VolleyballGamePlane(CreateVolleyballGameBoard* model, QWidget *parent = nullptr);
    ~VolleyballGamePlane();

private slots:
    void refreshCountGamesPerPlayer();
    void refreshGameBoard();
    void createNewGame();
private:
    Ui::VolleyballGamePlane *ui;
    void _connect();

    void setCountGamesPerPlayerTable();

    QSqlQueryModel* _countGamesPerPlayerTableModel = new QSqlQueryModel();
    GameManagement* _gameManagement;
    CreateVolleyballGameBoard* _volleyballGameBoardModel;
};

#endif // VOLLEYBALL_GAME_PLANE_H
