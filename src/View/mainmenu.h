#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>

class MenuButton;
class Model;
class PlayermanagementWindow;
class PlayerManagement;
class QAction;
class QMenu;
class Referee;
class RefereepopupWinningLeg;
class RefereeWindow;
class SqliteConnector;
class TournamentNamePopUp;
class TournamentWindow;
class ViewerWindow;
class WindowLabel;

/*!
 * \file mainmenu.h
 * \brief Die Klasse MainMenu erzeugt ein Hauptmenu aus dem die verschieden Fenster geöffnet werden können
 * \author Lea Kesselmeier
 *
 * In der Menüleiste lassen sich Spielerdatenbanken und Turniere erstellen oder starten
 * Eine Warnung erscheint, wenn keine Datenbank geöfnnet ist und welches Turnier gerade laden ist
 * Über die verschiedenen Buttons gelangt man zu der Meldestelle, dem Spielplan, der Zuschaueransicht und dem Richterfenster
*/

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(Model* model, QMainWindow* parent = nullptr);
    ~MainMenu();

    WindowLabel* getNote(){return _noteDatabase;}

public slots:

private slots:
    void openPlayermanagmentWindow();
    void openTournamentWindow();
    void openViewerWindow();
    void openRefereeWindow();
    void createDatabase();
    void loadDatabase();
    void tournamentName();
    void setTouenamentName();

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
    TournamentNamePopUp*    _tournamentName = nullptr;

    MenuButton* _playermanagment    = nullptr;
    MenuButton* _tournament         = nullptr;
    MenuButton* _viewer             = nullptr;
    MenuButton* _referee            = nullptr;

    PlayerManagement*       _playerManagementModel;
    Referee*                _refereeModel;
    RefereepopupWinningLeg* _refereePopupModel;



};
#endif // MAINMENU_H
