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

enable_language(CXX)
enable_language(OBJC)

MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )
MESSAGE( STATUS "CMAKE_OBJC_FLAGS: " ${CMAKE_OBJC_FLAGS} )

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
)

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
if (HBUI_WITH_DEBUG_WINDOW)
#    list(APPEND HBUI_SOURCES
#            ${HBUI_DIR}/private/src/Debug/HBDebugWidget.cpp
#    )
#    target_compile_definitions(HBUI PUBLIC HBUI_WITH_DEBUG_WINDOW)
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

target_sources(HBUI PRIVATE ${GENERATED_SOURCES})
target_sources(HBUI PRIVATE ${HBUI_SOURCES})
target_sources(HBUI PUBLIC ${HBUI_HEADERS})
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
    target_include_directories(HBUI PRIVATE ${HBUI_DIR}/../ImVK/include)
    target_include_directories(ImVk PUBLIC ${HBUI_DIR}/private/Generated)
endif ()


###############################################################################
# imgui
###############################################################################
target_link_libraries(HBUI PUBLIC imgui)
#
## Find the frameworks
#find_library(APPKIT_FRAMEWORK AppKit)
#find_library(FOUNDATION_FRAMEWORK Foundation)
#
#target_link_libraries(HBUI PUBLIC ${APPKIT_FRAMEWORK})
#target_link_libraries(HBUI PUBLIC ${FOUNDATION_FRAMEWORK})


find_library(APPKIT_LIBRARY AppKit)
if (NOT APPKIT_LIBRARY)
    message(STATUS "AppKit.framework NOT found!")
else()
    message(STATUS "AppKit.framework found! ${APPKIT_LIBRARY}")
endif()

find_library(FOUNDATION_LIBRARY Foundation)
if (NOT FOUNDATION_LIBRARY)
    message(STATUS "Foundation.framework NOT found!")
else()
    message(STATUS "Foundation.framework found! ${FOUNDATION_LIBRARY}")
endif()

target_link_libraries(ImVk PRIVATE ${FOUNDATION_LIBRARY})