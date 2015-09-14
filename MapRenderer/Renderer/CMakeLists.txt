cmake_minimum_required(VERSION 2.8)
project(Renderer)
add_library(renderer STATIC Renderer.c Atlas.c)
target_link_libraries(renderer SDL2)
