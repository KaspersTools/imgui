//
// Created by Kasper de Bruin on 19/01/2024.
//
#include "MainWindow.h"
#include <HBUI/HBUI.h>
#include <HBUI/WindowManager.h>

void MainWindow::init() {
  HBUI::initialize("Hello, World!", 1280, 720, HBUI_MAIN_WINDOW_FLAG_DEFAULT_DOCKSPACE | HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR);
  run();
}

bool MainWindow::run() {
  while (!HBUI::wantToClose()) {
    render();
  }
  return true;
}

void MainWindow::render() {
  HBUI::startFrame();

  ImGui::Begin("Hello, window from template");
  ImGui::Text("Hello, world!");
  ImGui::Text("This is some useful text.");
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
