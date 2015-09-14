cmake_minimum_required(VERSION 2.8)
project(MapRenderer)

include_directories(../)
add_subdirectory(Renderer)
add_library(maprenderer STATIC MapRenderer.c)
