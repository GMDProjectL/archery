cmake_minimum_required(VERSION 3.10.0)
project(archery VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(core)
add_subdirectory(cli)
add_subdirectory(gui)