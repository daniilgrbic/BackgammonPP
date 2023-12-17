QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    barcheckerholder.cpp \
    boardbar.cpp \
    boardchecker.cpp \
    boardscene.cpp \
    boardtriangle.cpp \
    boardwindow.cpp \
    checkerholder.cpp \
    main.cpp \
    mainwindow.cpp \
    engine/backgammon.cpp \
    engine/core/boardstate.cpp \
    engine/core/die.cpp \
    engine/core/game.cpp \
    engine/core/point.cpp \
    engine/core/roll.cpp \
    network/chat_message.cpp \
    network/server.cpp \
    network/client.cpp

HEADERS += \
    mainwindow.h \
    engine/backgammon.h \
    engine/core/boardstate.h \
    engine/core/die.h \
    engine/core/game.h \
    engine/core/gameresult.h \
    engine/core/move.h \
    engine/core/playercolor.h \
    engine/core/point.h \
    engine/core/roll.h \
    engine/core/specialposition.h \
    engine/core/turn.h \
    network/server_consts.h \
    utility/jsonserializer.h \
    utility/serializable.h \
    network/chat_message.h \
    network/server.h \
    network/client.h \
    barcheckerholder.h \
    boardbar.h \
    boardchecker.h \
    boardscene.h \
    boardtriangle.h \
    boardwindow.h \
    checkerholder.h \
    mainwindow.h

FORMS += \
    boardwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
