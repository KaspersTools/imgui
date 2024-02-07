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

# Minimal supported C++ standard.
if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif ()

set(HBUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(imgui-extension STATIC)

set(HBUI_SOURCES
        ${HBUI_DIR}/src/HBUI.cpp
        ${HBUI_DIR}/src/WindowManager.cpp
)

set(HBUI_HEADERS
        ${HBUI_DIR}/include/HBUI/HBUI.h
        ${HBUI_DIR}/include/HBUI/UIWindow.h
        ${HBUI_DIR}/include/HBUI/WindowManager.h)
target_sources(imgui-extension PUBLIC
        ${HBUI_SOURCES}
)

target_link_libraries(imgui-extension PUBLIC imgui)
target_include_directories(imgui-extension PUBLIC ${HBUI_DIR}/include)