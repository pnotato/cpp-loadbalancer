cmake_minimum_required(VERSION 3.31)

project(cpp-loadbalancer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(loadbalancer ${SOURCES})

target_include_directories(loadbalancer PRIVATE src)