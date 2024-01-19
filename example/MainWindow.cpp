//
// Created by Kasper de Bruin on 19/01/2024.
//

#include "MainWindow.h"
#include <imgui_impl_glfw_vulkan_window.h>

extern GLFWwindow* g_GlfwWindow;

bool MainWindow::run() {
    ImGuiConfigFlags imGuiConfigFlags = ImGuiConfigFlags_None;
    imGuiConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    imGuiConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    imGuiConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    generateWindow(imGuiConfigFlags);

    // Main loop
    while (!glfwWindowShouldClose(g_GlfwWindow)) {
        startRender();
        ImGui::ShowDemoWindow();
        endRender();
    }

    cleanupWindow();
    return 0;
}

void MainWindow::init() {

}

void MainWindow::render() {

}
