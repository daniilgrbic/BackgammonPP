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

add_executable(BackgammonPP WIN32 MACOSX_BUNDLE

    # Engine Core
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

    # Game Variants
    engine/backgammon.h engine/backgammon.cpp
    engine/longnardy.h engine/longnardy.cpp

    # Game Bot
    engine/bot/bot.hpp engine/bot/bot.cpp
    engine/bot/connectgene.hpp engine/bot/connectgene.cpp
    engine/bot/genome.hpp engine/bot/genome.cpp
    engine/bot/innovation.cpp engine/bot/innovation.hpp
    engine/bot/link.hpp
    engine/bot/neat.hpp engine/bot/neat.cpp
    engine/bot/network.hpp engine/bot/network.cpp
    engine/bot/neuron.hpp engine/bot/species.hpp
    engine/bot/nodegene.hpp
    engine/bot/saved_genomes/lilith.genome
    engine/bot/saved_genomes/lilith_copy.genome

    # Utility Functions / Classes
    utility/jsonserializer.h
    utility/serializable.h
    utility/threadsafequeue.h

    # Networking
    network/chat_message.h network/chat_message.cpp
    network/client.h network/client.cpp
    network/server.h network/server.cpp
    network/server_commands.h

    # GUI
    barcheckerholder.h barcheckerholder.cpp
    boardbar.h boardbar.cpp
    boardchecker.h boardchecker.cpp
    boarddie.h boarddie.cpp
    boarddoublingdie.h boarddoublingdie.cpp
    boardplayingdie.h boardplayingdie.cpp
    boardscene.h boardscene.cpp
    boardsidebar.h boardsidebar.cpp
    boardtriangle.h boardtriangle.cpp
    boardwidget.h boardwidget.cpp
    boardwindow.h boardwindow.cpp boardwindow.ui
    checkerholder.h checkerholder.cpp
    mainwindow.h mainwindow.cpp mainwindow.ui
    sidebarcheckerholder.h sidebarcheckerholder.cpp
    verticalchecker.h verticalchecker.cpp

    stringlistmodel.h stringlistmodel.cpp
    historylistmodel.h historylistmodel.cpp

    consts.h
    match.h match.cpp
    player.h player.cpp
    botplayer.h botplayer.cpp
    localplayer.h localplayer.cpp
    remoteplayer.h remoteplayer.cpp

    background_menu_image.qrc
    music.qrc
    botnetwork.qrc

    controller.h controller.cpp
    main.cpp
)
target_link_libraries(BackgammonPP PRIVATE Qt::Core Qt::Gui Qt::Network Qt::Multimedia)

if((QT_VERSION_MAJOR GREATER 4))
  target_link_libraries(BackgammonPP PRIVATE Qt::Widgets)
endif()

install(
  TARGETS BackgammonPP
  BUNDLE DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
