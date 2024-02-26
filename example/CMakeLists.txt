cmake_minimum_required(VERSION 3.13)
project(ImVk-example C CXX)
include(../cmake/defaultsetup.cmake)

cmake_policy(SET CMP0077 NEW)
set(SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
    )

add_executable(ImVk-example ${SOURCES})