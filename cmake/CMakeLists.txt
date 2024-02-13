cmake_minimum_required(VERSION 3.14)
project(imgui)

cmake_policy(SET CMP0077 NEW)

option(IMGUI_DEMO "Include the ImGui demo window implementation in library" ON)
option(IMGUI_DISABLE_OBSOLETE_FUNCTIONS "Disable deprecated functions" ON)
option(IMGUI_DISABLE_OBSOLETE_KEYIO "Disable legacy input handling" ON)
option(IMGUI_DISABLE_FILE_FUNCTIONS "Disable use of file functions" OFF)
option(IMGUI_ENABLE_STDLIB_SUPPORT "Enable extra functions taking stdlib types" OFF)
option(IMGUI_ENABLE_FREETYPE "Enable FreeType font rasterizer" ON)
#option(IMGUI_DISABLE_OBSOLETE_FUNCTIONS "Disable obsolete functions" ON)
#option(IMGUI_DISABLE_OBSOLETE_KEYIO "Disable obsolete key IO" ON)
#option(IMGUI_DISABLE_FILE_FUNCTIONS "Disable file functions" OFF)
#option(IMGUI_ENABLE_COVERAGE "Enable coverage" OFF)
#option(IMGUI_ENABLE_ASAN "Enable AddressSanitizer" OFF)
#option(IMGUI_ENABLE_FREETYPE "Enable FreeType" OFF)
#option(IMGUI_DEMO "Enable demo" ON)

###############################################################################
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

# These warnings are overzealous in GCC and only practical to use with Clang.
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wshadow)
endif ()

###############################################################################
# Main library
###############################################################################
# Set up main library. It is created as an INTERFACE library because it allows using different
# IMGUI_USER_CONFIG defines in different targets of downstream projects.
file(GLOB IMGUI_SOURCE_FILES
        ${IMGUI_ROOT_DIR}/*.h
        ${IMGUI_ROOT_DIR}/*.cpp)
#message(STATUS "IMGUI_SOURCE_FILES: ${IMGUI_SOURCE_FILES}")
#
#${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
#${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
#${IMGUI_DIR}/imgui.cpp
#${IMGUI_DIR}/imgui_draw.cpp
#${IMGUI_DIR}/imgui_demo.cpp
#${IMGUI_DIR}/imgui_tables.cpp
#${IMGUI_DIR}/imgui_widgets.cpp

message(STATUS "IMGUI_ROOT_DIR: ${IMGUI_ROOT_DIR}")
add_library(imgui SHARED)

target_link_libraries(imgui PRIVATE ${CMAKE_DL_LIBS})

message(STATUS "IMGUI_SOURCE_FILES: ${IMGUI_SOURCE_FILES}")
target_sources(imgui PRIVATE ${IMGUI_SOURCE_FILES})
target_precompile_headers(imgui PUBLIC ${IMGUI_ROOT_DIR}/ppImGui.h)

if (IMGUI_DISABLE_OBSOLETE_FUNCTIONS)
    target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1)
endif ()
if (IMGUI_DISABLE_OBSOLETE_KEYIO)
    target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_OBSOLETE_KEYIO=1)
endif ()
if (IMGUI_DISABLE_FILE_FUNCTIONS)
    target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_FILE_FUNCTIONS=1)
endif ()
if (IMGUI_USER_CONFIG)
    target_compile_definitions(imgui PUBLIC "-DIMGUI_USER_CONFIG=<${IMGUI_USER_CONFIG}>")
endif ()
if (NOT IMGUI_DEMO)
    target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_DEMO_WINDOWS=1)
endif ()
if (IMGUI_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(imgui PUBLIC -fprofile-arcs -ftest-coverage)
    target_link_libraries(imgui PUBLIC -fprofile-arcs -ftest-coverage)
    target_link_libraries(imgui PUBLIC gcov)
endif ()
if (IMGUI_ENABLE_ASAN AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(imgui PUBLI -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    target_link_libraries(imgui PUBLI -g -fsanitize=address)
endif ()


###############################################################################
# Misc
###############################################################################
set(IMGUI_MISC_DIR ${IMGUI_ROOT_DIR}/misc)

if (IMGUI_ENABLE_FREETYPE)
    message(STATUS "IMGUI_ENABLE_FREETYPE: ${IMGUI_ENABLE_FREETYPE}")
    add_library(imgui-freetype INTERFACE)
    target_sources(imgui-freetype INTERFACE
            ${IMGUI_MISC_DIR}/freetype/imgui_freetype.h
            ${IMGUI_MISC_DIR}/freetype/imgui_freetype.cpp
    )
    target_include_directories(imgui-freetype  INTERFACE ${IMGUI_ROOT_DIR})
    find_package(Freetype)
    message(STATUS "FREETYPE_FOUND: ${FREETYPE_FOUND}")
    if (FREETYPE_FOUND)
        message(STATUS "Freetype found: ${FREETYPE_INCLUDE_DIRS} ${FREETYPE_LIBRARIES}")
        target_include_directories(imgui-freetype INTERFACE ${FREETYPE_INCLUDE_DIRS})
        target_link_libraries(imgui-freetype INTERFACE ${FREETYPE_LIBRARIES})
    elseif (EXISTS ../freetype)
        message(STATUS "Freetype not found, building from source.")
        set(SKIP_INSTALL_ALL ON)
        add_subdirectory(../freetype ${CMAKE_BINARY_DIR}/freetype)
        target_link_libraries(imgui-freetype INTERFACE freetype)
    else ()
        message(FATAL_ERROR "IMGUI_ENABLE_FREETYPE but FreeType was not found.")
    endif ()
    target_compile_definitions(imgui-freetype INTERFACE -DIMGUI_ENABLE_FREETYPE=1)
    target_link_libraries(imgui INTERFACE imgui-freetype)
endif ()
