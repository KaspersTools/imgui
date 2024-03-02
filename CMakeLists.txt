################################################################################
cmake_minimum_required(VERSION 3.27.8)
project(imgui VERSION 1.0.0
        LANGUAGES CXX C OBJCXX OBJC)
cmake_policy(SET CMP0077 NEW)


if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()
if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()
if (NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif ()
if (NOT DEFINED CMAKE_CXX_STANDARD)

    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23)
endif ()
if(NOT DEFINED CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif ()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###############################################################################
# Warnings
###############################################################################
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wunused-parameter -Wformat -Wnarrowing -Wno-sign-conversion -Wno-error=declaration-after-statement) # $<$<COMPILE_LANGUAGE:CXX>:-pedantic>
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wnontrivial-memaccess)
    endif ()
    if (NOT APPLE)
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-deprecated-copy>)
    endif ()
endif ()
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wshadow)
endif ()


###############################################################################
# CMake Globals
###############################################################################
enable_language(C)
enable_language(CXX)
enable_language(OBJCXX)
enable_language(OBJC)

set(CMAKE_CXX_STANDARD 23)

#include(cmake/defaultsetup.cmake)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB sources *.cpp)
file(GLOB headers *.h)

message(STATUS "IMGUI_DIR: ${IMGUI_DIR}")
message(STATUS "sources: ${sources}")

add_library(imgui SHARED)
target_sources(imgui PRIVATE ${sources} misc/cpp/imgui_stdlib.cpp)
target_include_directories(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#################################################################################
## HBUI
#################################################################################
add_subdirectory(HBUI)
message(STATUS "CMAKEPROJECT NAME: ${CMAKE_PROJECT_NAME}")
#
################################################################################
## EXAMPLE
################################################################################
option(HBUI_BUILD_EXAMPLE "Build example" ON)
if(HBUI_BUILD_EXAMPLE)
    set(IMVK_DOWNLOAD_GLFW ON)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example/ ${CMAKE_BINARY_DIR}/example)
    target_link_libraries(ImVk-example PRIVATE HBUI)
endif()

