cmake_minimum_required(VERSION 3.8)
project(lab6)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp DynamicArray.h DynamicArrayException.h)
add_executable(lab6 ${SOURCE_FILES})