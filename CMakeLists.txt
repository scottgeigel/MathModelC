cmake_minimum_required(VERSION 2.8)

include_directories(./)
project(ZombiesVsHumans)
add_subdirectory(InputHandler)
add_subdirectory(Renderer)
add_subdirectory(Model)

add_executable(ZombiesVsHumans main.c MapRenderer.c ConwayCell.c)
target_link_libraries(ZombiesVsHumans inputhandler renderer model SDL2)
