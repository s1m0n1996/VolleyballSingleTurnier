/*!
* \file main.cpp
* \brief main
* \author Nico Meyer
*/

#include "View/mainmenu.h"
#include "Model/model.h"
#include "View/dartboard.h"
#include "View/refereewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model* model = new Model();

  //  mainWindow->setWindowState(Qt::WindowMaximized); // Startet das Fenster von anfang an in Vollbild
    //mainWindow->show();
    MainMenu* mainmenu = new MainMenu;
    mainmenu -> show();

    RefereeWindow* test = new RefereeWindow();
    test->show();



    return a.exec();
}
