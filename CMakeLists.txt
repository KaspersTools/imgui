cmake_minimum_required(VERSION 3.26)
project(imgui)

set(CMAKE_CXX_STANDARD 23)

#check if there is an cmake build type
if (NOT CMAKE_BUILD_TYPE)
    message("No build type selected, defaulting to Release")
    set(CMAKE_BUILD_TYPE Release)
endif ()

option(IMGUI_BUILD_EXAMPLES "Build examples" OFF)

#imgui
set(IMGUI_SOURCES
        #imgui
        imgui.cpp
        imgui.h
        imconfig.h

        #imgui widgets
        imgui_draw.cpp
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp

        #imstb
        imstb_textedit.h
        imstb_rectpack.h
        imstb_textedit.h

        #imgui demos
        imgui_demo.cpp

        #backends
        backends/imgui_impl_opengl3_loader.h
        backends/imgui_impl_opengl3.cpp
        backends/imgui_impl_opengl3.h
        backends/imgui_impl_sdl2.cpp
        backends/imgui_impl_sdl2.h


        #misc
        misc/cpp/imgui_stdlib.cpp
        misc/cpp/imgui_stdlib.h

        #fonts
        addons/imguinotify/font_awesome_5.h
        addons/imguinotify/fa_solid_900.h

        #addons
        addons/imguinotify/imgui_notify.h
        addons/imguitoolbar/imguitoolbar.h
)

add_library(imgui STATIC
        ${IMGUI_SOURCES}
)

#include imgui
include_directories(
        .
        backends
        misc/cpp
        addons/imguinotify
)

#sdl
find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    message("SDL2 found")
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(imgui ${SDL2_LIBRARIES})
endif ()


if(IMGUI_BUILD_EXAMPLES)
    set(IMGUI_EXAMPLE_SOURCE
            example/main.cpp
            example/MainWindow.cpp
            example/MainWindow.h
    )

    add_executable(imgui_example
            ${IMGUI_EXAMPLE_SOURCE}
    )

    target_link_libraries(imgui_example imgui)

    find_package(OpenGL REQUIRED)
    if(OPENGL_FOUND)
        message("OpenGL found")
        include_directories(${OPENGL_INCLUDE_DIRS})
        target_link_libraries(imgui_example ${OPENGL_LIBRARIES})
    endif ()
endif ()

#copy folder
file(COPY misc/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Assets)