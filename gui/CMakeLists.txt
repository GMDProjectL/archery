cmake_minimum_required(VERSION 3.10.0)
project(archery-gui VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(archery-gui ${SOURCES})

target_include_directories(archery-gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)