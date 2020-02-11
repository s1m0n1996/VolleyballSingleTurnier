#include "tournementnamepopup.h"

TournementNamePopUp::TournementNamePopUp(QWidget *parent) : QWidget (parent)
{

        _dateLabel = new WindowLabel("Turnierdatum");
        _dateEdit = new WindowEdit("1990-01-30");

        _nameLabel = new WindowLabel("Turniername");
        _nameEdit = new WindowEdit("Max Mustermann");

        _createTournement = new WindowButton("Erstelle Spielplan");


        QGridLayout* layout = new QGridLayout;

        layout -> addWidget(_dateLabel,0,0);
        layout -> addWidget(_dateEdit,0,1);
        layout -> addWidget(_nameLabel,1,0);
        layout -> addWidget(_nameEdit,1,1);
        layout -> addWidget(_createTournement,2,2);
        setLayout(layout);

        connect(_createTournement, SIGNAL(released()), this, SLOT(openTournementWindow()));

}


void TournementNamePopUp::openTournementWindow(void)
{

    TournementWindow* tournementWindow2 = new TournementWindow;
    tournementWindow2->show();
}
