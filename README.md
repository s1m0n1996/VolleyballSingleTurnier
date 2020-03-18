<pre>
  ____                   _                                 _        _ _                     
 / ___| _ __   ___  _ __| |___   _____ _ __ __ _ _ __  ___| |_ __ _| | |_ _   _ _ __   __ _ 
 \___ \| '_ \ / _ \| '__| __\ \ / / _ \ '__/ _` | '_ \/ __| __/ _` | | __| | | | '_ \ / _` |
  ___) | |_) | (_) | |  | |_ \ V /  __/ | | (_| | | | \__ \ || (_| | | |_| |_| | | | | (_| |
 |____/| .__/ \___/|_|   \__| \_/ \___|_|  \__,_|_| |_|___/\__\__,_|_|\__|\__,_|_| |_|\__, |
       |_|                                                                            |___/ 
</pre>       

# Aufgabenstellung

Dieses Programm verwaltet eine Sportveranstaltung (in diesem Fall wird ein Darts-Turnier verwaltet). 
Verwaltet werden die Einzelwettbewerbe mit Uhrzeiten, deren Teilnehmer und erzielte Ergebnisse. 
Es gibt eine Oberfläche für die Meldestelle, eine für die Richter sowie eine Anzeigetafel für die Zuschauer 
mit Ergebnissen.

# Version
Das Programm befindet sich in der Version: **v1.0**

# Benutzte Bibliotheken/Frameworks

* [QT](https://www.qt.io/) - Framework (erstellt mit Qt 5.12.6)


# Projektbeteiligte

* **Lea Kesselmeier**
* **Philipp Wietfeld**
* **Nico Meyer**
* **Simon Flörke**

# Projektzeitraum
Das Projekt wurde im 3. Semester (JG 2018) vom **06.01.2020 - 23.03.2020** erstellt.

# License

Das Projekt ist under der GPL version 2 License lizensiert - diehe [LICENSE.md](LICENSE.md) Datei für mehr Details.

# Informationen zur Versionskontrolle

Das Programm steht unter GIT Versionskontrolle

# Verzeichnisstruktur des Repositorys

<pre>
│   .gitignore
│   LICENSE.md
│   README.md
│               
├───doc
│   │   
│   ├───besprechungsprotokolle
│   │       2020_01_13 Lastenheft.txt
│   │       2020_01_20 Klassendiagramm und Roadmap.txt
│   │       2020_01_27 Fragenrunde.txt
│   │       2020_02_05 Fragenrunde.txt
│   │       2020_02_10 Fragenrunde.txt
│   │       2020_02_10 Fragenrunde.txt
│   │       2020_02_17 Fragenrunde.txt
│   │       2020_02_24 Fragenrunde.txt
│   │       2020_03_02 Fragenrunde.txt
│   │       2020_03_11 Fragenrunde.txt
│   │       
│   └───doxygen
│          Doxyfile
│
├───rsrc
│   │   resources.qrc
│   │   
│   ├───database
│   │       DDL_DatabaseInserts.sql
│   │       DDL_DatabaseSchema.sql
│   │       DDL_DatabaseSchemaMariaDB.sql
│   │       FinishCombination.json
│   │       
│   └───img
│           addPhoto.png
│           addPlayer.png
│           backgroundGamePlan.png
│           createDart.png
│           crowns.png
│           Dartboard.png
│           darts.png
│           gameplan.png
│           left.png
│           referee.png
│           right.png
│           statistic.png
│           user.png
│           viewer.png
│           
└───src
    │   CMakeLists.txt
    │   main.cpp
    │   sportveranstaltung.pro
    │               
    ├───Model
    │       game.cpp
    │       game.h
    │       gameboard.cpp
    │       gameboard.h
    │       gameManagement.cpp
    │       gameManagement.h
    │       player.cpp
    │       player.h
    │       playerManagement.cpp
    │       playerManagement.h
    │       playerStatistics.cpp
    │       playerStatistics.h
    │       referee.cpp
    │       referee.h
    │       sqliteConnector.cpp
    │       sqliteConnector.h
    │       statistics.cpp
    │       statistics.h
    │       viewer.cpp
    │       viewer.h
    │       
    └───View
            createTournamentPopup.cpp
            createTournamentPopup.h
            dartboard.cpp
            dartboard.h
            dartboardViewer.cpp
            dartboardViewer.h
            drilldownChart.cpp
            drilldownChart.h
            drilldownSlice.cpp
            drilldownSlice.h
            gameboardWindow.cpp
            gameboardWindow.h
            loadTournamentPopup.cpp
            loadTournamentPopup.h
            mainmenu.cpp
            mainmenu.h
            playerManagementWindow.cpp
            playerManagementWindow.h
            refereePopupBustLeg.cpp
            refereePopupBustLeg.h
            refereePopupWinningLeg.cpp
            refereePopupWinningLeg.h
            refereeWindow.cpp
            refereeWindow.h
            refereeWindow.ui
            statisticWindow.cpp
            statisticWindow.h
            tabelView.cpp
            tabelView.h
            tournamentWindow.cpp
            tournamentWindow.h
            viewerWindow.cpp
            viewerWindow.h
            viewerWindow.ui
            windowButton.cpp
            windowButton.h
            windowEdit.cpp
            windowEdit.h
            windowLabel.cpp
            windowLabel.h
</pre>

# Struktur des Codes
Das Programm wurde in Model und View unterteilt (Siehe Klassendiagramm)
