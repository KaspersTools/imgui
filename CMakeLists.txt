cmake_minimum_required(VERSION 3.26)
project(imgui)

set(CMAKE_CXX_STANDARD 23)

option(KDB_IMGUI_BUILD_EXAMPLES "Build examples" ON)

#display info

#glfw
set(GLFW_DIR vendor/glfw)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
set(KDB_IMGUI_INCLUDE_DIR ./)
 add_subdirectory(vendor/glfw)

#vulkan
find_package(Vulkan REQUIRED)

#imgui
set(IMGUI_SOURCES
        #imgui
        ${KDB_IMGUI_INCLUDE_DIR}imgui.cpp
        ${KDB_IMGUI_INCLUDE_DIR}imgui.h
        ${KDB_IMGUI_INCLUDE_DIR}imconfig.h

        #imgui widgets
        ${KDB_IMGUI_INCLUDE_DIR}imgui_draw.cpp
        ${KDB_IMGUI_INCLUDE_DIR}imgui_internal.h
        ${KDB_IMGUI_INCLUDE_DIR}imgui_tables.cpp
        ${KDB_IMGUI_INCLUDE_DIR}imgui_widgets.cpp

        #imstb
        ${KDB_IMGUI_INCLUDE_DIR}imstb_textedit.h
        ${KDB_IMGUI_INCLUDE_DIR}imstb_rectpack.h
        ${KDB_IMGUI_INCLUDE_DIR}imstb_textedit.h

        #imgui demos
        ${KDB_IMGUI_INCLUDE_DIR}imgui_demo.cpp

        #backends
        ${KDB_IMGUI_INCLUDE_DIR}backends/imgui_impl_glfw.cpp
        ${KDB_IMGUI_INCLUDE_DIR}backends/imgui_impl_glfw.h

        ${KDB_IMGUI_INCLUDE_DIR}backends/imgui_impl_vulkan.cpp
        ${KDB_IMGUI_INCLUDE_DIR}backends/imgui_impl_vulkan.h

        ${KDB_IMGUI_INCLUDE_DIR}backends/imgui_impl_glfw_vulkan_window.cpp
        ${KDB_IMGUI_INCLUDE_DIR}backends/imgui_impl_glfw_vulkan_window.h

        ${KDB_IMGUI_INCLUDE_DIR}backends/imgui_impl_vulkan_image.cpp
        ${KDB_IMGUI_INCLUDE_DIR}backends/imgui_impl_vulkan_image.h

        #misc
        ${KDB_IMGUI_INCLUDE_DIR}misc/cpp/imgui_stdlib.cpp
        ${KDB_IMGUI_INCLUDE_DIR}misc/cpp/imgui_stdlib.h

        #fonts
        ${KDB_IMGUI_INCLUDE_DIR}addons/imguinotify/font_awesome_5.h
        ${KDB_IMGUI_INCLUDE_DIR}addons/imguinotify/fa_solid_900.h

        #misc
        ${KDB_IMGUI_INCLUDE_DIR}misc/stb_image/stb_image.h
        ${KDB_IMGUI_INCLUDE_DIR}backends/debug/imgui_impl_glfw_vulkan_debug.cpp
        ${KDB_IMGUI_INCLUDE_DIR}backends/debug/imgui_impl_glfw_vulkan_debug.h

        Extension.cpp
        Extension.h
        fonts/FontManager.cpp
        fonts/FontManager.h
)
add_library(KDB_IMGUI-STATIC STATIC
        ${IMGUI_SOURCES}
)

message(STATUS "KDB_IMGUI_INCLUDE_DIR: ${KDB_IMGUI_INCLUDE_DIR}")
message(STATUS "GLFW_DIR: ${GLFW_DIR}")
message(STATUS "Vulkan_INCLUDE_DIRS: ${Vulkan_INCLUDE_DIRS}")

list(APPEND  ${KDB_IMGUI_INCLUDE_DIR} {Vulkan_INCLUDE_DIRS})

target_include_directories(KDB_IMGUI-STATIC PUBLIC
        ${KDB_IMGUI_INCLUDE_DIR}
        ${Vulkan_INCLUDE_DIRS}
)

target_include_directories(KDB_IMGUI-STATIC PRIVATE
        ${GLFW_DIR}/include
)
target_link_libraries(KDB_IMGUI-STATIC PRIVATE
        glfw
        Vulkan::Vulkan
)


if (KDB_IMGUI_BUILD_EXAMPLES)
    set(IMGUI_EXAMPLES_SOURCES example/main.cpp example/MainWindow.cpp example/MainWindow.h)
    add_executable(KDB_IMGUI-EXAMPLE ${IMGUI_EXAMPLES_SOURCES})
    target_link_libraries(KDB_IMGUI-EXAMPLE PRIVATE KDB_IMGUI-STATIC)

    #copy assets
    file(COPY example/Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    target_include_directories(KDB_IMGUI-EXAMPLE PRIVATE ${KDB_IMGUI_INCLUDE_DIR})
endif ()

#compile definitions
target_compile_definitions(KDB_IMGUI-STATIC PUBLIC -DImTextureID=ImU64)
