#
# CMake build system for Dear ImGui
# =================================
#
#   # Link Dear ImGui to your application:
#   #  HBUI - a main library target
#   target_link_libraries(YourApplication PRIVATE HBUI)
#
# A list of defined targets:
#   HBUI
cmake_minimum_required(VERSION 3.13)
project(HBUI)
###############################################################################
# options
###############################################################################
option(HBUI_GLFW "Use VULKAN WITH GLFW" ON)

###############################################################################
# ImVk
###############################################################################
set(HBUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(HBUI SHARED
        private/headers/Backend.h)

set(HBUI_SOURCES
        ${HBUI_DIR}/private/src/HBUI.cpp
        ${HBUI_DIR}/private/src/WindowManager.cpp
        ${HBUI_DIR}/private/src/UIWindow.cpp
        ${HBUI_DIR}/private/src/HBWidgets.cpp
)
set(HBUI_HEADERS
        ${HBUI_DIR}/include/HBUI/HBUI.h
        ${HBUI_DIR}/include/HBUI/UIWindow.h
        ${HBUI_DIR}/include/HBUI/WindowManager.h
)

target_sources(HBUI PRIVATE ${HBUI_SOURCES})
target_sources(HBUI PUBLIC  ${HBUI_HEADERS})

target_include_directories(HBUI PUBLIC ${HBUI_DIR}/include)
target_include_directories(HBUI PUBLIC ${HBUI_DIR}/include/imgui)

target_include_directories(HBUI PRIVATE ${HBUI_DIR}/private)

#target_include_directories(HBUI PUBLIC ${HBUI_DIR}/../imgui)
###############################################################################
# ImVk
###############################################################################

if (HBUI_GLFW)
    add_subdirectory(../ImVK ${CMAKE_BINARY_DIR}/ImVK)
    target_link_libraries(HBUI PRIVATE ImVk)
    target_include_directories(HBUI PRIVATE ${HBUI_DIR}/../ImVK/include)
endif ()

###############################################################################
# imgui
###############################################################################
target_link_libraries(HBUI PUBLIC imgui)
