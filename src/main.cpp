/*!
* \file main.cpp
* \brief main
* \author Nico Meyer
*/

#include "View/mainmenu.h"
#include <QApplication>


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainMenu* mainMenu = new MainMenu();
    mainMenu->show();

    return a.exec();
}
