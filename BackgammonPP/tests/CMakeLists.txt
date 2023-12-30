cmake_minimum_required(VERSION 3.16)

project(BackgammonPPTest LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 20)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)

Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

include_directories("../")

add_executable(BackgammonPPTest
    ../engine/core/boardstate.cpp ../engine/core/boardstate.h
    ../engine/core/move.cpp ../engine/core/move.h
    ../engine/core/point.cpp ../engine/core/point.h
    ../engine/core/turn.cpp ../engine/core/turn.h
    ../engine/core/turntrie.cpp ../engine/core/turntrie.h
    ../engine/core/die.cpp ../engine/core/die.h
    ../engine/core/game.cpp ../engine/core/game.h
    ../engine/core/gameresult.h
    ../engine/core/playercolor.h
    ../engine/core/point.cpp
    ../engine/core/point.h
    ../engine/core/roll.cpp
    ../engine/core/roll.h
    ../engine/core/specialposition.h
    ../engine/core/turn.h
    ../engine/backgammon.cpp
    ../engine/backgammon.h
    ../engine/longnardy.cpp
    ../engine/longnardy.h
    ../utility/jsonserializer.h
    ../utility/serializable.h
    ../stringlistmodel.h ../stringlistmodel.cpp
    ../historylistmodel.h ../historylistmodel.cpp
    ../engine/core/game.h ../engine/core/game.cpp
    ../engine/backgammon.h ../engine/backgammon.cpp
    ../utility/threadsafequeue.h

    boardstate.test.cpp
    longnardy.test.cpp
    move.test.cpp
    point.test.cpp
    turn.test.cpp
    stringlistmode.test.cpp
    historylistmodel.test.cpp
    game.test.cpp
    backgammon.test.cpp
    threadsafequeue.test.cpp
    roll.test.cpp
    die.test.cpp
    turntrie.test.cpp
)

target_link_libraries(BackgammonPPTest PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(BackgammonPPTest PRIVATE Catch2::Catch2WithMain)

