QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    barcheckerholder.cpp \
    boardbar.cpp \
    boardchecker.cpp \
    boardscene.cpp \
    boardsidebar.cpp \
    boardtriangle.cpp \
    boardwindow.cpp \
    checkerholder.cpp \
    main.cpp \
    mainwindow.cpp \
    sidebarcheckerholder.cpp \
    verticalchecker.cpp

HEADERS += \
    barcheckerholder.h \
    boardbar.h \
    boardchecker.h \
    boardscene.h \
    boardsidebar.h \
    boardtriangle.h \
    boardwindow.h \
    checkerholder.h \
    mainwindow.h \
    sidebarcheckerholder.h \
    verticalchecker.h

FORMS += \
    boardwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    background_menu_image.qrc
