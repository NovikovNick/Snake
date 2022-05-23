# CMakeList.txt : CMake project for Snake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (Snake WIN32  "Main.cpp" "GameModels.h" "GameLoopService.cpp" "InputService.cpp" "InputService.h" "RenderService.h" "RenderService.cpp" "GameLoopService.h" "GameLogicService.h" "GameLogicService.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Snake PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
