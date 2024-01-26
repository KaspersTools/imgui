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
#   set(IMGUI_SDL_TARGET sdl CACHE STRING "" FORCE)
#   # Include Dear ImGui into build.
#   add_subdirectory(path/to/imgui)
#   <..>
#   # Link Dear ImGui to your application:
#   #  imgui - a main library target
#   #  imgui-glfw: platform backend for Windows/Linux/MacOS/etc using GLFW.
#   #  imgui-vulkan: graphics backend for Windows/Linux/MacOS/etc using Vulkan graphics API.
#   target_link_libraries(YourApplication PUBLIC imgui imgui-glfw imgui-vulkan)
#
# A list of defined targets:
#   imgui-sdl2: platform backend for Windows/Linux/MacOS/etc using SDL2.
#   imgui-glfw: platform backend for Windows/Linux/MacOS/etc using GLFW.
#   imgui-osx: platform backend for MacOS.
#   imgui-win32: platform backend for Windows.
#   imgui-glut: platform backend for Windows/Linux/MacOS using glut/freeglut with OpenGL.
#   imgui-metal: graphics backend for MacOS using Metal graphics API.
#   imgui-opengl3: graphics backend for Windows/Linux/MacOS/etc using OpenGL3 graphics API.
#   imgui-opengl2: graphics backend for Windows/Linux/MacOS/etc using OpenGL2 graphics API.
#   imgui-vulkan: graphics backend for Windows/Linux/MacOS/etc using Vulkan graphics API.
#   imgui-dx9: graphics backend for Windows using DirectX 9 graphics API.
#   imgui-dx10: graphics backend for Windows using DirectX 10 graphics API.
#   imgui-dx11: graphics backend for Windows using DirectX 11 graphics API.
#   imgui-dx12: graphics backend for Windows using DirectX 12 graphics API.
#   imgui-sdlrenderer: graphics backend for platforms supported by SDL.
#
cmake_minimum_required(VERSION 3.13)
project(imgui)

# CMake: 3.13: option() honors normal variables. Sufficiently new CMake and this policy allow user to configure library
# by setting plain variables as opposed to cache variables as demonstrated in example.
cmake_policy(SET CMP0077 NEW)

###############################################################################
# Supported build parameters
###############################################################################
option(IMGUI_EXAMPLES "Build ImGui examples" ON)
option(IMGUI_DEMO "Include the ImGui demo window implementation in library" ON)

option(IMGUI_DISABLE_OBSOLETE_FUNCTIONS "Disable deprecated functions" ON)
option(IMGUI_DISABLE_OBSOLETE_KEYIO "Disable legacy input handling" ON)
option(IMGUI_DISABLE_FILE_FUNCTIONS "Disable use of file functions" OFF)
option(IMGUI_ENABLE_STDLIB_SUPPORT "Enable extra functions taking stdlib types" OFF)
option(IMGUI_ENABLE_COVERAGE "Enable coverage testing for supported compilers" OFF)
option(IMGUI_ENABLE_ASAN "Enable compiler-based sanitizers" OFF)
option(IMGUI_ENABLE_FREETYPE "Enable FreeType font rasterizer" ON)

option(IMGUI_IMPL_GLFW "Build the GLFW implementation (only if supported)" ON)
option(IMGUI_IMPL_VULKAN "Build the Vulkan implementation (only if supported)" ON)

set(IMGUI_GLFW_TARGET "" CACHE STRING "A custom GLFW target name that will be used for linking to examples.")
set(IMGUI_GLUT_TARGET "" CACHE STRING "A custom GLFW target name that will be used for linking to examples.")

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

# Helper variables. They allow to use this CMakeLists.txt file while it is in examples or in root directory. You may
# copy/symlink this file to imgui root directory in order to help IDEs like CLion to recognize headers as part of the
# project.
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/imgui.h)
    # CMakeLists.txt is in root directory.
    set(IMGUI_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "" FORCE)
elseif (EXISTS ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.h)
    # CMakeLists.txt is in parent directory.
    set(IMGUI_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/imgui CACHE STRING "" FORCE)
else ()
    # CMakeLists.txt is in examples directory.
    set(IMGUI_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE STRING "" FORCE)
endif ()

set(IMGUI_EXAMPLES_DIR ${IMGUI_ROOT_DIR}/examples CACHE STRING "" FORCE)
set(IMGUI_BACKENDS_DIR ${IMGUI_ROOT_DIR}/backends CACHE STRING "" FORCE)
set(IMGUI_MISC_DIR ${IMGUI_ROOT_DIR}/misc CACHE STRING "" FORCE)

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

