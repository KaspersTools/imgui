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

        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_glfw.h
        backends/imgui_impl_vulkan.cpp
        backends/imgui_impl_vulkan.h


        #misc
        misc/cpp/imgui_stdlib.cpp
        misc/cpp/imgui_stdlib.h

        #fonts
        addons/imguinotify/font_awesome_5.h
        addons/imguinotify/fa_solid_900.h

        #addons
        addons/imguinotify/imgui_notify.h
        addons/imguitoolbar/imguitoolbar.h

        addons/imguidatechooser/imguidatechooser.cpp
        addons/imguidatechooser/imguidatechooser.h
)

add_library(KDB_IMGUI-STATIC STATIC
        ${IMGUI_SOURCES}
)

#glfw
set(GLFW_DIR vendor/glfw)
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
target_include_directories(KDB_IMGUI-STATIC PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

include_directories(KDB_IMGUI-STATIC PRIVATE ${GLFW_DIR}/include)
include_directories(KDB_IMGUI-STATIC PRIVATE ${GLFW_DIR}/deps)
include_directories(KDB_IMGUI-STATIC PRIVATE ${Vulkan_INCLUDE_DIRS})

#example
if(KDB_IMGUI_BUILD_EXAMPLES)
    set(IMGUI_EXAMPLE_SOURCE
            example/main.cpp
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

#copy folder
file(COPY misc/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Assets)