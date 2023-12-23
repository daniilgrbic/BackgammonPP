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

add_executable(BackgammonPPTest
    boardstate.test.cpp
    point.test.cpp
    ../engine/core/boardstate.cpp ../engine/core/boardstate.h
    ../engine/core/move.cpp ../engine/core/move.h
    ../engine/core/point.cpp ../engine/core/point.h
    ../engine/core/playercolor.h
    ../utility/jsonserializer.h
    ../utility/serializable.h
)

target_link_libraries(BackgammonPPTest PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(BackgammonPPTest PRIVATE Catch2::Catch2WithMain)
