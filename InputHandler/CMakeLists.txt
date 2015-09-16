cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
project(InputHandler)
add_library(inputhandler STATIC InputHandler.c)
