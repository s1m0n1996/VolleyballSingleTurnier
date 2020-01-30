#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QWidget>
#include <QDialog>
#include <Model/model.h>

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
    void openWindow(void);
    void modifiButton(void);

private:
    Ui::MainMenu *ui;
    Model* _model = nullptr;
};
#endif // MAINMENU_H
