QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    *.cpp \
    engine/backgammon.cpp \
    engine/core/boardstate.cpp \
    engine/core/die.cpp \
    engine/core/game.cpp \
    engine/core/point.cpp \
    engine/core/roll.cpp

HEADERS += \
    *.h \
    engine/backgammon.h \
    engine/core/*.h \
    utility/jsonserializer.h \
    utility/serializable.h

FORMS += *.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
