cmake_minimum_required(VERSION 3.16)
project(BackgammonPP VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

add_executable(BackgammonPP WIN32 MACOSX_BUNDLE
    engine/backgammon.cpp engine/backgammon.h
    engine/core/boardstate.cpp engine/core/boardstate.h
    engine/core/die.cpp engine/core/die.h
    engine/core/game.cpp engine/core/game.h
    engine/core/gameresult.h
    engine/core/move.cpp engine/core/move.h
    engine/core/playercolor.h
    engine/core/point.cpp engine/core/point.h
    engine/core/roll.cpp engine/core/roll.h
    engine/core/specialposition.h
    engine/core/turn.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    utility/jsonserializer.h
    utility/serializable.h
    utility/threadsafequeue.h

    engine/bot/bot.cpp engine/bot/bot.hpp engine/bot/connectgene.hpp engine/bot/genome.hpp engine/bot/innovation.cpp engine/bot/innovation.hpp  engine/bot/neat.cpp engine/bot/network.cpp engine/bot/network.hpp engine/bot/neuron.hpp engine/bot/pool.cpp engine/bot/pool.hpp engine/bot/species.hpp
    engine/bot/neat.hpp
    engine/bot/genome.cpp
    engine/bot/saved_genomes/lilith.genome
    engine/bot/saved_genomes/lilith_copy.genome
    engine/bot/nodegene.hpp
    engine/bot/link.hpp
    engine/bot/connectgene.cpp
)
target_link_libraries(BackgammonPP PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(BackgammonPP PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS BackgammonPP
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
