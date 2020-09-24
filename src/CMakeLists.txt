# https://www.jetbrains.com/help/clion/qt-tutorial.html

cmake_minimum_required(VERSION 3.15)
project(Sportveranstaltung)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.14.2\\mingw73_64")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Tell cmake to find the modules Qt5Core and Qt5widgets
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Sql REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Charts REQUIRED)

include_directories(../doc)
include_directories(../)

# set .h files
set(MOC_HEADERS
        Model/game.h
        Model/gameboard.h
        Model/gameManagement.h
        Model/player.h
        Model/playerStatistics.h
        Model/referee.h
        Model/sqliteConnector.h
        Model/playerManagement.h
        Model/statistics.h
        Model/viewer.h
        View/createTournamentPopup.h
        View/dartboard.h
        View/dartboardViewer.h
        View/drilldownChart.h
        View/drilldownSlice.h
        View/mainmenu.h
        View/playerManagementWindow.h
        View/refereePopupBustLeg.h
        View/refereePopupWinningLeg.h
        View/refereeWindow.h
        View/statisticWindow.h
        View/tabelView.h
        View/tournamentWindow.h
        View/viewerWindow.h
        View/windowButton.h
        View/windowEdit.h
        View/windowLabel.h
        View/loadTournamentPopup.h
        View/winnerpopup.h
        Model/create_volleyball_game_board.h
        View/volleyball_game_plane.h
        )

# set .cpp files
set(SOURCES
        Model/game.cpp
        Model/gameboard.cpp
        Model/gameManagement.cpp
        Model/player.cpp
        Model/playerStatistics.cpp
        Model/referee.cpp
        Model/sqliteConnector.cpp
        Model/playerManagement.cpp
        Model/statistics.cpp
        Model/viewer.cpp
        View/createTournamentPopup.cpp
        View/dartboard.cpp
        View/dartboardViewer.cpp
        View/drilldownChart.cpp
        View/drilldownSlice.cpp
        View/mainmenu.cpp
        View/playerManagementWindow.cpp
        View/refereePopupBustLeg.cpp
        View/refereePopupWinningLeg.cpp
        View/refereeWindow.cpp
        View/statisticWindow.cpp
        View/tabelView.cpp
        View/tournamentWindow.cpp
        View/viewerWindow.cpp
        View/windowButton.cpp
        View/windowEdit.cpp
        View/windowLabel.cpp
        View/loadTournamentPopup.cpp
        View/winnerpopup.cpp
        Model/create_volleyball_game_board.cpp
        View/volleyball_game_plane.cpp
        main.cpp
        )

# set .ui files
set(UIS
        View/refereeWindow.ui
        View/viewerWindow.ui
        View/volleyball_game_plane.ui
        )

# set ressources
set(RESOURCES
        ../rsrc/resources.qrc
        ../rsrc/resources.rc
        )


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("debug mode")
    add_executable(Sportveranstaltung
            ${RESOURCES}
            ${SOURCES}
            ${MOC_SRCS}
            ${UI_HEADERS}
            ${LIBARY}
            )
else ()
    message("release mode")
    add_executable(Sportveranstaltung WIN32
            ${RESOURCES}
            ${SOURCES}
            ${MOC_SRCS}
            ${UI_HEADERS}
            ${LIBARY}
            )
ENDIF()


target_link_libraries(Sportveranstaltung Qt5::Widgets)
target_link_libraries(Sportveranstaltung Qt5::Core)
target_link_libraries(Sportveranstaltung Qt5::Sql)
target_link_libraries(Sportveranstaltung Qt5::Gui)
target_link_libraries(Sportveranstaltung Qt5::Charts)
