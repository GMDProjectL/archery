cmake_minimum_required(VERSION 3.10.0)
project(archery-gui VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

find_package(Qt6 REQUIRED Core Widgets Gui)

add_executable(archery-gui ${SOURCES})

target_include_directories(archery-gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(archery-gui archery-core Qt6::Core Qt6::Widgets Qt6::Gui)