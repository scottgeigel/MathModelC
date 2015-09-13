cmake_minimum_required(VERSION 2.8)
project(Model)
add_library(model STATIC World.c Agent.c Model.c Random.c)
