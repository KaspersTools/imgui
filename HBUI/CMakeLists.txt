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
project(imgui-extension)

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

set(IMGUI_ROOT_DIR ${HBUI_DIR}/../)
set(IMGUI_SOURCE_FILES
        ${IMGUI_ROOT_DIR}/imgui.cpp
        ${IMGUI_ROOT_DIR}/imgui.h
)

target_sources(imgui-extension PUBLIC
        ${HBUI_SOURCES}
        ${IMGUI_SOURCE_FILES}
)

message(STATUS "HBUI_SOURCES: ${IMGUI_ROOT_DIR}")

#check if target imgui already excitst
if (NOT TARGET imgui)
    set(IMGUI_ENABLE_STDLIB_SUPPORT OFF CACHE BOOL "" FORCE)
    set(IMGUI_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(IMGUI_DEMO OFF CACHE BOOL "" FORCE)
    set(IMGUI_ENABLE_FREETYPE OFF CACHE BOOL "" FORCE)
    set(IMGUI_IMPL_GLFW OFF CACHE BOOL "" FORCE)
    set(IMGUI_IMPL_VULKAN OFF CACHE BOOL "" FORCE)
    set(IMGUI_EXTENSION OFF CACHE BOOL "" FORCE)
    set(IMGUI_USER_CONFIG OFF CACHE BOOL "" FORCE)
    add_subdirectory(${IMGUI_ROOT_DIR} ${CMAKE_BINARY_DIR}/imgui)
endif ()

target_link_libraries(imgui-extension PRIVATE imgui)
target_include_directories(imgui-extension PUBLIC ${HBUI_DIR}/include)