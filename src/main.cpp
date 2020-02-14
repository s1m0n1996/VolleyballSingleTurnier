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


void test(){

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model* model = new Model();

    MainMenu* mainmenu= new MainMenu(model);
//    mainmenu->setFixedSize(700,600);
    mainmenu -> show();

    return a.exec();
}
