# https://www.jetbrains.com/help/clion/qt-tutorial.html

cmake_minimum_required(VERSION 3.15)
project(Sportveranstaltung)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.12.6\\mingw73_64")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Tell cmake to find the modules Qt5Core and Qt5widgets
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Sql REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)

include_directories(../doc)
include_directories(../)

# set .h files
set(MOC_HEADERS
        Model/game.h
        Model/gameboard.h
        Model/model.h
        Model/player.h
        Model/referee.h
        Model/sqliteConnector.h
        Model/playermanagement.h
        Model/viewer.h
        View/dartboard.h
        View/gameboardWindow.h
        View/mainmenu.h
        View/playermanagementWindow.h
        View/refereepopupbustleg.h
        View/refereepopupwinningleg.h
        View/refereewindow.h
        View/tabelview.h
        View/tournamentnamepopup.h
        View/tournamentwindow.h
        View/viewerwindow.h
        View/windowbutton.h
        View/windowedit.h
        View/windowlabel.h
        Model/gameManagement.h
        View/loadTournamentPopup.h
        )

# set .cpp files
set(SOURCES
        Model/game.cpp
        Model/gameboard.cpp
        Model/model.cpp
        Model/player.cpp
        Model/referee.cpp
        Model/sqliteConnector.cpp
        Model/playermanagement.cpp
        Model/viewer.cpp
        View/dartboard.cpp
        View/gameboardWindow.cpp
        View/mainmenu.cpp
        View/playermanagementWindow.cpp
        View/refereepopupbustleg.cpp
        View/refereepopupwinningleg.cpp
        View/refereewindow.cpp
        View/tabelview.cpp
        View/tournamentnamepopup.cpp
        View/tournamentwindow.cpp
        View/viewerwindow.cpp
        View/windowbutton.cpp
        View/windowedit.cpp
        View/windowlabel.cpp
        Model/gameManagement.cpp
        View/loadTournamentPopup.cpp
        main.cpp
        )

# set .ui files
set(UIS
        View/refereewindow.ui
        View/viewerWindow.ui
        )

# set ressources
set(RESOURCES
        ../rsrc/resources.qrc
        )


add_executable(Sportveranstaltung
        ${RESOURCES}
        ${SOURCES}
        ${MOC_SRCS}
        ${UI_HEADERS}
        )


target_link_libraries(Sportveranstaltung Qt5::Widgets)
target_link_libraries(Sportveranstaltung Qt5::Core)
target_link_libraries(Sportveranstaltung Qt5::Sql)
target_link_libraries(Sportveranstaltung Qt5::Gui)
