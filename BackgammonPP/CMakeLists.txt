cmake_minimum_required(VERSION 3.16)
project(
  BackgammonPP
  VERSION 1.0
  LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network Multimedia)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

add_subdirectory(tests)

qt_add_executable(BackgammonPP WIN32 MACOSX_BUNDLE
    engine/core/boardstate.h engine/core/boardstate.cpp
    engine/core/die.h engine/core/die.cpp
    engine/core/game.h engine/core/game.cpp
    engine/core/gameresult.h
    engine/core/move.h engine/core/move.cpp
    engine/core/playercolor.h
    engine/core/point.h engine/core/point.cpp
    engine/core/roll.h engine/core/roll.cpp
    engine/core/specialposition.h
    engine/core/turn.h engine/core/turn.cpp
    engine/core/turntrie.h engine/core/turntrie.cpp
    engine/backgammon.h engine/backgammon.cpp
    engine/longnardy.h engine/longnardy.cpp
    network/server.cpp
    network/server.h
    network/client.cpp
    network/client.h
    network/server_commands.h
    network/chat_message.cpp
    network/chat_message.h
    utility/jsonserializer.h
    utility/serializable.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    background_menu_image.qrc
    barcheckerholder.cpp
    barcheckerholder.h
    boardbar.cpp
    boardbar.h
    boardchecker.cpp
    boardchecker.h
    boarddie.cpp
    boarddie.h
    boarddoublingdie.cpp
    boarddoublingdie.h
    boardplayingdie.cpp
    boardplayingdie.h
    boardscene.cpp
    boardscene.h
    boardsidebar.cpp
    boardsidebar.h
    boardtriangle.cpp
    boardtriangle.h
    boardwidget.cpp
    boardwidget.h
    boardwindow.cpp
    boardwindow.h
    boardwindow.ui
    checkerholder.cpp
    checkerholder.h
    CMakeLists.txt
    consts.h
    controller.cpp
    controller.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    preferences.ui
    sidebarcheckerholder.cpp
    sidebarcheckerholder.h
    verticalchecker.cpp
    verticalchecker.h
    player.h player.cpp
    botplayer.h botplayer.cpp
    localplayer.h localplayer.cpp
    match.h match.cpp
    consts.h
    stringlistmodel.h stringlistmodel.cpp
    historylistmodel.h historylistmodel.cpp
    stringlistmodel.h stringlistmodel.cpp
    remoteplayer.h remoteplayer.cpp
    music.qrc
)
target_link_libraries(BackgammonPP PRIVATE Qt::Core Qt::Gui Qt::Network Qt::Multimedia)

if((QT_VERSION_MAJOR GREATER 4))
  target_link_libraries(BackgammonPP PRIVATE Qt::Widgets)
endif()

install(
  TARGETS BackgammonPP
  BUNDLE DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
