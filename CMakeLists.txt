cmake_minimum_required (VERSION 3.8)
project ("Snake")

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(BOOST_ROOT "C:\\local\\boost_1_80_0")
set(Boost_DEBUG OFF)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
add_definitions( -DBOOST_ALL_NO_LIB )
add_definitions( -DBOOST_ALL_DYN_LINK )
   
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB SRC_SERVICE "Snake/src/service/*.h")
file(GLOB SRC_SERVICE_IMPL "Snake/src/service/impl/*.cc")
file(GLOB SRC_WIN "Snake/src/win32/*.h" "Snake/src/win32/*.cc")

file(GLOB SRC_AI_HEADERS  "Snake/src/ai/*.h")
file(GLOB SRC_AI_IMPL "Snake/src/ai/impl/*.cc")
file(GLOB SRC_MODEL "Snake/src/model/*.h")

file(GLOB SRC_FILES ${SRC_SERVICE} ${SRC_SERVICE_IMPL} ${SRC_WIN} ${SRC_AI_HEADERS} ${SRC_AI_IMPL})



add_executable (Snake WIN32  "Snake/main.cc" "Snake/src/model/common_models.h" ${SRC_FILES})

# unit tests

add_executable(CoordOperationTest "Snake/test/coord_operation_test.cc" ${SRC_FILES})
target_include_directories(CoordOperationTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_coord_operations COMMAND CoordOperationTest)

add_executable(SnakeMoveTest "Snake/test/snake_move_test.cc" ${SRC_FILES})
target_include_directories(SnakeMoveTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_snake_move COMMAND SnakeMoveTest)

add_executable(GameStateBufferTest "Snake/test/game_state_buffer_test.cc" ${SRC_FILES})
target_include_directories(GameStateBufferTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_game_state_buffer COMMAND GameStateBufferTest)

add_executable(GameStateBufferExceptionSafetyTest "Snake/test/game_state_buffer_exception_safety_test.cc" ${SRC_FILES})
target_include_directories(GameStateBufferExceptionSafetyTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_game_state_buffer_exception_safety COMMAND GameStateBufferExceptionSafetyTest)

add_executable(SnakeIsInBoundTest "Snake/test/snake_is_in_bound_test.cc" ${SRC_FILES})
target_include_directories(SnakeIsInBoundTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_snake_is_inbound COMMAND SnakeIsInBoundTest)

add_executable(SnakeIsCollideTest "Snake/test/snake_is_collide_test.cc" ${SRC_FILES})
target_include_directories(SnakeIsCollideTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_snake_is_collide COMMAND SnakeIsCollideTest)

add_executable(PathfinderTest "Snake/test/pathfinder_test.cc" ${SRC_AI_HEADERS} ${SRC_AI_IMPL})
target_include_directories(PathfinderTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_pathfinder COMMAND PathfinderTest)

add_executable(SnakeIsValidTest "Snake/test/snake_is_valid_test.cc" ${SRC_FILES})
target_include_directories(SnakeIsValidTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_snake_is_valid COMMAND SnakeIsValidTest)

add_executable(AStarAlgorithm "Snake/test/a_star_pathfinder_test.cc" ${SRC_MODEL} ${SRC_AI_HEADERS} ${SRC_AI_IMPL})
target_include_directories(AStarAlgorithm PRIVATE Boost::unit_test_framework)
add_test(NAME test_AStarAlgorithm COMMAND AStarAlgorithm)

add_executable(grid2d "Snake/test/grid.cc" ${SRC_MODEL} ${SRC_AI_HEADERS} ${SRC_AI_IMPL})
target_include_directories(grid2d PRIVATE Boost::unit_test_framework)
add_test(NAME test_grid2d COMMAND grid2d)

add_executable(grid_find_adjacents "Snake/test/grid_find_adjacents.cc" ${SRC_MODEL})
target_include_directories(grid_find_adjacents PRIVATE Boost::unit_test_framework)
add_test(NAME test_grid_find_adjacents COMMAND grid_find_adjacents)


message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
