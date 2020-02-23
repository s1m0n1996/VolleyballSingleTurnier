#ifndef TOURNEMENTWINDOW_H
#define TOURNEMENTWINDOW_H

#include <QWidget>
class WindowLabel;
class WindowButton;
class QLabel;

class TournamentWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TournamentWindow(QWidget *parent = nullptr);

signals:

private:
    void createWidges(void);
    QLabel* _color = nullptr;
    WindowLabel* _title= nullptr;
    WindowButton* _startGame= nullptr;


};

#endif // TOURNEMENTWINDOW_H
