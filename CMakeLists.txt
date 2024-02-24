################################################################################
cmake_minimum_required(VERSION 3.13 )
project(imgui LANGUAGES CXX)

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
    set(CMAKE_CXX_STANDARD 23)
endif ()

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

project(imgui)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})
#
file(GLOB sources *.cpp)
file(GLOB headers *.h)
message(STATUS "IMGUI_DIR: ${IMGUI_DIR}")
message(STATUS "sources: ${sources}")

add_library(imgui SHARED)
target_include_directories(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(imgui PRIVATE ${sources} misc/cpp/imgui_stdlib.cpp)

################################################################################
# HBUI
################################################################################
#add_subdirectory(HBUI)

###############################################################################
# EXAMPLE
###############################################################################
option(HBUI_BUILD_EXAMPLE "Build example" ON)
if(HBUI_BUILD_EXAMPLE)
    set(IMVK_DOWNLOAD_GLFW ON)
    add_subdirectory(HBUI)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example/ ${CMAKE_BINARY_DIR}/example)
    target_link_libraries(ImVk-example PRIVATE HBUI)
endif()

