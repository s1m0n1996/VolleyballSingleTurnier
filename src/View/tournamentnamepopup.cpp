#include "tournamentnamepopup.h"
#include "View/mainmenu.h"

TournamentNamePopUp::TournamentNamePopUp(QWidget *parent) : QWidget (parent)
{

        _dateLabel = new WindowLabel("Turnierdatum");
        _dateEdit = new WindowEdit("1990-01-30");

        _nameLabel = new WindowLabel("Turniername");
        _nameEdit = new WindowEdit("Max Mustermann");

        _createTournament = new WindowButton("Erstelle Spielplan");


        QGridLayout* layout = new QGridLayout;

        layout -> addWidget(_dateLabel,0,0);
        layout -> addWidget(_dateEdit,0,1);
        layout -> addWidget(_nameLabel,1,0);
        layout -> addWidget(_nameEdit,1,1);
        layout -> addWidget(_createTournament,2,2);
        setLayout(layout);

        connect(_createTournament, SIGNAL(released()), this, SLOT(openTournamentWindow()));



        setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);

}


void TournamentNamePopUp::openTournamentWindow(void)
{
     setWindowFlags(Qt::Window);
     isTournament = true;
     qDebug()<<getName();
     emit tournamentName();
}

QString TournamentNamePopUp::getName()
{
    return _nameEdit -> text();

}
