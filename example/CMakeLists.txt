cmake_minimum_required(VERSION 3.13)
project(ImVk-example C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0077 NEW)
set(SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
    )

add_executable(ImVk-example ${SOURCES})