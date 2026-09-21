cmake_minimum_required(VERSION 3.20)
project(GameboyEmulator)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL3 REQUIRED)

file(GLOB SOURCES "src/*.cpp")
add_executable(emulator ${SOURCES})

target_include_directories(emulator PRIVATE include)
target_link_libraries(emulator SDL3::SDL3)
