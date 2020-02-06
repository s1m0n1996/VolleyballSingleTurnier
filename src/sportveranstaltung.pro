QT       += core gui sql

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
    Model/model.cpp \
    Model/player.cpp \
    Model/referee.cpp \
    Model/sqliteConnector.cpp \
    Model/playermanagement.cpp \
    View/dartboard.cpp \
    View/gameboardWindow.cpp \
    View/mainmenu.cpp \
    View/menubutton.cpp \
    View/playermanagementWindow.cpp \
    View/refereewindow.cpp \
    View/tournementwindow.cpp \
    View/viewerwindow.cpp \
    View/windowbutton.cpp \
    View/windowedit.cpp \
    View/windowlabel.cpp \
    main.cpp

HEADERS += \
    Model/game.h \
    Model/gameboard.h \
    Model/model.h \
    Model/player.h \
    Model/referee.h \
    Model/sqliteConnector.h \
    Model/playermanagement.h \
    View/dartboard.h \
    View/gameboardWindow.h \
    View/mainmenu.h \
    View/menubutton.h \
    View/playermanagementWindow.h \
    View/refereewindow.h \
    View/tournementwindow.h \
    View/viewerwindow.h \
    View/windowbutton.h \
    View/windowedit.h \
    View/windowlabel.h

FORMS += \
    View/mainmenu.ui \
    View/playermanagementWindow.ui \
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
