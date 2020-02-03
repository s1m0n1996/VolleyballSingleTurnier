#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <Model/model.h>
#include "View/menubutton.h"

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
//    void openWindow();
    void openPlayermanagmentWindow();
    void openTournementWindow();
    void openViewerWindow();
    void openRefereeWindow();
    void createButton();
    void setButtonsLayout();

private:
    Ui::MainMenu *ui;
    Model* _model = nullptr;
    MenuButton* _playermanagment = nullptr;
    MenuButton* _tournement = nullptr;
    MenuButton* _viewer = nullptr;
    MenuButton* _referee = nullptr;
    QLabel* _loadSave = nullptr;
    QPushButton* _load = nullptr;
    QPushButton* _save = nullptr;


};
#endif // MAINMENU_H
