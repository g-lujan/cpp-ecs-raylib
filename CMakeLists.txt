cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)

project(game VERSION 0.0.1 DESCRIPTION "A game")

find_package(raylib CONFIG REQUIRED)

set(SOURCE_FILES 
	code/main.cpp 
	code/components.hpp
	code/ecs.cpp 
	code/ecs.hpp 
	code/resources.cpp 
	code/resources.hpp
	code/settings.hpp
    code/graphics.cpp
	code/graphics.hpp
	code/tilemap.cpp
	code/tilemap.hpp
	code/movement.cpp
	code/movement.hpp
    external/json.hpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
file(COPY resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")


target_link_libraries(${PROJECT_NAME} PRIVATE raylib opengl32 gdi32 winmm)
