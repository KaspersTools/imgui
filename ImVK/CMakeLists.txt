project(ImVk VERSION 0.1.0 LANGUAGES CXX C OBJC OBJCXX)

cmake_minimum_required(VERSION ${CMAKE_VERSION})
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif ()

include(../cmake/defaultsetup.cmake)

# Make list of include directories
list(APPEND ImVk_PUBLIC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND ImVk_PRIVATE_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/../imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/private
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

###############################################################################
# Dependencies
###############################################################################
#glfw
if (NOT DEFINED IMVK_DOWNLOAD_GLFW)
    option(IMVK_DOWNLOAD_GLFW "Download GLFW" OFF)
endif ()

if (NOT TARGET glfw)
    #-Wunused-parameter]
    #disable warnings for glfw
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
    set(GLFW_DIR ../../glfw) # Set this to point to an up-to-date GLFW repo

    if (NOT EXISTS ../../glfw/CMakeLists.txt)
        if (NOT IMVK_DOWNLOAD_GLFW)
            message(FATAL_ERROR "The GLFW submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
        else ()
            message(WARNING "The GLFW submodule was not found. Downloading GLFW from github.")
            #downlaod from github
            execute_process(COMMAND git clone https://github.com/KaspersTools/glfw.git ../../glfw
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        endif ()
    endif ()

    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_DOCUMENT_INTERNALS OFF)
    set(GLFW_STANDALONE OFF)
    set(GLFW_INSTALL OFF)
    set(GLFW_BUILD_DOCS OFF)

    #disable all warnings for glfw
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-parameter")

    add_subdirectory(${GLFW_DIR} ${CMAKE_CURRENT_BINARY_DIR}/glfw)
    list(APPEND ImVk_PRIVATE_INCLUDE_DIRS ${GLFW_DIR}/include)
endif ()

#Vulkan
find_package(Vulkan REQUIRED)
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
endif ()

set(ImVk_LIBS "glfw;Vulkan::Vulkan")

# Use vulkan headers from glfw:
list(APPEND ${ImVk_PRIVATE_INCLUDE_DIRS} ${GLFW_DIR}/deps)

###############################################################################
# ImVk
###############################################################################
set(ImVk_SOURCES
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_vulkan.cpp
        private/src/ImVk.cpp
        private/src/Image.cpp
)

set(ImVK_Headers
        include/ImVk/ImVk.h
        include/ImVk/Image.h
)

add_library(ImVk STATIC)
target_sources(ImVk PUBLIC
        ${ImVk_SOURCES}
)

target_sources(ImVk PUBLIC
        ${ImVK_Headers})

target_link_libraries(ImVk PUBLIC
        ${ImVk_LIBS}
)

target_include_directories(ImVk PUBLIC
        ${ImVk_PUBLIC_INCLUDE_DIRS}
        ${HBUI_DIR}/include/imgui
)

target_include_directories(ImVk PUBLIC
        ${ImVk_PRIVATE_INCLUDE_DIRS}
)
###############################################################################
# Compile definitions
###############################################################################
target_compile_definitions(ImVk PUBLIC -DImTextureID=ImU64)
target_compile_options(ImVk PUBLIC "-x" "objective-c++")
if (APPLE AND NOT IOS) # If mac
    target_link_libraries(ImVk PUBLIC "-framework AppKit -framework IOKit")
endif()
if (APPLE)
    target_link_libraries(ImVk PUBLIC "-framework Foundation")
endif()
target_compile_definitions(ImVk PUBLIC HELLOIMGUI_HAS_VULKAN)
#set(HBUI_HAS_VULKAN ON CACHE BOOL "" FORCE)
find_package(Vulkan REQUIRED)
target_link_libraries(ImVk PUBLIC Vulkan::Vulkan)

###############################################################################
# print infos
###############################################################################
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")


###############################################################################