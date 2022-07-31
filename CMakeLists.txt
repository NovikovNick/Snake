cmake_minimum_required (VERSION 3.8)
project ("Snake")

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(Boost_DEBUG ON)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
add_definitions( -DBOOST_ALL_NO_LIB )
add_definitions( -DBOOST_ALL_DYN_LINK )
   
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB SRC_SERVICE "Snake/src/service/*.h")
file(GLOB SRC_SERVICE_IMPL "Snake/src/service/impl/*.cc")
file(GLOB SRC_UTIL "Snake/src/util/*.h" "Snake/src/util/*.cc")
file(GLOB SRC_WIN "Snake/src/win32/*.h" "Snake/src/win32/*.cc")
file(GLOB SRC_FILES ${SRC_SERVICE} ${SRC_SERVICE_IMPL} ${SRC_UTIL} ${SRC_WIN})

add_executable (Snake WIN32  "Snake/main.cc" ${SRC_FILES})

# unit tests

add_executable(CoordOperationTest "Snake/test/coord_operation_test.cc" ${SRC_FILES})
target_include_directories(CoordOperationTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_coord_operations COMMAND CoordOperationTest)

add_executable(SnakeMoveTest "Snake/test/snake_move_test.cc" ${SRC_FILES})
target_include_directories(SnakeMoveTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_SnakeMove COMMAND SnakeMoveTest)

add_executable(GameStateBufferTest "Snake/test/game_state_buffer_test.cc" ${SRC_FILES})
target_include_directories(GameStateBufferTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_GameStateBuffer COMMAND GameStateBufferTest)

add_executable(SnakeIsInBoundTest "Snake/test/snake_is_in_bound_test.cc" ${SRC_FILES})
target_include_directories(SnakeIsInBoundTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_SnakeIsInBound COMMAND SnakeIsInBoundTest)

add_executable(SnakeIsCollideTest "Snake/test/snake_is_collide_test.cc" ${SRC_FILES})
target_include_directories(SnakeIsCollideTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_SnakeIsCollide COMMAND SnakeIsCollideTest)

add_executable(PathfinderTest "Snake/test/pathfinder_test.cc" ${SRC_FILES})
target_include_directories(PathfinderTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_Pathfinder COMMAND PathfinderTest)

add_executable(SnakeIsValidTest "Snake/test/snake_is_valid_test.cc" ${SRC_FILES})
target_include_directories(SnakeIsValidTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_SnakeIsValid COMMAND SnakeIsValidTest)

message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
