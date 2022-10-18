cmake_minimum_required(VERSION 3.11)
option(WINDOWS_BUILD "Compiles using MinGW" ON)
set(CMAKE_CXX_STANDARD 20)

project(game VERSION 0.0.1 DESCRIPTION "A game")


find_package(raylib CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)

# Our Project
set(SOURCE_FILES 
	code/main.cpp 
	code/components.hpp
	code/ecs.cpp 
	code/ecs.hpp 
	code/resources.cpp 
	code/resources.hpp
	code/settings.hpp
	code/setup.cpp 
	code/setup.hpp
      code/graphics.cpp
	code/graphics.hpp
	code/tilemap.cpp
	code/tilemap.hpp
      external/json.hpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
file(COPY resources/images DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources")

if(WINDOWS_BUILD)
	message("Building on Windows")
	target_link_libraries(${PROJECT_NAME} PRIVATE raylib opengl32 gdi32 winmm box2d::box2d)
else()
	message("Only Windows supported for now")
endif()