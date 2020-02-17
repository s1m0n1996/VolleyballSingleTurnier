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
    QList<Player> list;
    for(const Player& pla : list)
    {

    }
    Game test(65);
    test.prepareNextGame(7);

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model* model = new Model();

    MainMenu* mainmenu= new MainMenu(model);
//    mainmenu->setFixedSize(700,600);
    mainmenu -> show();
    test();

    return a.exec();
}
