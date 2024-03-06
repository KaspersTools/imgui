################################################################################
cmake_minimum_required(VERSION 3.27.8)
project(imgui VERSION 1.0.0
        LANGUAGES CXX C)


set(CMAKE_CXX_STANDARD 23 CACHE STRING "The C++ standard whose features are requested.")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "The C++ standard is required.")
set(CMAKE_CXX_EXTENSIONS ON CACHE BOOL "When C++ is enabled, this flag is passed to the compiler.")


#####SET OUTPUT DIRECTORIES AND SAVE FOR CACHE
if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables.")
endif ()
if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries")
endif ()
if (NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Single Directory for all static libraries.")
endif ()


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

################################################################################
## EXAMPLE
################################################################################
option(HBUI_BUILD_EXAMPLE "Build example" ON)
if(HBUI_BUILD_EXAMPLE)
    set(IMVK_DOWNLOAD_GLFW ON)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example/ ${CMAKE_BINARY_DIR}/example)
    target_link_libraries(ImVk-example PRIVATE HBUI)
endif()

