cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
include_directories(./)

project(ConwaysGameOfLife)

add_subdirectory(InputHandler)
add_subdirectory(MapRenderer)
add_subdirectory(Model)
add_subdirectory(Types)
message("libs: ${ADDITONAL_LIBS}")
add_executable(ConwaysGameOfLife main.c ModelApp.c Humans.c Zombies.c Configuration.c)
target_link_libraries(ConwaysGameOfLife inputhandler maprenderer renderer model types)
