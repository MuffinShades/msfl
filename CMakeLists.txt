cmake_minimum_required(VERSION 3.15)
project(msfl VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)
include(CTest)
enable_testing()

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.c
    src/*.hpp
    src/*.h
)

add_executable(msfl ${SOURCES})