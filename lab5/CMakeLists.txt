cmake_minimum_required(VERSION 3.8)
project(lab5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(SOURCE_FILES main.cpp Decomposition.cpp Decomposition.h Task.h Task.cpp)
add_executable(lab5 ${SOURCE_FILES})