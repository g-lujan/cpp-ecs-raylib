find_package(raylib CONFIG REQUIRED)

set(SOURCE_FILES 
	code/main.cpp 
	code/ecs/ecs.cpp 
	code/management/resources.cpp 
	code/management/resources.hpp
	code/management/settings.hpp
        code/graphics/graphics.cpp
	code/graphics/graphics.hpp
	code/graphics/tilemap.cpp
	code/graphics/tilemap.hpp
	code/utils/movement.cpp
	code/utils/movement.hpp
	code/AI/npc.cpp
	code/AI/npc.hpp
	code/management/serialization.cpp
	code/management/serialization.hpp
	code/utils/time.hpp
	code/utils/time.cpp
	code/utils/side.hpp

	code/components/collider.hpp
	code/components/tile.hpp
	code/components/kinematics.hpp
	code/components/player.hpp
	code/components/ai.hpp
	code/components/animation.hpp
	code/components/health.hpp
	code/components/input.hpp
	code/components/view.hpp
	code/components/controls.hpp
	code/components/controls.cpp
	code/systems/available_systems.hpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib opengl32 gdi32 winmm ecs_core)

file(COPY resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")