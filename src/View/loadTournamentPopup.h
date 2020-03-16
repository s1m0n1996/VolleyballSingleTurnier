
#ifndef SPORTVERANSTALTUNG_LOADTOURNAMENTPOPUP_H
#define SPORTVERANSTALTUNG_LOADTOURNAMENTPOPUP_H

#include <QWidget>
#include <QComboBox>
#include <QWindow>
#include <QGridLayout>

#include "View/windowlabel.h"
#include "View/windowbutton.h"
#include "Model/gameManagement.h"

/*!
 * \file loadTournamentPopup.h
 * \brief Fenster um anderes Tournier zu Laden.
 * \author Simon Flörke
 *
 * In dieser Klasse öffnet sich ein Fenster, um ein anderes Tournier zu laden.
 * Um das Turnier zu laden kann man in einem DropDown Menü zwischen den verfügbaren Turnieren eines auswählen.
 * Wird das Turnier geladen, so holt sich das gesamte Programm immer nur noch die Daten von dem
 * jetzt ausgewählten Turnier
*/
class LoadTournamentPopup : public QWidget
{
Q_OBJECT

public:
    LoadTournamentPopup();
    ~LoadTournamentPopup();

private slots:

    void _loadTournamentForGame(void);

private:

    GameManagement* _gameManagement;
    QComboBox* _comboBox;
    WindowLabel* _firstLabel;
    WindowButton* _loadButton;

    void _createWidgets(void);

    void _setLayout(void);

    void _connect(void);

    void _loadAllTournamentsFromDatabase(void);
};

#endif //SPORTVERANSTALTUNG_LOADTOURNAMENTPOPUP_H
