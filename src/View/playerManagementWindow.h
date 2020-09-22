#ifndef PLAYERMANAGMENT_H
#define PLAYERMANAGMENT_H

#include <QMainWindow>
class PlayerManagement;
class QAction;
class QDateEdit;
class QCalendarWidget;
class QGroupBox;
class QMenu;
class QLabel;
class TableView;
class ViewerWindow;
class WindowButton;
class WindowEdit;
class WindowLabel;


/*!
 * \file playermanagmentWindow.h
 * \brief Die Klasse playermanagmentWindow erzeugt die Meldestelle, in der neue Spieler einer Gesamtdatenbank,
 * als auch einem neuen Spiel hinzugefügt oder gelöscht werden
 * \author Lea Kesselmeier
 *
 * Zwei TabelViews zeigen die Gesamtspielerdatenbank als auch die aktuelle Spielerdatenbank für ein Turnier.
 * Darunter wird die benötigte Spieleranzahl für ein Turnier angezeigt.
 * Unter kann man neue Spieler hinzufügen (Name, Geburtsdatum, Land, Photo).
 * Ein Turnier beginnt man wenn die Spieleranzahl entsprechend ist.
*/
class PlayermanagementWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayermanagementWindow(PlayerManagement* playerManagementModel, QWidget *parent = nullptr);
    ~PlayermanagementWindow(void);

signals:
private slots:
    void setMissingPlayersForNewTournamentLabel(void);

    void addPlayerToDatabase(void);
    void addPhotoWithButton(void);

    void addPlayerForNewGame(void);
    void dropPlayerForNewGame(void);

    void deletePlayer(void);
    void restorePlayer(void);

    void addPhotoWithSelection(void);

    void startTournament(void);
    void enableAddPlayerButton(void);
    void showDeletedPlayers(void);

    void createDeleteMenu(void);
    void createRestoreMenu(void);

private:
    void createWidgets(void);
    void showTable(void);
    void setAllLayout(void);
    void connecting(void);

    PlayerManagement* _playerManagementModel = nullptr;

    QByteArray* _playerPicture               = nullptr;

    WindowLabel*    _title      = nullptr;

    WindowLabel* _allPlayerLabel    = nullptr;
    WindowLabel* _gamePlayerLabel   = nullptr;

    TableView* _allPlayerTableView          = nullptr;
    TableView* _gamePlayerTableView         = nullptr;
    TableView* _deletedPlayersTableView     = nullptr;

    QMenu*        _menuDelete                    = nullptr;
    QAction*      _showDeletedPlayersAction      = nullptr;
    QAction*      _deletePlayerInAllTournament   = nullptr;
    QAction*      _addPhotoAction                = nullptr;
    WindowButton* _addPlayerForNewTournament     = nullptr;
    WindowButton* _deletePlayerForNewTournament  = nullptr;

    WindowLabel* _nameMissingPlayersLabel        = nullptr;
    WindowLabel* _valueMissingPlayersLabel       = nullptr;

    QGroupBox* _addPlayer                       = nullptr;

    WindowLabel* _playernameLabel           = nullptr;
    WindowLabel* _birthdayLabel             = nullptr;
    WindowLabel* _countryLabel              = nullptr;
    WindowLabel* _photoLabel                = nullptr;
    WindowLabel* _genderLabel                = nullptr;


    WindowEdit*     _playernameEdit             = nullptr;
    QCalendarWidget* _calendar                  = nullptr;
    QDateEdit*      _birthday                   = nullptr;
    WindowEdit*     _countryEdit                = nullptr;
    WindowEdit*     _genderEdit                = nullptr;
    WindowButton*   _addPhoto                   = nullptr;


    WindowButton* _addPlayerButton          = nullptr;
    WindowButton* _startTournamentButton    = nullptr;    

};
#endif // PLAYERMANAGMENT_H
