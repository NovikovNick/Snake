﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Snake")


# Include sub-projects.
add_subdirectory ("Snake")

set_property(TARGET Snake PROPERTY CXX_STANDARD 17)