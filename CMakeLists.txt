cmake_minimum_required(VERSION 3.26)
project(imgui)

set(CMAKE_CXX_STANDARD 23)

#check if there is an cmake build type
if (NOT CMAKE_BUILD_TYPE)
    message("No build type selected, defaulting to Release")
    set(CMAKE_BUILD_TYPE Release)
endif ()

option(KDB_IMGUI_BUILD_EXAMPLES "Build examples" ON)


#display info
message("Project: ${PROJECT_NAME}")
message("Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message("Current binary dir: ${CMAKE_CURRENT_BINARY_DIR}"
        " (where the executable will be placed)"
        " (and where the assets will be copied to)"
        " (if you are using an IDE, this is where you should set the working directory)"
        )
message("Build type: ${CMAKE_BUILD_TYPE}")
message("Build examples: ${KDB_IMGUI_BUILD_EXAMPLES}")

set(IMGUI_DIR  ${CMAKE_CURRENT_SOURCE_DIR})

#imgui
set(IMGUI_SOURCES
        #imgui
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui.h
        ${IMGUI_DIR}/imconfig.h

        #imgui widgets
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_internal.h
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp

        #imstb
        ${IMGUI_DIR}/imstb_textedit.h
        ${IMGUI_DIR}/imstb_rectpack.h
        ${IMGUI_DIR}/imstb_textedit.h

        #imgui demos
        ${IMGUI_DIR}/imgui_demo.cpp

        #backends
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.h
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.h
        ${IMGUI_DIR}/backends/imgui_impl_glfw_vulkan_window.h
        ${IMGUI_DIR}/backends/imgui_impl_glfw_vulkan_window.cpp


        #misc
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.h

        #fonts
        ${IMGUI_DIR}/addons/imguinotify/font_awesome_5.h
        ${IMGUI_DIR}/addons/imguinotify/fa_solid_900.h

        #addons
        ${IMGUI_DIR}/addons/imguinotify/imgui_notify.h
        ${IMGUI_DIR}/addons/imguitoolbar/imguitoolbar.h

        ${IMGUI_DIR}/addons/imguidatechooser/imguidatechooser.cpp
        ${IMGUI_DIR}/addons/imguidatechooser/imguidatechooser.h
)

add_library(KDB_IMGUI-STATIC STATIC
        ${IMGUI_SOURCES}
)

#glfw
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)

#vulkan
find_package(Vulkan REQUIRED)

set(LIBRARIES "glfw;Vulkan::Vulkan")
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#link libraries
target_link_libraries(KDB_IMGUI-STATIC
        ${LIBRARIES}
)

set(IMGUI_DIR .)
target_include_directories(KDB_IMGUI-STATIC PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)

include_directories(KDB_IMGUI-STATIC PRIVATE ${GLFW_DIR}/include)
include_directories(KDB_IMGUI-STATIC PRIVATE ${GLFW_DIR}/deps)
include_directories(KDB_IMGUI-STATIC PRIVATE ${Vulkan_INCLUDE_DIRS})

#example
if(KDB_IMGUI_BUILD_EXAMPLES)
    set(IMGUI_EXAMPLE_SOURCE
            example/main.cpp
            example/MainWindow.cpp
            example/MainWindow.h
    )

    add_executable(imgui_example
            ${IMGUI_EXAMPLE_SOURCE}
    )

    target_link_libraries(imgui_example
            KDB_IMGUI-STATIC
            ${LIBRARIES}
    )
endif ()

#compile definitions
target_compile_definitions(KDB_IMGUI-STATIC PUBLIC -DImTextureID=ImU64)

#set debug, release, etc. flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug build")
    target_compile_definitions(KDB_IMGUI-STATIC PUBLIC -DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Release build")
    target_compile_definitions(KDB_IMGUI-STATIC PUBLIC -DNDEBUG)
endif()

#copy folder
file(COPY misc/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Assets)