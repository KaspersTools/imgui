#
# CMake build system for Dear ImGui
# =================================
#
# Build instructions:
# 1. Install latest CMake
#   * Windows: https://cmake.org/download/ (Tick checkbox to place cmake in system PATH)
#   * Linux: from your favorite package manager
#   * MacOS: brew install cmake
# 2. Open command prompt in directory containing "imgui" and "imgui_dev" folders
#   * Windows: open Visual Studio tools command prompt or specify -G "Generator Name" (quotes are important).
#   * Other platforms: Specifying generator name is optional.
# 3. Create a build directory and configure build
#    git clone https://github.com/ocornut/imgui.git
#    mkdir cmake-build
#    cd cmake-build
#    cmake <optional build parameters> ../imgui
# 4. Build
#   * Windows (Visual Studio generators): Open generated imgui_dev.sln
#   * All platforms: cmake --build .
#
# Example of using Dear ImGui in your CMake project:
#
#   # Set various library options:
#   set(IMGUI_VULKAN_TARGET vulkan CACHE STRING "" FORCE)
#   # Include Dear ImGui into build.
#   add_subdirectory(path/to/imgui)
#   <..>
#   # Link Dear ImGui to your application:
#   #  imgui-extension - a main library target
#   target_link_libraries(YourApplication PUBLIC imgui-extension)
#
# A list of defined targets:
#   imgui-extension: graphics backend for Windows/Linux/MacOS/etc using Vulkan graphics API.

cmake_minimum_required(VERSION 3.13)
project(imgui)

# CMake: 3.13: option() honors normal variables. Sufficiently new CMake and this policy allow user to configure library
# by setting plain variables as opposed to cache variables as demonstrated in example.
cmake_policy(SET CMP0077 NEW)

###############################################################################
# Supported build parameters
###############################################################################
###############################################################################

if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(IMGUI_IMPL_GLFW OFF)
    set(IMGUI_IMPL_GLUT OFF)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

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

set(HBUI_DIR ${IMGUI_ROOT_DIR}/HBUI)
add_library(imgui-extension INTERFACE)
target_sources(imgui-extension INTERFACE PUBLIC
        ${HBUI_DIR}/include/HBUI/HBUI.h
)
target_link_libraries(imgui-extension INTERFACE imgui)
target_include_directories(imgui-extension SYSTEM INTERFACE ${HBUI_DIR}/include)