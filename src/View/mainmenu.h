#ifndef MAINMENU_H
#define MAINMENU_H

#include "ui_mainmenu.h"
#include "Model/model.h"
#include "Model/sqliteConnector.h"

#include "View/menubutton.h"
#include "View/playermanagementWindow.h"
#include "View/refereewindow.h"
#include "View/tournamentwindow.h"
#include "View/viewerwindow.h"
#include "View/windowlabel.h"
#include "Model/playermanagement.h"
#include "View/refereepopupwinningleg.h"

#include <QPalette>
#include <QAction>
#include <QCoreApplication>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPalette>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>



QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE



class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(Model* model, QMainWindow* parent = nullptr);
    ~MainMenu();
    WindowLabel* getNote(){return _noteDatabase;}

public slots:
    void openPlayermanagmentWindow();
    void openTournamentWindow();
    void openViewerWindow();
    void openRefereeWindow();
    void createDatabase();
    void loadDatabase();
    void tournamentName();
    void setTouenamentName();


private:
    void connecting();
    void createWidgets();
    void setwholeLayout();
    Ui::MainMenu *ui;
    Model* _model = nullptr;
    WindowLabel* _noteDatabase = nullptr;
    WindowLabel* _noteTournament = nullptr;
    WindowLabel* _title = nullptr;
    MenuButton* _playermanagment = nullptr;
    MenuButton* _tournament = nullptr;
    MenuButton* _viewer = nullptr;
    MenuButton* _referee = nullptr;
    QAction* _loadPlayer = nullptr;
    QAction* _newPlayer = nullptr;
    QAction* _loadTournament = nullptr;
    QAction* _newTournament = nullptr;
    PlayerManagement* _playerManagementModel;
    Referee* _refereeModel;
    RefereepopupWinningLeg* _refereePopupModel;
    TournamentNamePopUp* _tournamentName = nullptr;
    QMenu* _playerData = nullptr;
    QMenu* _tournamentData= nullptr;





};
#endif // MAINMENU_H
