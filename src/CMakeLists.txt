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

include_directories(../doc)
include_directories(../)

# set .h files
set(MOC_HEADERS
        Model/model.h
        Model/player.h
        Model/sqliteConnector.h
        Model/tournament.h
        View/dartboard.h
        View/gameboard.h
        View/mainmenu.h
        View/playermanagment.h
        View/refereewindow.h
        View/tournementwindow.h
        View/viewerwindow.h
        main.h
        )

# set .cpp files
set(SOURCES
        Model/model.cpp
        Model/player.cpp
        Model/sqliteConnector.cpp
        Model/tournament.cpp
        View/dartboard.cpp
        View/gameboard.cpp
        View/mainmenu.cpp
        View/playermanagment.cpp
        View/refereewindow.cpp
        View/tournementwindow.cpp
        View/viewerwindow.cpp
        main.cpp
        )

# set .ui files
set(UIS
        View/mainmenu.ui
        View/playermanagment.ui
        View/refereewindow.ui
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
