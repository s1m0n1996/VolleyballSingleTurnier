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
 * \brief Die Klasse MainMenu erzeugt ein Hauptmenü, aus dem die verschieden Fenster geöffnet werden können
 * \author Lea Kesselmeier
 *
 * In der Menüleiste lassen sich Spielerdatenbanken und Turniere erstellen oder starten
 * Eine Warnung erscheint, wenn keine Datenbank geöffnet ist und welches Turnier gerade laden ist
 * Über die verschiedenen Buttons gelangt man zu der Meldestelle, dem Spielplan, der Zuschaueransicht,
 * dem Richteransicht und dem Statistikfenster
*/

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QMainWindow* parent = nullptr);
    ~MainMenu(void);

    WindowLabel* getNote(void){return _noteDatabase;}


private slots:
    void openPlayermanagmentWindow(void);
    void openTournamentWindow(void);
    void openViewerWindow(void);
    void openRefereeWindow(void);
    void openStatisticWindow(void);

    void createDatabase(void);
    void loadDatabase(void);
    void refreshDatabase(void);

    void createTournament(void);
    void loadTournament(void);
    void setTournamentName(void);

    void setButtonEnableState(void);

private:
    void connecting(void);
    void createWidgets(void);
    void setAllLayout(void);
    void hideForVolleyball();

    Model*      _model = nullptr;

    SqliteConnector*        _sqliteConnector        = nullptr;
    PlayerManagement*       _playerManagementModel  = nullptr;
    Referee*                _refereeModel           = nullptr;
    RefereepopupWinningLeg* _refereePopupModel      = nullptr;
    GameManagement*         _gameManagement         = nullptr;

    QMenu*      _playerData    = nullptr;
    QAction*    _loadPlayer    = nullptr;
    QAction*    _newPlayer     = nullptr;

    QMenu*      _tournamentData    = nullptr;
    QAction*    _loadTournament    = nullptr;
    QAction*    _newTournament     = nullptr;

    WindowLabel* _title = nullptr;

    WindowLabel*            _noteDatabase               = nullptr;
    WindowLabel*            _noteTournament             = nullptr;
    CreateTournamentPopUp*  __createTournamentPopup     = nullptr;
    LoadTournamentPopup*    _loadTournamentPopup        = nullptr;

    WindowButton* _playermanagment    = nullptr;
    WindowButton* _tournament         = nullptr;
    WindowButton* _viewer             = nullptr;
    WindowButton* _referee            = nullptr;
    WindowButton* _statitsic          = nullptr;
    WindowButton* _volleyballGameBoard= nullptr;



};
#endif // MAINMENU_H