# These warnings are overzealous in GCC and only practical to use with Clang.
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wshadow)
endif ()

# Disable warnings for third party source code. We can disable warnings for tracy, because it's source code is built as
# a part of our own build system. In case SDL2/GLFW/FreeType are used from a source distribution, we can not modify
# build flags of those targets from over here and thus they will have a warning spam.
file(GLOB_RECURSE THIRD_PARTY_SOURCE_FILES ${IMGUI_EXAMPLES_DIR}/libs/* imstb_*.c ../tracy/*.cpp)
set_source_files_properties(${THIRD_PARTY_SOURCE_FILES} PROPERTIES COMPILE_OPTIONS -w)

#if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-class-memaccess>)
#endif ()

###############################################################################
# Main library
###############################################################################
# Set up main library. It is created as an INTERFACE library because it allows using different
# IMGUI_USER_CONFIG defines in different targets of downstream projects.
file(GLOB IMGUI_SOURCE_FILES ${IMGUI_ROOT_DIR}/*.h ${IMGUI_ROOT_DIR}/*.cpp)
if (NOT IMGUI_DEMO)
    list(REMOVE_ITEM IMGUI_SOURCE_FILES ${IMGUI_ROOT_DIR}/imgui_demo.cpp)
endif ()
add_library(imgui INTERFACE)
target_link_libraries(imgui INTERFACE ${CMAKE_DL_LIBS})
target_sources(imgui INTERFACE ${IMGUI_SOURCE_FILES})
if (IMGUI_DISABLE_OBSOLETE_FUNCTIONS)
    target_compile_definitions(imgui INTERFACE -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1)
endif ()
if (IMGUI_DISABLE_OBSOLETE_KEYIO)
    target_compile_definitions(imgui INTERFACE -DIMGUI_DISABLE_OBSOLETE_KEYIO=1)
endif ()
if (IMGUI_DISABLE_FILE_FUNCTIONS)
    target_compile_definitions(imgui INTERFACE -DIMGUI_DISABLE_FILE_FUNCTIONS=1)
endif ()
if (IMGUI_USER_CONFIG)
    target_compile_definitions(imgui INTERFACE "-DIMGUI_USER_CONFIG=<${IMGUI_USER_CONFIG}>")
endif ()
target_include_directories(imgui INTERFACE ${IMGUI_ROOT_DIR})
if (NOT IMGUI_DEMO)
    target_compile_definitions(imgui INTERFACE -DIMGUI_DISABLE_DEMO_WINDOWS=1)
endif ()
if (IMGUI_ENABLE_STDLIB_SUPPORT)
    target_include_directories(imgui INTERFACE misc/cpp)
    target_sources(imgui INTERFACE misc/cpp/imgui_stdlib.h misc/cpp/imgui_stdlib.cpp)
endif ()
if (IMGUI_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(imgui INTERFACE -fprofile-arcs -ftest-coverage)
    target_link_libraries(imgui INTERFACE -fprofile-arcs -ftest-coverage)
    target_link_libraries(imgui INTERFACE gcov)
endif ()

if (IMGUI_ENABLE_ASAN AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(imgui INTERFACE -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    target_link_libraries(imgui INTERFACE -g -fsanitize=address)
endif ()

###############################################################################
# Freetype
###############################################################################
if (IMGUI_ENABLE_FREETYPE)
    add_library(imgui-freetype INTERFACE)
    target_sources(imgui-freetype INTERFACE
            ${IMGUI_MISC_DIR}/freetype/imgui_freetype.h
            ${IMGUI_MISC_DIR}/freetype/imgui_freetype.cpp
    )

    find_package(Freetype)
    if (FREETYPE_FOUND)
        target_include_directories(imgui-freetype INTERFACE ${FREETYPE_INCLUDE_DIRS})
        target_link_libraries(imgui-freetype INTERFACE ${FREETYPE_LIBRARIES})
    elseif (EXISTS ../freetype)
        set(SKIP_INSTALL_ALL ON)
        add_subdirectory(../freetype ${CMAKE_BINARY_DIR}/freetype)
        target_link_libraries(imgui-freetype INTERFACE freetype)
    else ()
        message(FATAL_ERROR "IMGUI_ENABLE_FREETYPE but FreeType was not found.")
    endif ()
    target_compile_definitions(imgui-freetype INTERFACE -DIMGUI_ENABLE_FREETYPE=1)
    target_link_libraries(imgui INTERFACE imgui-freetype)
endif ()

###############################################################################
# C++ standard library
###############################################################################
add_library(imgui-stdlib INTERFACE)
target_sources(imgui-stdlib INTERFACE
        ${IMGUI_MISC_DIR}/cpp/imgui_stdlib.h
        ${IMGUI_MISC_DIR}/cpp/imgui_stdlib.cpp
)

###############################################################################
# Backends
###############################################################################
# Ignore silently when required headers are missing.
set(CMAKE_REQUIRED_QUIET ON)
include(CheckIncludeFile)

# PkgConfig will be needed for SDL and glfw on some platforms.
find_package(PkgConfig QUIET)

if (IMGUI_IMPL_GLFW)
    if (IMGUI_GLFW_TARGET)
        # Custom user target was set. No auto-detection required.
    elseif (TARGET glfw)
        # GLFW exists already. Nothing to do.
    elseif (EXISTS ${IMGUI_ROOT_DIR}/../glfw/CMakeLists.txt)
        # When parent directory contains GLFW source code - we can build it directly.
        set(GLFW_STANDALONE OFF)
        set(GLFW_INSTALL OFF)
        set(GLFW_BUILD_DOCS OFF)
        add_subdirectory(${IMGUI_ROOT_DIR}/../glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
    elseif (MSVC)
        # We ship compiled libraries in our repository for Visual Studio.
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(IMGUI_GLFW_PLATFORM_BITS 64)
        elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(IMGUI_GLFW_PLATFORM_BITS 32)
        else ()
            message(FATAL_ERROR "Unsupported platform.")
        endif ()
        add_library(glfw INTERFACE IMPORTED)
        target_link_libraries(glfw INTERFACE ${IMGUI_ROOT_DIR}/examples/libs/glfw/lib-vc2010-${IMGUI_GLFW_PLATFORM_BITS}/glfw3.lib)
        target_include_directories(glfw INTERFACE ${IMGUI_ROOT_DIR}/examples/libs/glfw/include)
    elseif (PkgConfig_FOUND)
        # CMake 3.6 supports IMPORTED_TARGET parameter which creates PkgConfig::glfw target that we can easily link to.
        # TODO: Switch to using IMPORTED_TARGET when CMake minimal version is increased.
        pkg_check_modules(GLFW QUIET glfw3)
        if (GLFW_FOUND)
            add_library(glfw INTERFACE IMPORTED)
            target_link_libraries(glfw INTERFACE ${GLFW_LDFLAGS})
            target_compile_options(glfw INTERFACE ${GLFW_CFLAGS})
        endif ()
    endif ()
    if (NOT IMGUI_GLFW_TARGET)
        set(IMGUI_GLFW_TARGET glfw)
    endif ()
    if (TARGET ${IMGUI_GLFW_TARGET})
        add_library(imgui-glfw INTERFACE)
        target_sources(imgui-glfw INTERFACE
                ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.h
                ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp
        )
        target_link_libraries(imgui-glfw INTERFACE ${CMAKE_DL_LIBS} ${IMGUI_GLFW_TARGET})
        target_include_directories(imgui-glfw SYSTEM INTERFACE ${IMGUI_BACKENDS_DIR})
    else ()
        message(STATUS "IMGUI_IMPL_GLFW set to ON but GLFW could not be found.")
    endif ()
endif ()
if (IMGUI_IMPL_OPENGL2 OR IMGUI_IMPL_OPENGL3)
    set(OpenGL_GL_PREFERENCE "GLVND")   # GLVND is vendor-agnostic OpenGL dispatch library on Linux.
    include(FindOpenGL)
    if (NOT OPENGL_FOUND)
        message(FATAL_ERROR "IMGUI_IMPL_OPENGL2 or IMGUI_IMPL_OPENGL3 set to ON but OpenGL could not be found.")
    endif ()
    if (NOT TARGET OpenGL::GL)
        add_library(OpenGL::GL INTERFACE IMPORTED)
        target_include_directories(OpenGL::GL INTERFACE ${OPENGL_INCLUDE_DIR})
        target_link_libraries(OpenGL::GL INTERFACE ${OPENGL_LIBRARY})
    endif ()
    if (IMGUI_IMPL_OPENGL2)
        add_library(imgui-opengl2 INTERFACE)
        target_sources(imgui-opengl2 INTERFACE ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl2.cpp)
        target_link_libraries(imgui-opengl2 INTERFACE imgui OpenGL::GL)
        target_include_directories(imgui-opengl2 SYSTEM INTERFACE ${IMGUI_BACKENDS_DIR})
    endif ()
    if (IMGUI_IMPL_OPENGL3)
        add_library(imgui-opengl3 INTERFACE)
        target_sources(imgui-opengl3 INTERFACE ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp)
        target_link_libraries(imgui-opengl3 INTERFACE imgui OpenGL::GL)
        target_include_directories(imgui-opengl3 SYSTEM INTERFACE ${IMGUI_BACKENDS_DIR})
    endif ()
endif ()
if (IMGUI_IMPL_GLUT)
    find_package(GLUT QUIET)
    if (GLUT_glut_LIBRARY OR IMGUI_GLUT_TARGET)
        add_library(imgui-glut INTERFACE)
        target_sources(imgui-glut INTERFACE
                ${IMGUI_BACKENDS_DIR}/imgui_impl_glut.h
                ${IMGUI_BACKENDS_DIR}/imgui_impl_glut.cpp
        )
        target_link_libraries(imgui-glut INTERFACE imgui)
        if (IMGUI_GLUT_TARGET)
            target_link_libraries(imgui-glut INTERFACE ${IMGUI_GLUT_TARGET})
        else ()
            target_include_directories(imgui-glut SYSTEM INTERFACE ${GLUT_INCLUDE_DIR} ${IMGUI_BACKENDS_DIR})
            target_link_libraries(imgui-glut INTERFACE imgui ${GLUT_glut_LIBRARY})
            if (APPLE)
                target_link_libraries(imgui-glut INTERFACE ${GLUT_cocoa_LIBRARY})
            endif ()
        endif ()
    else ()
        message(STATUS "IMGUI_IMPL_GLUT set to ON but GLUT could not be found.")
    endif ()
endif ()

if (IMGUI_IMPL_VULKAN)
    find_package(Vulkan QUIET)
    if (CMAKE_CXX_STANDARD EQUAL 98)
        message(STATUS "IMGUI_IMPL_VULKAN set to ON but Vulkan SDK requires at least C++11.")
    elseif (NOT Vulkan_FOUND)
        message(STATUS "IMGUI_IMPL_VULKAN set to ON but Vulkan could not be found.")
    else ()
        add_library(imgui-vulkan INTERFACE)
        set(KDB_IMGUI_DIR ${IMGUI_ROOT_DIR}/KDB_ImGui)
        target_sources(imgui-vulkan INTERFACE
                ${KDB_IMGUI_DIR}/backends/imgui_impl_glfw_vulkan_window.h
                ${KDB_IMGUI_DIR}/backends/imgui_impl_glfw_vulkan_window.cpp
                ${KDB_IMGUI_DIR}/backends/imgui_impl_vulkan_image.h
                ${KDB_IMGUI_DIR}/backends/imgui_impl_vulkan_image.cpp

                ${KDB_IMGUI_DIR}/backends/debug/imgui_impl_glfw_vulkan_debug.h
                ${KDB_IMGUI_DIR}/backends/debug/imgui_impl_glfw_vulkan_debug.cpp

                ${KDB_IMGUI_DIR}/fonts/FontManager.h
                ${KDB_IMGUI_DIR}/fonts/FontManager.cpp

                ${KDB_IMGUI_DIR}/Extension.h
                ${KDB_IMGUI_DIR}/Extension.cpp

                ${IMGUI_BACKENDS_DIR}/imgui_impl_vulkan.cpp

        )
        target_link_libraries(imgui-vulkan INTERFACE imgui ${Vulkan_LIBRARIES})
        target_include_directories(imgui-vulkan SYSTEM INTERFACE ${Vulkan_INCLUDE_DIRS} ${IMGUI_BACKENDS_DIR})
    endif ()
endif ()

###############################################################################
# Examples
###############################################################################
if (IMGUI_EXAMPLES)
    set(IMGUI_EXAM_SRC
            KDB_ImGui/example/main.cpp
            KDB_ImGui/example/MainWindow.h
            KDB_ImGui/example/MainWindow.cpp)
    add_executable(KDB_ImGui ${IMGUI_EXAM_SRC})
    target_link_libraries(KDB_ImGui PUBLIC imgui imgui-glfw imgui-vulkan)
endif ()
