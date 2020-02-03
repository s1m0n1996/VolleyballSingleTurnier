#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QMainWindow>
namespace Ui {
    class ViewerWindow;
}
class ViewerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ViewerWindow(QWidget *parent = nullptr);

private:
     Ui::ViewerWindow *ui;

};

#endif // VIEWERWINDOW_H
