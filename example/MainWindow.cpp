//
// Created by Kasper de Bruin on 19/01/2024.
//

#include "MainWindow.h"
#include <imgui_impl_glfw_vulkan_window.h>

void MainWindow::init() {
  ImGui::GLFWVULKANIMPL::ApplicationSpecification m_Specification;
  m_Specification.Name = "Imgui Vulkan Example";
  m_Specification.Width = 800;
  m_Specification.Height = 600;

  m_Specification.WindowResizeable = true;
  m_Specification.WindowDecorated = true;
  m_Specification.CenterWindow = true;
  m_Specification.CustomTitlebar = true;

  ImGui::GLFWVULKANIMPL::init(m_Specification);
  run();
}

bool MainWindow::run() {
  while (!glfwWindowShouldClose(ImGui::GLFWVULKANIMPL::getMainWindowHandle())) {
    render();
  }
}

void MainWindow::render() {
  ImGui::GLFWVULKANIMPL::startRender();
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        glfwSetWindowShouldClose(ImGui::GLFWVULKANIMPL::getMainWindowHandle(), true);
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools")) {
      if (ImGui::MenuItem("Show Demo Window")) {
        m_demoWindow = true;
      }
      if (ImGui::MenuItem("Show Render Stats")) {
        m_showRenderStats = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  if (m_demoWindow) {
    ImGui::ShowDemoWindow(&m_demoWindow);
  }

  if (m_showRenderStats) {
    ImGui::GLFWVULKANIMPL::getRenderStats().showRenderStats(&m_showRenderStats);
  }


  ImGui::GLFWVULKANIMPL::endRender();
}

void MainWindow::shutdown() {
  ImGui::GLFWVULKANIMPL::shutdown();
}
