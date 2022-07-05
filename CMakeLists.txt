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
file(GLOB SRC_SERVICE_IMPL "Snake/src/service/impl/*.cpp")
file(GLOB SRC_UTIL "Snake/src/util/*.h" "Snake/src/util/*.cpp")
file(GLOB SRC_WIN "Snake/src/win32/*.h" "Snake/src/win32/*.cpp")
file(GLOB SRC_FILES ${SRC_SERVICE} ${SRC_SERVICE_IMPL} ${SRC_UTIL} ${SRC_WIN})

add_executable (Snake WIN32  "Snake/main.cpp" ${SRC_FILES})

# unit tests

add_executable(CoordOperationTest "Snake/test/CoordOperationTest.cpp" ${SRC_FILES})
target_include_directories(CoordOperationTest PRIVATE Boost::unit_test_framework)
add_test(NAME test_coord_operations COMMAND CoordOperationTest)


message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
