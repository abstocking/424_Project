cmake_minimum_required(VERSION 3.23) # Specify CMake version.
project(project) # A project name. This can be anything.
set(CMAKE_CXX_STANDARD 20) # Specify C++ version.
add_executable(project project.cpp) # source files.

find_package (Threads)
target_link_libraries (project Threads::Threads)