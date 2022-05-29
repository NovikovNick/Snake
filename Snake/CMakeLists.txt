# CMakeList.txt : CMake project for Snake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (Snake WIN32  "main.cpp" "GameModels.h" "GameLoopService.cpp" "InputService.cpp" "InputService.h" "RenderService.h" "RenderService.cpp" "GameLoopService.h" "GameLogicService.h" "GameLogicService.cpp" "windows/BaseWindow.h" "windows/MainWindow.h" "windows/MainWindow.cpp" "GameStateHolder.h" "GameStateHolder.cpp")


# TODO: Add tests and install targets if needed.
