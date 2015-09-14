cmake_minimum_required(VERSION 2.8)

include_directories(./)

project(ZombiesVsHumans)

add_subdirectory(InputHandler)
add_subdirectory(MapRenderer)
add_subdirectory(Model)
message("libs: ${ADDITONAL_LIBS}")
add_executable(ZombiesVsHumans main.c ConwayCell.c Configuration.c)
target_link_libraries(ZombiesVsHumans inputhandler maprenderer renderer model)
