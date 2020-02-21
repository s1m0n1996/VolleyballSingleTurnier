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
//#include "View/windowpopup.h"

class TournamentNamePopUp : public QWidget
{
    Q_OBJECT
public:
    explicit TournamentNamePopUp(QWidget *parent = nullptr);
    QString getName();
    WindowEdit* getDate(){return _dateEdit;}
    bool isTournament =  false;


public slots:
    void openTournamentWindow(void);

signals:
    void tournamentName();
private:
    WindowLabel* _dateLabel = nullptr;
    WindowEdit* _dateEdit = nullptr;

    WindowLabel* _nameLabel = nullptr;
    WindowEdit* _nameEdit = nullptr;

    WindowButton* _createTournament = nullptr;
};

#endif // TOURNEMENTNAMEPOPUP_H
