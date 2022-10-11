cmake_minimum_required(VERSION 3.11)
option(WINDOWS_BUILD "Compiles using MinGW" ON)
set(CMAKE_CXX_STANDARD 20)

project(game VERSION 0.0.1 DESCRIPTION "A game")


set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

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
	code/setup.hpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
file(COPY resources/images DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources")

if(WINDOWS_BUILD)
	message("Building on Windows")
	target_link_libraries(${PROJECT_NAME} raylib opengl32 gdi32 winmm)
else()
	message("Only Windows supported for now")
endif()
