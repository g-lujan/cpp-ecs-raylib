add_executable(perf_test perf_test.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES perf_test.cpp)

target_link_libraries(perf_test PRIVATE ecs_core)
