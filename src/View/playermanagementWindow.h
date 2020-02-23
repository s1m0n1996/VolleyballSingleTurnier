#ifndef PLAYERMANAGMENT_H
#define PLAYERMANAGMENT_H

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QList>


#include "ui_playermanagementWindow.h"
#include "windowbutton.h"
#include "windowedit.h"
#include "windowlabel.h"
#include "View/tournamentnamepopup.h"
#include "Model/playermanagement.h"
#include "View/tabelview.h"

#include <cmath>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class PlayermanagementWindow; }
QT_END_NAMESPACE

class PlayermanagementWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayermanagementWindow(PlayerManagement* playerManagementModel, QWidget *parent = nullptr);
    ~PlayermanagementWindow();


signals:
public slots:
    void addPlayerToDatabase(void);
    void setMissingPlayersForNewTournamentLabel(void);
    void tournamentName(void);
    void showTable(void);
    void addPlayerForNewGame(void);
    void dropPlayerForNewGame(void);

private:
    void connecting();
    void createAddPlayerEdit();
    void createButtons(void);
    void createMissingPlayersForNewTournamentLabel();
    void setAllLayout(void);

    Ui::PlayermanagementWindow *ui;
    WindowButton* _addPlayerButton          = nullptr;
    WindowButton* _startTournamentButton    = nullptr;
    WindowButton* _addPlayerForNewTournament                      = nullptr;
    WindowButton* _deletePlayerForNewTournament                   = nullptr;
    WindowLabel* _playernameLabel           = nullptr;
    WindowLabel* _birthdayLabel             = nullptr;
    WindowLabel* _countryLabel              = nullptr;
    WindowLabel* _nameMissingPlayersLabel        = nullptr;
    WindowLabel* _valueMissingPlayersLabel       = nullptr;
    WindowEdit* _playernameEdit             = nullptr;
    WindowEdit* _birthdayEdit               = nullptr;
    WindowEdit* _countryEdit                = nullptr;
    PlayerManagement* _playerManagementModel                = nullptr;
    TableView* _allPlayerTableView          = nullptr;
    TableView* _gamePlayerTableView         = nullptr;
    QLabel* _colorLabel         = nullptr;
    WindowLabel* _title = nullptr;
    WindowLabel* _photo= nullptr;
    WindowButton* _addPhoto = nullptr;

};
#endif // PLAYERMANAGMENT_H
