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

file(GLOB SRC_MODEL "Snake/src/model/**/*.h" "Snake/src/model/**/*.cc")
file(GLOB SRC_SERVICE "Snake/src/service/**/*.h" "Snake/src/service/**/*.cc")
file(GLOB SRC_WIN "Snake/src/win32/*.h" "Snake/src/win32/*.cc")

file(GLOB SRC_FILES "Snake/src/util.h" ${SRC_WIN} ${SRC_SERVICE} ${SRC_MODEL})

add_executable (Snake WIN32 "Snake/main.cc" ${SRC_FILES})

# unit tests
add_executable(AStarAlgorithm "Snake/test/a_star_pathfinder_test.cc" ${SRC_FILES})
target_include_directories(AStarAlgorithm PRIVATE Boost::unit_test_framework)
add_test(NAME test_AStarAlgorithm COMMAND AStarAlgorithm)

add_executable(grid2d "Snake/test/grid.cc" ${SRC_FILES})
target_include_directories(grid2d PRIVATE Boost::unit_test_framework)
add_test(NAME test_grid2d COMMAND grid2d)

add_executable(grid_find_adjacents "Snake/test/grid_find_adjacents.cc" ${SRC_FILES})
target_include_directories(grid_find_adjacents PRIVATE Boost::unit_test_framework)
add_test(NAME test_grid_find_adjacents COMMAND grid_find_adjacents)


message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
