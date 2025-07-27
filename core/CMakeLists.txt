cmake_minimum_required(VERSION 3.10.0)
project(archery-core VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

find_package(Qt6 REQUIRED Core)

add_library(archery-core ${SOURCES})

target_include_directories(archery-core PUBLIC include)
target_link_libraries(archery-core Qt6::Core)