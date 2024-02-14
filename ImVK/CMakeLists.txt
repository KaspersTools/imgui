cmake_minimum_required(VERSION 3.13)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif ()

project(IMVK C CXX)

cmake_policy(SET CMP0077 NEW)
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
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

# Set the default build type to 'Debug'
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif ()

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
option(IMVK_DOWNLOAD_GLFW "Download GLFW" OFF)
if (NOT TARGET glfw)
    #-Wunused-parameter]
    #disable warnings for glfw
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")

    set(GLFW_DIR ../../glfw) # Set this to point to an up-to-date GLFW repo

    if(NOT EXISTS ../../glfw/CMakeLists.txt)
        if(NOT IMVK_DOWNLOAD_GLFW)
            message(FATAL_ERROR "The GLFW submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
        else()
            message(WARNING "The GLFW submodule was not found. Downloading GLFW from github.")
            #downlaod from github
            execute_process(COMMAND git clone https://github.com/KaspersTools/glfw.git ../../glfw
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        endif()
    endif()

    option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
    option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
    option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
    option(GLFW_INSTALL "Generate installation target" OFF)
    option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
    add_subdirectory(${GLFW_DIR} binary_dir)
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