cmake_minimum_required(VERSION 2.8)

project(ZombiesVsHumans)
add_subdirectory(InputHandler)
add_subdirectory(Renderer)

add_executable(ZombiesVsHumans main.c)
target_link_libraries(ZombiesVsHumans inputhandler renderer SDL2)
