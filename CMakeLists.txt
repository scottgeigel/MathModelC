cmake_minimum_required(VERSION 2.8)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
include_directories(./)

project(ZombiesVsHumans)

add_subdirectory(InputHandler)
add_subdirectory(MapRenderer)
add_subdirectory(Model)
message("libs: ${ADDITONAL_LIBS}")
add_executable(ZombiesVsHumans main.c ModelApp.c Configuration.c)
target_link_libraries(ZombiesVsHumans inputhandler maprenderer renderer model)
