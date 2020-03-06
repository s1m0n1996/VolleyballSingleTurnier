#ifndef TOURNEMENTNAMEPOPUP_H
#define TOURNEMENTNAMEPOPUP_H

#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <View/tournamentwindow.h>
#include "View/windowbutton.h"
#include "View/windowedit.h"
#include "View/windowlabel.h"
#include <QDebug>
#include "Model/gameManagement.h"

class CreateTournamentPopUp : public QWidget
{
    Q_OBJECT
public:
    explicit CreateTournamentPopUp(QWidget *parent = nullptr);

public slots:
    void openTournamentWindow(void);

private:
    WindowLabel* _dateLabel = nullptr;
    WindowEdit* _dateEdit = nullptr;

    WindowLabel* _nameLabel = nullptr;
    WindowEdit* _nameEdit = nullptr;

    WindowButton* _createTournament = nullptr;

    GameManagement* _gameManagement;
};

#endif // TOURNEMENTNAMEPOPUP_H
