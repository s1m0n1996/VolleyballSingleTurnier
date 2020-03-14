#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>

class Model;
class PlayermanagementWindow;
class PlayerManagement;
class QAction;
class QMenu;
class Referee;
class RefereepopupWinningLeg;
class RefereeWindow;
class SqliteConnector;
class CreateTournamentPopUp;
class TournamentWindow;
class ViewerWindow;
class WindowButton;
class WindowLabel;
class GameManagement;
class LoadTournamentPopup;


/*!
 * \file mainmenu.h
 * \brief Die Klasse MainMenu erzeugt ein Hauptmenu aus dem die verschieden Fenster geöffnet werden können
 * \author Lea Kesselmeier
 *
 * In der Menüleiste lassen sich Spielerdatenbanken und Turniere erstellen oder starten
 * Eine Warnung erscheint, wenn keine Datenbank geöfnnet ist und welches Turnier gerade laden ist
 * Über die verschiedenen Buttons gelangt man zu der Meldestelle, dem Spielplan, der Zuschaueransicht und dem Richterfenster
*/

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QMainWindow* parent = nullptr);
    ~MainMenu();

    WindowLabel* getNote(){return _noteDatabase;}

public slots:

private slots:
    void openPlayermanagmentWindow();
    void openTournamentWindow();
    void openViewerWindow();
    void openRefereeWindow();
    void openStatisticWindow();
    void createDatabase();
    void loadDatabase();
    void createTournament();
    void loadTournament();
    void setTournamentName();
    void setButtonEnableState();

private:
    void connecting();
    void createWidgets();
    void setwholeLayout();

    Model*      _model = nullptr;

    QMenu*      _playerData    = nullptr;
    QAction*    _loadPlayer    = nullptr;
    QAction*    _newPlayer     = nullptr;

    QMenu*      _tournamentData    = nullptr;
    QAction*    _loadTournament    = nullptr;
    QAction*    _newTournament     = nullptr;

    WindowLabel* _title = nullptr;

    WindowLabel*            _noteDatabase   = nullptr;
    WindowLabel*            _noteTournament = nullptr;
    CreateTournamentPopUp*    _tournamentName = nullptr;
    LoadTournamentPopup* _loadTournamentPopup;

    WindowButton* _playermanagment    = nullptr;
    WindowButton* _tournament         = nullptr;
    WindowButton* _viewer             = nullptr;
    WindowButton* _referee            = nullptr;
    WindowButton* _statitsic          = nullptr;

    PlayerManagement*       _playerManagementModel;
    Referee*                _refereeModel;
    RefereepopupWinningLeg* _refereePopupModel;

    GameManagement* _gameManagement;


};
#endif // MAINMENU_H
