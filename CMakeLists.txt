cmake_minimum_required(VERSION 3.10)
project(FlappyBirdStyleGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(FlappyBirdStyleGame
    src/main.cpp
    src/player.cpp
    src/asteroid.cpp
    src/saveload.cpp
    src/datamanager.cpp
)

target_link_libraries(FlappyBirdStyleGame sfml-graphics sfml-window sfml-system)
