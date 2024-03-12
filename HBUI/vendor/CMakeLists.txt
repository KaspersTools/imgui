#   Vendor
# adds:
#   - magic_enum

cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(HBUI_VENDOR VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23 CACHE STRING "The C++ standard whose features are requested.")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "The C++ standard is required.")
set(CMAKE_CXX_EXTENSIONS ON CACHE BOOL "When C++ is enabled, this flag is passed to the compiler.")

#####SET OUTPUT DIRECTORIES AND SAVE FOR CACHE
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Single Directory for all static libraries.")

if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    set(OPTIONS -Wall -Wextra -pedantic-errors -Werror)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(OPTIONS /W4 /WX)
    if(HAS_PERMISSIVE_FLAG)
        set(OPTIONS ${OPTIONS} /permissive-)
    endif()
endif()
add_subdirectory(magic_enum)

add_library(HBUI_VENDOR INTERFACE)
target_link_libraries(HBUI_VENDOR INTERFACE magic_enum::magic_enum)
target_include_directories(HBUI_VENDOR INTERFACE vendor/magic_enum/include)

message(STATUS "HBUI_VENDOR: ${CMAKE_PROJECT_NAME}")
message(STATUS "HBUI_VENDOR: ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION}")
message(STATUS "HBUI_VENDOR: ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION} ${CMAKE_CXX_STANDARD}")
message(STATUS "HBUI_VENDOR: ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION} ${CMAKE_CXX_STANDARD} ${CMAKE_CXX_STANDARD_REQUIRED}")
message(STATUS "HBUI_VENDOR: HBUI_MAGIC_ENUM: " ${magic_enum})