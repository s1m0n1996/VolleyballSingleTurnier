/*!
* \file mainwindow.h
* \brief Mainwindow
* \author Nico Meyer
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class Model;
class QLabel;
class QLineEdit;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(Model* model, QWidget* parent = nullptr);
    ~MainWindow();
public slots:
    void update(void);
    void openWindow(void);
private slots:
    void accept(void);
    void newWindow(void);
private:
    Model* _model = nullptr;
    QLineEdit* _input = nullptr;
    QLabel* _output = nullptr;
};
#endif // MAINWINDOW_H

