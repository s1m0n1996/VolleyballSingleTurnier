#ifndef TOURNEMENTNAMEPOPUP_H
#define TOURNEMENTNAMEPOPUP_H

#include <QWidget>
#include <QMainWindow>

class GameManagement;
class QDateEdit;
class WindowButton;
class WindowEdit;
class WindowLabel;

class CreateTournamentPopUp : public QWidget
{
    Q_OBJECT
public:
    explicit CreateTournamentPopUp(QWidget *parent = nullptr);

public slots:
    void openTournamentWindow(void);
    void enableCreateTournamentButton(void);

private:
    void createWidgets(void);
    void setAllLayout(void);
    void connecting(void);

    GameManagement* _gameManagement;

    WindowLabel* _dateLabel = nullptr;
    QDateEdit* _date        = nullptr;;
    WindowEdit* _dateEdit = nullptr;

    WindowLabel* _nameLabel = nullptr;
    WindowEdit* _nameEdit = nullptr;

    WindowButton* _createTournament = nullptr;
};

#endif // TOURNEMENTNAMEPOPUP_H
