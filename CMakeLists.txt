cmake_minimum_required(VERSION 3.5)
project(CppConcurrency)
set(CMAKE_BUILD_TYPE Debug)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.5.0 REQUIRED COMPONENTS thread)

add_subdirectory(demos)
add_subdirectory(src)