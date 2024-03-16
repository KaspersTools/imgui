cmake_minimum_required(VERSION 3.13)
project(ImVk-example C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -g")

include(../cmake/defaultsetup.cmake)

cmake_policy(SET CMP0077 NEW)
set(SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
    )

add_executable(ImVk-example ${SOURCES})

MESSAGE(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
