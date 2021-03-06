cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
include_directories(./)

project(AgentBasedModel)

add_subdirectory(InputHandler)
add_subdirectory(MapRenderer)
add_subdirectory(Model)
add_subdirectory(Types)
message("libs: ${ADDITONAL_LIBS}")

set (BASE_FILES
    main.c
)
set (ZOMBIE_VS_HUMANS_SOURCE_FILES
    ${BASE_FILES}
    ModelApps/ZombiesVsHumans/ModelApp.c
    ModelApps/ZombiesVsHumans/Humans.c
    ModelApps/ZombiesVsHumans/Zombies.c
    ModelApps/ZombiesVsHumans/Configuration.c
)

set (CONWAYS_GAME_OF_LIFE_SOURCE_FILES
    ${BASE_FILES}
    ModelApps/ConwaysGameOfLife/ModelApp.c
    ModelApps/ConwaysGameOfLife/Configuration.c
)

add_executable(zombies_vs_humans ${ZOMBIE_VS_HUMANS_SOURCE_FILES})
target_include_directories(zombies_vs_humans PUBLIC ModelApps/ZombiesVsHumans/)
target_link_libraries(zombies_vs_humans inputhandler maprenderer renderer model types)

add_executable(conways_game_of_life ${CONWAYS_GAME_OF_LIFE_SOURCE_FILES})
target_include_directories(conways_game_of_life PUBLIC ModelApps/ConwaysGameOfLife/)
target_link_libraries(conways_game_of_life inputhandler maprenderer renderer model types)
