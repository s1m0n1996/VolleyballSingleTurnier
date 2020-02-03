#include "viewerwindow.h"
#include "ui_viewerWindow.h"
#include "ui_viewerWindow.h"

ViewerWindow::ViewerWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::ViewerWindow)
{
    ui->setupUi(this);
}
