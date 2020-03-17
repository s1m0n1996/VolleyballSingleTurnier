QT       += core gui sql
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Model/game.cpp \
    Model/gameboard.cpp \
Model/gameManagement.cpp \
    Model/player.cpp \
    Model/playerStatistics.cpp \
    Model/referee.cpp \
    Model/sqliteConnector.cpp \
    Model/playerManagement.cpp \
    Model/statistics.cpp \
    Model/viewer.cpp \
    View/createTournamentPopup.cpp \
    View/dartboard.cpp \ 
    View/dartboardViewer.cpp \
    View/drilldownChart.cpp \
    View/drilldownSlice.cpp \
    View/mainmenu.cpp \
    View/playerManagementWindow.cpp \
    View/refereePopupBustLeg.cpp \
    View/refereePopupWinningLeg.cpp \
    View/refereeWindow.cpp \
    View/statisticWindow.cpp \
    View/tabelView.cpp \
    View/tournamentWindow.cpp \
    View/viewerWindow.cpp \
    View/windowButton.cpp \
    View/windowEdit.cpp \
    View/windowLabel.cpp \
    View/loadTournamentPopup.cpp \
    main.cpp

HEADERS += \
    Model/game.h \
    Model/gameboard.h \
    Model/gameManagement.h \
    Model/player.h \ 
    Model/playerStatistics.h \
    Model/referee.h \
    Model/sqliteConnector.h \   
    Model/playerManagement.h \
    Model/statistics.h \
    Model/viewer.h \
    View/createTournamentPopup.h \
    View/dartboard.h \ 
    View/dartboardViewer.h \
    View/drilldownChart.h \
    View/drilldownSlice.h \
    View/mainmenu.h \
    View/playerManagementWindow.h \
    View/refereePopupBustLeg.h \
    View/refereePopupWinningLeg.h \
    View/refereeWindow.h \
    View/statisticWindow.h \
    View/tabelView.h \
    View/tournamentWindow.h \
    View/viewerWindow.h \
    View/windowButton.h \
    View/windowEdit.h \
    View/windowLabel.h \
    View/loadTournamentPopup.h

FORMS += \
    View/refereewindow.ui \
    View/viewerWindow.ui

TRANSLATIONS += \
    spotveranstaltung_de_DE.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../rsrc/resources.qrc
