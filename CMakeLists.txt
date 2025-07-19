cmake_minimum_required(VERSION 3.10)
project(SpaceExplorationGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
file(GLOB SRC_FILES src/*.cpp)
file(GLOB SCENE_FILES src/scenes/*.cpp)

add_executable(SpaceExplorationGame
    ${SRC_FILES}
    ${SCENE_FILES}
)

target_link_libraries(SpaceExplorationGame sfml-graphics sfml-window sfml-system)
