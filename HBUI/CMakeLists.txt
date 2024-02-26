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
project(HBUI LANGUAGES CXX C OBJC)
set(HBUI_VERSION 0.1.0)

cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0079 NEW)

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

    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23)
endif ()
if(NOT DEFINED CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif ()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wshadow)
endif ()
#
##find it im:/Library/Developer/CommandLineTools/SDKs/12.1
#find_library(APPKIT_LIBRARY AppKit)
#if (NOT APPKIT_LIBRARY)
#    message(FATAL_ERROR "AppKit.framework NOT found!")
#else()
#    message(STATUS "AppKit.framework found! ${APPKIT_LIBRARY}")
#endif()
#
#find_library(FOUNDATION_LIBRARY Foundation)
#if (NOT FOUNDATION_LIBRARY)
#    message(FATAL_ERROR "Foundation.framework NOT found!")
#else()
#    message(STATUS "Foundation.framework found! ${FOUNDATION_LIBRARY}")
#endif()

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
option(HBUI_GLFW "Use VULKAN WITH GLFW" ON)

###############################################################################
# HBUI
###############################################################################
set(HBUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(HBUI SHARED)

list(APPEND HBUI_SOURCES
        ${HBUI_DIR}/include/HBUI/Sources/HBUI.cpp

        ${HBUI_DIR}/include/HBUI/Sources/HBUIEnums.cpp
        ${HBUI_DIR}/include/HBUI/Sources/HBWidgets.cpp
        ${HBUI_DIR}/include/HBUI/Sources/HBDebugWidget.cpp


        ${HBUI_DIR}/private/src/fonts/FontLoader.cpp
)
set(HBUI_HEADERS
        ${HBUI_DIR}/include/HBUI/HBUI.h
)

option(HBUI_WITH_DEBUG_WINDOW "HBUI_WITH_DEBUG_WINDOW" OFF)
target_compile_definitions(HBUI PUBLIC HBUI_WITH_DEBUG_WINDOW)
if (HBUI_WITH_DEBUG_WINDOW)
endif ()


list(APPEND HBUI_PRIVATE_HEADERS
        ${HBUI_DIR}/private/headers/Backend.h

        ${HBUI_DIR}/private/headers/Animation/Animations.h
        ${HBUI_DIR}/private/headers/Animation/Animation.h

        ${HBUI_DIR}/private/headers/Utils/HBIUpdatable.h
        ${HBUI_DIR}/private/headers/Utils/HBMath.h
        ${HBUI_DIR}/private/headers/Utils/UIWindow.h
        ${HBUI_DIR}/private/headers/Utils/WindowManager.h

        ${HBUI_DIR}/private/headers/UIItems/HBDefaultItems.h
        ${HBUI_DIR}/private/headers/UIItems/HBNewLine.h
        ${HBUI_DIR}/private/headers/UIItems/HBUIItemBase.h
        ${HBUI_DIR}/private/src/UIItems/HBUIItemBase.cpp

        ${HBUI_DIR}/private/headers/types/HBFloat.h

        ${HBUI_DIR}/private/headers/Primitives/Primitive.h

        ${HBUI_DIR}/private/headers/Panels/HBPanels.h

        ${HBUI_DIR}/private/src/UIItems/Panels/HBFlexPanel.cpp
        ${HBUI_DIR}/private/headers/Panels/HBFlexPanel.h

        ${HBUI_DIR}/private/headers/Panels/Menus/HBSideBar.h
        ${HBUI_DIR}/private/headers/Panels/Menus/HBSideBarButton.h

        ${HBUI_DIR}/private/headers/fonts/FontLoader.h
)

set(GENERATED_SOURCES
        ${HBUI_DIR}/private/Generated/fa-brands/fontAwesome.cpp
        ${HBUI_DIR}/private/Generated/fa-brands/fontAwesome.hpp
        ${HBUI_DIR}/private/Generated/fa-brands/fontAwesome_codes.hpp
        ${HBUI_DIR}/private/Generated/fa-brands/fontAwesome_loader.hpp


        ${HBUI_DIR}/private/headers/fonts/IconsFontAwesome6.h
        ${HBUI_DIR}/private/headers/fonts/IconsFontAwesome6Brands.h
)

message(STATUS "GENERATED_SOURCES: ${GENERATED_SOURCES}")

target_sources(HBUI PUBLIC  ${HBUI_HEADERS})

target_sources(HBUI PRIVATE ${GENERATED_SOURCES})
target_sources(HBUI PRIVATE ${HBUI_SOURCES})
target_sources(HBUI PRIVATE ${HBUI_PRIVATE_HEADERS})

target_include_directories(HBUI PUBLIC ${HBUI_DIR}/include)
target_include_directories(HBUI PUBLIC ${HBUI_DIR}/include/imgui)

target_include_directories(HBUI PRIVATE ${HBUI_DIR}/private/headers)
target_include_directories(HBUI PRIVATE ${HBUI_DIR}/private/Generated)

message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

target_compile_definitions(HBUI PUBLIC HBUI_VERSION="${PROJECT_VERSION}")#

if (HBUI_GLFW)
    add_subdirectory(../ImVK ${CMAKE_BINARY_DIR}/ImVK)
    target_link_libraries(HBUI PRIVATE ImVk)
endif ()


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