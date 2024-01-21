cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(game VERSION 0.0.1 DESCRIPTION "A game")

add_subdirectory(ecs_core)
add_subdirectory(game)
add_subdirectory(perf_test)