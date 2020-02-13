#ifndef MAINMENU_H
#define MAINMENU_H

#include "ui_mainmenu.h"
#include "Model/model.h"
#include "Model/sqliteConnector.h"

#include "View/menubutton.h"
#include "View/playermanagementWindow.h"
#include "View/refereewindow.h"
#include "View/tournementwindow.h"
#include "View/viewerwindow.h"
#include "View/windowlabel.h"
#include "Model/playermanagement.h"

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



QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE



class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(Model* model, QMainWindow* parent = nullptr);
    ~MainMenu();

public slots:
    void openPlayermanagmentWindow();
    void openTournementWindow();
    void openViewerWindow();
    void openRefereeWindow();
    void createDatabase();
    void loadDatabase();


private:
    void connecting();
    void createButton();
    void setButtonsLayout();
    Ui::MainMenu *ui;
    Model* _model = nullptr;
    WindowLabel* _note = nullptr;
    MenuButton* _playermanagment = nullptr;
    MenuButton* _tournement = nullptr;
    MenuButton* _viewer = nullptr;
    MenuButton* _referee = nullptr;
    QAction* _load = nullptr;
    QAction* _new = nullptr;
    PlayerManagement* _playerManagementModel;
    Referee* _refereeModel;


};
#endif // MAINMENU_H
