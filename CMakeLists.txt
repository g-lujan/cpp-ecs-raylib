cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)

project(game VERSION 0.0.1 DESCRIPTION "A game")

add_subdirectory(ecs_core)
add_subdirectory(game)