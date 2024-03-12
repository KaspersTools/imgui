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
cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(HBUI VERSION 0.1.0 LANGUAGES CXX C)

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
option(HBUI_GLFW_VULKAN "Use VULKAN WITH GLFW" ON)

###############################################################################
# HBUI
###############################################################################
set(HBUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(HBUI SHARED
)

list(APPEND HBUI_SOURCES
        ${HBUI_DIR}/include/HBUI/HBUIEnums.cpp
        ${HBUI_DIR}/HBUI/Sources/HBUI.cpp
        ${HBUI_DIR}/HBUI/Sources/HBWidgets.cpp
)
set(HBUI_HEADERS
        ${HBUI_DIR}/include/HBUI/HBUI.h
)

option(HBUI_WITH_DEBUG_WINDOW "HBUI_WITH_DEBUG_WINDOW" OFF)
target_compile_definitions(HBUI PUBLIC HBUI_WITH_DEBUG_WINDOW)
if (HBUI_WITH_DEBUG_WINDOW)
endif ()

list(APPEND HBUI_PRIVATE_HEADERS
        ${HBUI_DIR}/private/headers/Windows/HBDockSpaceWindow.h

        ${HBUI_DIR}/private/headers/Utils/HBIUpdatable.h
        ${HBUI_DIR}/private/headers/Utils/HBMath.h

        ${HBUI_DIR}/private/headers/types/HBFloat.h
)

set(GENERATED_SOURCES
        ${HBUI_DIR}/include/HBUI/fonts/Headers/Fonts.h
)
#
#target_precompile_headers(HBUI PRIVATE ${HBUI_DIR}/private/headers/pch.h)
#target_precompile_headers(HBUI PRIVATE ${HBUI_DIR}/HBUI/PubPch.h)

message(STATUS "GENERATED_SOURCES: ${GENERATED_SOURCES}")
target_sources(HBUI PRIVATE
        ${HBUI_DIR}/private/src/fonts/FontLoader.cpp
        ${HBUI_DIR}/private/src/UIItems/HBUIItemBase.cpp

        ${HBUI_DIR}/private/headers/UIItems/Interfaces/HBIWidget.cpp
        ${HBUI_DIR}/private/headers/UIItems/Windows/HBWindow.cpp
        ${HBUI_DIR}/private/headers/UIItems/Debugger/HBWidgetDebugger.cpp

        ${HBUI_DIR}/private/headers/UIItems/Properties/HBWidgetColorProperties.cpp
        ${HBUI_DIR}/private/headers/UIItems/Builders/HBWidgetColorPropertiesBuilder.cpp

        ${HBUI_DIR}/private/headers/UIItems/Bars/HBTaskBar.cpp
        ${HBUI_DIR}/private/headers/UIItems/Buttons/HBButton.cpp

        ${HBUI_DIR}/private/src/HBDebugWidget.cpp
)


target_sources(HBUI PRIVATE ${HBUI_PRIVATE_HEADERS})
target_sources(HBUI PRIVATE ${HBUI_SOURCES})
target_sources(HBUI PRIVATE ${UI_PANELS})

target_sources(HBUI PUBLIC ${HBUI_HEADERS})
target_include_directories(HBUI PUBLIC ${HBUI_DIR}/include)
target_include_directories(HBUI PUBLIC ${HBUI_DIR}/include/imgui)

target_include_directories(HBUI PRIVATE ${HBUI_DIR}/private/headers)
target_include_directories(HBUI PRIVATE ${HBUI_DIR}/private/Generated)

message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

target_compile_definitions(HBUI PUBLIC HBUI_VERSION="${PROJECT_VERSION}")#

if (HBUI_GLFW_VULKAN)
    add_subdirectory(../ImVK ${CMAKE_BINARY_DIR}/ImVK)
    target_link_libraries(HBUI PRIVATE ImVk)
endif ()


###############################################################################
# vendor
###############################################################################
add_subdirectory(vendor)
target_link_libraries(HBUI PRIVATE HBUI_VENDOR)

###############################################################################
# imgui
###############################################################################
target_link_libraries(HBUI PUBLIC imgui)

##############################################################################
#copy assets to CMAKE_RUNTIME_OUTPUT_DIRECTORY
##############################################################################\
add_custom_command(TARGET HBUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:HBUI>/assets
        COMMENT "Copying assets to $<TARGET_FILE_DIR:HBUI>/assets"
)


#add_custom_command(TARGET HBUI POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory


###############################################################################
# example
################################################################################
#add_subdirectory(../example ${CMAKE_BINARY_DIR}/example)
#target_link_libraries(ImVk-example PRIVATE HBUI)
#target_include_directories(ImVk-example PRIVATE ${HBUI_DIR}/include)