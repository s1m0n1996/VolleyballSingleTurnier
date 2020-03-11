#ifndef PLAYERMANAGMENT_H
#define PLAYERMANAGMENT_H

#include <QMainWindow>


class PlayerManagement;
class QAction;
class QGroupBox;
class QMenu;
class QLabel;
class TableView;
class TournamentWindow;
class ViewerWindow;
class WindowButton;
class WindowEdit;
class WindowLabel;



/*!
 * \file playermanagmentWindow.h
 * \brief Die Klasse playermanagmentWindow erzeugt die Meldestelle, in der neue Spieler einer gesamt Datenbank,
 * als auch einem neuen Spiel hinzu gefügt oder gelöscht werden
 * \author Lea Kesselmeier
 *
 * Zwei TabelViews zeigt die gesamt Spielerdatenbank als auch die aktuelle Spielerdatenbank für ein Turnier
 * Darunter wird die benötigte Spieleranzahl für ein Turnier angezeigt
 * DArunter kann man neue Spieler hinzufügen (Name, Geburtsdatum, Land)
 * Ein Turnier beginnt man wenn die Spieleranzahl entsprechend ist
*/

class PlayermanagementWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayermanagementWindow(PlayerManagement* playerManagementModel, QWidget *parent = nullptr);
    ~PlayermanagementWindow();


signals:
public slots:    
    void setMissingPlayersForNewTournamentLabel(void);
    void startTournament(void);

    void addPlayerForNewGame(void);
    void dropPlayerForNewGame(void);
    void createDeleteMenu(void);
    void createRestoreMenu(void);
    void deletePlayer(void);
    void restorePlayer(void);

    void addPhotoWithSelection(void);
    void addPhotoWithButton(void);
    void addPlayerToDatabase(void);
    void enableAddPlayerButton(void);

    void showDeletedPlayers(void);

private:
    void createWidges(void);
    void showTable(void);
    void setAllLayout(void);
    void connecting(void);


    PlayerManagement* _playerManagementModel = nullptr; 

    QLabel*         _colorLabel = nullptr;
    WindowLabel*    _title      = nullptr;

    WindowLabel* _allPlayerLabel    = nullptr;
    WindowLabel* _gamePlayerLabel   = nullptr;

    TableView* _allPlayerTableView          = nullptr;
    TableView* _gamePlayerTableView         = nullptr;
    TableView* _deletedPlayersTableView     = nullptr;

    QMenu*        _menuDelete                    = nullptr;
    QAction*      _deletePlayerInAllTournament   = nullptr;
    QAction*      _addPhotoAction                = nullptr;
    WindowButton* _addPlayerForNewTournament     = nullptr;
    WindowButton* _deletePlayerForNewTournament  = nullptr;

    WindowLabel* _nameMissingPlayersLabel        = nullptr;
    WindowLabel* _valueMissingPlayersLabel       = nullptr;

    QByteArray* _byteArray                       = nullptr;

    QGroupBox* _addPlayer                       = nullptr;

    WindowLabel* _playernameLabel           = nullptr;
    WindowLabel* _birthdayLabel             = nullptr;
    WindowLabel* _countryLabel              = nullptr;
    WindowLabel* _photoLabel                     = nullptr;

    WindowLabel* _photo = nullptr;


    WindowEdit*     _playernameEdit             = nullptr;
    WindowEdit*     _birthdayEdit               = nullptr;
    WindowEdit*     _countryEdit                = nullptr;
    WindowButton*   _addPhoto                   = nullptr;

    WindowButton* _addPlayerButton          = nullptr;
    WindowButton* _startTournamentButton    = nullptr;

    QAction* _showDeletedPlayersAction;

};
#endif // PLAYERMANAGMENT_H
