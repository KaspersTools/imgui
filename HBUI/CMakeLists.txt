#
# CMake build system for Dear ImGui
# =================================
#
#   # Link Dear ImGui to your application:
#   #  imgui-extension - a main library target
#   target_link_libraries(YourApplication PRIVATE imgui-extension)
#
# A list of defined targets:
#   imgui-extension

cmake_minimum_required(VERSION 3.13)
project(imgui)

# CMake: 3.13: option() honors normal variables. Sufficiently new CMake and this policy allow user to configure library
cmake_policy(SET CMP0077 NEW)

# Tidy up build dir, but only if user has not configured it yet.
if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()
if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()
if (NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif ()

# Minimal supported C++ standard.
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23)
endif ()

if(NOT DEFINED ${IMGUI_ROOT_DIR})
    set(IMGUI_ROOT_DIR ../)
endif()

set(HBUI_DIR ${IMGUI_ROOT_DIR}/HBUI)
add_library(imgui-extension STATIC)

set(HBUI_SOURCES
        ${HBUI_DIR}/src/WindowManager.cpp)

set(HBUI_HEADERS
        ${HBUI_DIR}/include/HBUI/HBUI.h
        ${HBUI_DIR}/include/HBUI/UIWindow.h
        ${HBUI_DIR}/include/HBUI/WindowManager.h)

target_sources(imgui-extension INTERFACE PUBLIC
        ${HBUI_SOURCES}
        ${HBUI_HEADERS}
)

#if there is no target imgui
if(NOT TARGET imgui)
    file(GLOB SUB_FOLDERS LIST_DIRECTORIES true "*")
    list(FILTER SUB_FOLDERS EXCLUDE REGEX "sub_dir_z")
    target_include_directories(imgui-extension PRIVATE ${SUB_FOLDERS})
    target_include_directories(imgui-extension PUBLIC ${IMGUI_ROOT_DIR})
endif ()
target_include_directories(imgui-extension PUBLIC ${HBUI_DIR}/include)
