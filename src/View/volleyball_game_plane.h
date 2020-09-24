#ifndef VOLLEYBALL_GAME_PLANE_H
#define VOLLEYBALL_GAME_PLANE_H

#include <QMainWindow>
#include <QtSql/QSqlQueryModel>

class GameManagement;

namespace Ui {
class VolleyballGamePlane;
}

class VolleyballGamePlane : public QMainWindow
{
    Q_OBJECT

public:
    explicit VolleyballGamePlane(QWidget *parent = nullptr);
    ~VolleyballGamePlane();

private slots:
    void refreshCountGamesPerPlayer();
private:
    Ui::VolleyballGamePlane *ui;

    void setCountGamesPerPlayerTable();

    QSqlQueryModel* _countGamesPerPlayerTableModel = new QSqlQueryModel();
    GameManagement* _gameManagement;
};

#endif // VOLLEYBALL_GAME_PLANE_H
