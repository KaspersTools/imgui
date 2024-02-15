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
project(HBUI LANGUAGES CXX)
set(HBUI_VERSION 0.1.0)

#set project cmake project version and set define
set(PROJECT_VERSION ${HBUI_VERSION})

###############################################################################
# DATA
###############################################################################
define_property(GLOBAL PROPERTY HBUI_DATA
        BRIEF_DOCS "HBUI data"
        FULL_DOCS "HBUI data"
        )

###############################################################################
# options
###############################################################################
option(HBUI_GLFW "Use VULKAN WITH GLFW" OFF)

###############################################################################
# HBUI
###############################################################################
set(HBUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(HBUI SHARED
        include/HBUI/HBMath.h
)

list(APPEND HBUI_SOURCES
        ${HBUI_DIR}/private/src/HBUI.cpp
        ${HBUI_DIR}/private/src/WindowManager.cpp
        ${HBUI_DIR}/private/src/UIWindow.cpp
        ${HBUI_DIR}/private/src/HBWidgets.cpp
)

option(HBUI_WITH_DEBUG_WINDOW "HBUI_WITH_DEBUG_WINDOW" ON)
if (HBUI_WITH_DEBUG_WINDOW)
    list(APPEND HBUI_SOURCES
            ${HBUI_DIR}/private/src/HBDebugWidget.cpp
    )
    target_compile_definitions(HBUI PUBLIC HBUI_WITH_DEBUG_WINDOW)
endif ()

set(HBUI_HEADERS
        ${HBUI_DIR}/include/HBUI/HBUI.h
        ${HBUI_DIR}/include/HBUI/UIWindow.h
        ${HBUI_DIR}/include/HBUI/WindowManager.h
)

list(APPEND HBUI_PRIVATE_HEADERS
        ${HBUI_DIR}/private/headers/Backend.h
)

target_sources(HBUI PRIVATE ${HBUI_SOURCES} ${HBUI_PRIVATE_HEADERS})
target_sources(HBUI PUBLIC ${HBUI_HEADERS})

target_compile_definitions(HBUI PUBLIC HBUI_VERSION="${PROJECT_VERSION}")

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
