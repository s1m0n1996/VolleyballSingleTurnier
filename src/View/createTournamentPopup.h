#ifndef TOURNEMENTNAMEPOPUP_H
#define TOURNEMENTNAMEPOPUP_H

#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <View/tournamentWindow.h>
#include "View/windowButton.h"
#include "View/windowEdit.h"
#include "View/windowLabel.h"
#include <QDebug>
#include "Model/gameManagement.h"

class CreateTournamentPopUp : public QWidget
{
    Q_OBJECT
public:
    explicit CreateTournamentPopUp(QWidget *parent = nullptr);

public slots:
    void openTournamentWindow(void);
    void enableCreateTournamentButton(void);

private:
    WindowLabel* _dateLabel = nullptr;
    WindowEdit* _dateEdit = nullptr;

    WindowLabel* _nameLabel = nullptr;
    WindowEdit* _nameEdit = nullptr;

    WindowButton* _createTournament = nullptr;

    GameManagement* _gameManagement;
};

#endif // TOURNEMENTNAMEPOPUP_H
