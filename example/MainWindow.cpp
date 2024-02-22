//
// Created by Kasper de Bruin on 19/01/2024.
//
#include "MainWindow.h"
#include <HBUI/HBUI.h>

void MainWindow::init() {
  HBUI::initialize("Hello, World!", 1280, 720, HBMeinWindowFlags_NoTitleBar);

  //load font
  ImGuiIO &io = ImGui::GetIO();

  // Load the default font
  io.Fonts->AddFontDefault();

  // Load Font Awesome
//  static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
  {
    ImFontConfig icons_config = ImFontConfig();
    icons_config.MergeMode          = true;
    icons_config.PixelSnapH         = true;
    icons_config.RasterizerDensity  = 2.0f;
    icons_config.RasterizerMultiply = 1.0f;
    io.Fonts->AddFontFromFileTTF("assets/fonts/FontAwesome6Free/ttfs/fa-regular-400.ttf", 17.0f, &icons_config);
  }

  {
    ImFontConfig icons_config2 = ImFontConfig();
    icons_config2.MergeMode          = true;
    icons_config2.PixelSnapH         = true;
    icons_config2.RasterizerDensity  = 2.0f;
    icons_config2.RasterizerMultiply = 1.0f;
    io.Fonts->AddFontFromFileTTF("assets/fonts/FontAwesome6Free/ttfs/fa-solid-900.ttf", 17.0f, &icons_config2);
  }

  ImFontConfig fontConfig = ImFontConfig();
  fontConfig.RasterizerDensity  = 2.0f;
  fontConfig.RasterizerMultiply = 1.0f;
  io.FontDefault                = io.Fonts->AddFontFromFileTTF(
      "assets/fonts/JetBrainsMono/JetBrainsMonoNerdFont-Regular.ttf", 17.0f, &fontConfig);

  run();
}

bool MainWindow::run() {
  while (!HBUI::wantToClose()) {
    render();
  }
  return true;
}

void MainWindow::createMainMenuBars() {
}

void MainWindow::createDockSpace() {
}

void MainWindow::render() {
  HBUI::startFrame();

  if(HBUI::beginSideBar(ImGui::GetID("SideBar"),
                        HBSideBarFlags_OverviewPort | HBSideBarFlags_Horizontal,
                        {150, 150}, {150, 500})) {
    HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0,0}, {32,32});
    HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0,0}, {32,32});
    HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0,0}, {32,32});
    HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0,0}, {32,32});
    HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0,0}, {32,32});
    HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0,0}, {32,32});
    HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0,0}, {32,32});
    HBUI::endSideBar();
  }

  if(HBUI::beginSideBar(ImGui::GetID("SideBar"), HBSideBarFlags_OverviewPort | HBSideBarFlags_Horizontal,
                        {150, 150}, {150, 500})) {
    HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0,0}, {32,32});
    HBUI::endSideBar();
  }

  if(HBUI::beginSideBar(ImGui::GetID("SideBar"), HBSideBarFlags_OverviewPort | HBSideBarFlags_Horizontal,
                        {150, 150}, {150, 500})) {
    HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0,0}, {32,32});
    HBUI::endSideBar();
  }

  if(showDemoWindow) {
    ImGui::ShowDemoWindow(&showDemoWindow);
  }

#ifdef HBUI_WITH_DEBUG_WINDOW
  HBUI::showDebugWindow(&showDebugWindow);
#endif

  HBUI::endFrame();
}

void MainWindow::shutdown() {
  HBUI::shutdown();
}
