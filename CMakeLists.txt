cmake_minimum_required(VERSION 3.5)
project(CppConcurrency)
set(CMAKE_BUILD_TYPE Debug)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.5.0 REQUIRED COMPONENTS thread)

# 由于采用静态链接boost库，所以必须按照静态编译
if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT>
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT> 
    )
endif()

add_subdirectory(demos)
add_subdirectory(src)