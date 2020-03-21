#ifndef SPORTVERANSTALTUNG_LOADTOURNAMENTPOPUP_H
#define SPORTVERANSTALTUNG_LOADTOURNAMENTPOPUP_H

#include <QWidget>
#include <QComboBox>

class GameManagement;
class WindowButton;
class WindowLabel;
class QComboBox;

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
    LoadTournamentPopup(void);
    ~LoadTournamentPopup();

private slots:

    void _loadTournamentForGame(void);

private:
    void _loadAllTournamentsFromDatabase(void);
    void _createWidgets(void);
    void _setAllLayout(void);
    void _connect(void);

    GameManagement* _gameManagement;
    QComboBox* _comboBox = nullptr;
    WindowLabel* _informationLabel = nullptr;
    WindowButton* _loadButton;


};

#endif //SPORTVERANSTALTUNG_LOADTOURNAMENTPOPUP_H
