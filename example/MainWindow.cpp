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

void MainWindow::createDockSpace() {
  if (withMenuBar) {

    HBUI::beginFullScreenDockspace(HB_DOCKSPACE_FLAG_MENUBAR);

    if(horizontalMenuBar) {
      if(HBUI::beginMainMenuBar(HB_MAIN_MENU_BAR_FLAG_HORIZONTAL)){
        if (HBUI::beginMainMenuItem("HORIZONTAL MENU BAR")) {
          HBUI::EndMainMenuItem();
        }
        HBUI::endMainMenuBar();
      }
    }
    if(verticalMenuBar) {
      if(HBUI::beginMainMenuBar(HB_MAIN_MENU_BAR_FLAG_VERTICAL)){
        if (HBUI::beginMainMenuItem("VERTICAL MENU BAR")) {
          HBUI::EndMainMenuItem();
        }
        HBUI::endMainMenuBar();
      }
    }
    HBUI::endFullScreenDockspace();//end of dockspace

  } else {
    HBUI::beginFullScreenDockspace(HB_DOCKSPACE_FLAG_NONE);//begin of dockspace
    HBUI::endFullScreenDockspace();                        //end of dockspace
  }
}


void MainWindow::render() {
  HBUI::startFrame();

  if (withDockSpace) {
    createDockSpace();
  }

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

  if (ImGui::CollapsingHeader("Dockspaces")) {
    if (ImGui::Checkbox("With Dockspace     |", &withDockSpace)) {}
    if (ImGui::Checkbox("With MenuBar       |", &withMenuBar)) {
      if(!withMenuBar){
          horizontalMenuBar = false;
          verticalMenuBar = false;
        }
    }
    if (ImGui::Checkbox("Horizontal MenuBar |", &horizontalMenuBar)) {

    }
    if (ImGui::Checkbox("Vertical MenuBar   |", &verticalMenuBar)) {
      if (verticalMenuBar) {

      }
    }
  }

  ImGui::End();

#ifdef HBUI_WITH_DEBUG_WINDOW
  HBUI::showDebugWindow();
#endif

  HummingBirdCore::UI::WindowManager::getInstance()->render();

  HBUI::endFrame();
}

void MainWindow::shutdown() {
  HBUI::shutdown();
}
