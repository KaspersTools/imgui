//
// Created by Kasper de Bruin on 19/01/2024.
//

#include "MainWindow.h"
#include <imgui_impl_glfw_vulkan_window.h>

using namespace ImGui::GLFWVULKANIMPL;

void MainWindow::init() {

  ImGui::GLFWVULKANIMPL::ApplicationSpecification m_Specification;
  m_Specification.Name = "Imgui Vulkan Example";

  m_Specification.WindowSettings.Width = 800;
  m_Specification.WindowSettings.Height = 600;
  m_Specification.WindowSettings.WindowResizeable = true;
  m_Specification.WindowSettings.WindowDecorated = true;
  m_Specification.WindowSettings.CenterWindow = true;

  m_Specification.TitleBarSettings.WindowNoDefaultTitleBar = true;

  ImGui::GLFWVULKANIMPL::setImplErrorCallback([](int error, const char *description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
  });

  ImGui::GLFWVULKANIMPL::init(m_Specification);

  //log
  std::cout << "----------Application Specification---------------------------------------------------------------------" << std::endl;
  std::cout << "GLFWVULKANIMPL::init() finished" << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().Name: " << getApplicationSpecification().Name << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().IconPath: " << getApplicationSpecification().IconPath << std::endl;
  std::cout << "----------Window Specification--------------------------------------------------------------------------" << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().Width: " << getWindowSettings().Width << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().Height: " << getWindowSettings().Height << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().WindowResizeable: " << getWindowSettings().WindowResizeable << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().WindowDecorated: " << getWindowSettings().WindowDecorated << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().CenterWindow: " << getWindowSettings().CenterWindow << std::endl;
  std::cout << "---------Title bar Specification------------------------------------------------------------------------" << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.WindowNoDefaultTitleBar: " << getApplicationSpecification().TitleBarSettings.WindowNoDefaultTitleBar << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarHeight: " << getApplicationSpecification().TitleBarSettings.TitleBarHeight << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarPadding: " << getApplicationSpecification().TitleBarSettings.TitleBarPadding << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarIconSize: " << getApplicationSpecification().TitleBarSettings.TitleBarIconSize << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarIconPadding: " << getApplicationSpecification().TitleBarSettings.TitleBarIconPadding << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarOffsetX: " << getApplicationSpecification().TitleBarSettings.TitleBarOffsetX << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarOffsetY: " << getApplicationSpecification().TitleBarSettings.TitleBarOffsetY << std::endl;
  std::cout << "--------------------------------------------------------------------------------------------------------" << std::endl;
  run();
}

bool MainWindow::run() {
  while (!glfwWindowShouldClose(ImGui::GLFWVULKANIMPL::getMainWindowHandle())) {
    render();
  }
}

void MainWindow::render() {
  ImGui::GLFWVULKANIMPL::startRender();

  ImGui::Begin("Titlebar settings");
  ImGui::SliderFloat("Titlebar height", &ImGui::GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarHeight, 0.0f, 100.0f);
  ImGui::SliderFloat("Titlebar padding", &ImGui::GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarPadding, 0.0f, 100.0f);
  ImGui::SliderFloat("Titlebar icon size", &ImGui::GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarIconSize, 0.0f, 100.0f);
  ImGui::SliderFloat("Titlebar icon padding", &ImGui::GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarIconPadding, 0.0f, 100.0f);
  ImGui::SliderFloat("Titlebar offset X", &ImGui::GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarOffsetX, 0.0f, 100.0f);
  ImGui::SliderFloat("Titlebar offset Y", &ImGui::GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.TitleBarOffsetY, 0.0f, 100.0f);
  ImGui::End();

  if (ImGui::BeginMainMenuBar()) {
    //Offset
    if (getTitleBarSpecification().WindowNoDefaultTitleBar && !isWindowMaximized()) {
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + getApplicationSpecification().TitleBarSettings.TitleBarOffsetX);
    }

    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        glfwSetWindowShouldClose(getMainWindowHandle(), true);
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
