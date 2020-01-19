/*!
* \file main.cpp
* \brief main
* \author Nico Meyer
*/

#include "mainwindow.h"
#include "Model/model.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model* model = new Model();
    MainWindow* mainWindow = new MainWindow(model);
  //  mainWindow->setWindowState(Qt::WindowMaximized); // Startet das Fenster von anfang an in Vollbild
    mainWindow->show();
    return a.exec();
}
