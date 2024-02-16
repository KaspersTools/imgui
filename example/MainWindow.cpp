//
// Created by Kasper de Bruin on 19/01/2024.
//
#include "MainWindow.h"
#include <HBUI/HBUI.h>
#include <HBUI/WindowManager.h>

void MainWindow::init() {
  HBUI::initialize("Hello, World!", 1280, 720, HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR);
  run();
}

bool MainWindow::run() {
  while (!HBUI::wantToClose()) {
    render();
  }
  return true;
}

void renderMenuBar(HBMainMenuBarFlags_ flags) {
  if (HBUI::beginMainMenuBar(flags)) {
    HBUI::endMainMenuBar();
  }
}
void MainWindow::createMainMenuBars() {
  if (firstHorizontal) {
    if (horizontalMenuBar) {
      renderMenuBar(HB_MAIN_MENU_BAR_FLAG_HORIZONTAL);
      if (verticalMenuBar) {
        renderMenuBar(HB_MAIN_MENU_BAR_FLAG_VERTICAL);
      }
    }
  } else {
    if (verticalMenuBar) {
      renderMenuBar(HB_MAIN_MENU_BAR_FLAG_VERTICAL);
    }
    if (horizontalMenuBar) {
      renderMenuBar(HB_MAIN_MENU_BAR_FLAG_HORIZONTAL);
    }
  }
}
void MainWindow::createDockSpace() {
  if (HBUI::beginFullScreenDockspace(HB_DOCKSPACE_FLAG_NONE)) {
  }
}

void MainWindow::render() {
  HBUI::startFrame();


  if (withMenuBar) { createMainMenuBars(); }
  if (withDockSpace) { createDockSpace(); }

  ImGui::ShowDemoWindow(&showDemoWindow);

  ImGui::Begin("HBUI Demo Window                          ");

  ImGui::Text("HBUI -- (%f) -- and dear imgui says hello! (%s) (%d) ", HBUI_VERSION, IMGUI_VERSION, IMGUI_VERSION_NUM);
  ImGui::Spacing();
  if (ImGui::CollapsingHeader("Help")) {
    ImGui::SeparatorText("ABOUT THIS DEMO:");
    ImGui::BulletText("This HBUI Demo Window is a simple example of how to use HBUI.");
    ImGui::BulletText("All the UI elements are available and shown in this window.");

    ImGui::SeparatorText("ABOUT THE LIBRARIES:");
    ImGui::BulletText("The code for this window is located in the example/MainWindow.cpp file.                                    <---- DEMO IS HERE!!!           ");
    ImGui::BulletText("The code for the HBUI library  is located in the HBUI/include/HBUI folder.                                 <---- HBUI IS HERE!!!");
    ImGui::BulletText("The code for the ImGui library is located in the root directory, this will be moved to the include folder. <-- IMVK IS HERE!!!");
    ImGui::BulletText("The code for the ImVk library is located in the ImVK folder. This handles the backend (for now only Vulkan in combination with glfw   <---- IMVK IS HERE!!!");
  }
  if (ImGui::CollapsingHeader("Example Options")) {
    ImGui::SeparatorText("Windows             ");
    ImGui::Checkbox("Show Debug Window    |   ", &showDebugWindow);
    ImGui::SameLine();
    ImGui::Checkbox("Show Demo Window     |   ", &showDemoWindow);
    ImGui::SeparatorText("Docking             ");
    ImGui::Checkbox("With Dock Space      |   ", &withDockSpace);
    ImGui::SameLine();
    ImGui::Text("Renderered At: f , f ");//HBUI::getDrawData().
    ImGui::SeparatorText("Menu Bars           ");
    ImGui::Checkbox("With Menu Bar        |   ", &withMenuBar);
    ImGui::Checkbox("Vertical Menu Bar    |   ", &verticalMenuBar);
    if (HBUI::getDrawData().mainMenuBarVertical != nullptr) {
      ImGui::SameLine();
      ImGui::Text("Renderered At: %f , %f", HBUI::getDrawData().mainMenuBarVertical->start.x, HBUI::getDrawData().mainMenuBarVertical->end.y);
    }
    ImGui::Checkbox("Horizontal Menu Bar  |   ", &horizontalMenuBar);
    if (HBUI::getDrawData().mainMenuBarHorizontal != nullptr) {
      ImGui::SameLine();
      ImGui::Text("Renderered At: %f , %f", HBUI::getDrawData().mainMenuBarHorizontal->start.x, HBUI::getDrawData().mainMenuBarHorizontal->end.y);
    }
    ImGui::Checkbox("Horizontal First     |   ", &firstHorizontal);
    ImGui::SameLine();
    ImGui::Text("Render the horizontal before the vertical one so you can check the allignment");
  }

  ImGui::End();


#ifdef HBUI_WITH_DEBUG_WINDOW
  HBUI::showDebugWindow(&showDebugWindow);
#endif

  HummingBirdCore::UI::WindowManager::getInstance()->render();

  HBUI::endFrame();
}

void MainWindow::shutdown() {
  HBUI::shutdown();
}
