//
// Created by Kasper de Bruin on 19/01/2024.
//
#include "MainWindow.h"
#include <HBUI/HBUI.h>
#include <HBUI/WindowManager.h>
#include <IconsFontAwesome6.h>

void MainWindow::init() {
  HBUI::initialize("Hello, World!", 1280, 720, HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR);

  //load font
  ImGuiIO& io = ImGui::GetIO();

  // Load the default font
  io.Fonts->AddFontDefault();

  // Load Font Awesome
//  static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
  {
    ImFontConfig icons_config = ImFontConfig();
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.RasterizerDensity = 2.0f;
    icons_config.RasterizerMultiply = 1.0f;
    io.Fonts->AddFontFromFileTTF("assets/fonts/FontAwesome6Free/ttfs/fa-regular-400.ttf", 15.0f, &icons_config);
  }

  ImFontConfig fontConfig = ImFontConfig();
  fontConfig.RasterizerDensity = 2.0f;
  fontConfig.RasterizerMultiply = 1.0f;
  io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/JetBrainsMono/JetBrainsMonoNerdFont-Regular.ttf", 15.0f, &fontConfig);

  run();
}

bool MainWindow::run() {
  while (!HBUI::wantToClose()) {
    render();
  }
  return true;
}

void renderMenuBar(const std::string &id, MainMenuBarFlags flags, float width, float height) {
  if (HBUI::beginMainMenuBar(id, flags, {width, height})) {
    if (HBUI::menuBarButton("test2")) {}
    if (HBUI::menuBarButton("test1")) {}
    if (HBUI::menuBarButton("test1")) {}
    if (HBUI::menuBarButton("test1")) {}
    if (HBUI::menuBarButton("test1")) {}
    if (HBUI::menuBarButton("test1")) {}
    if (HBUI::menuBarButton("test1")) {}
    if (HBUI::menuBarButton("test1")) {}
    if (HBUI::menuBarButton("test1")) {}
    if (HBUI::menuBarButton("test1")) {}
    if (HBUI::menuBarButton("test1")) {}
    if (HBUI::menuBarButton("test1")) {}
    HBUI::endMainMenuBar();
  }
}

void MainWindow::createMainMenuBars() {

  renderMenuBar("kasper", HB_MAIN_MENU_BAR_FLAG_HORIZONTAL,  0.0f, horizontalMenuBarFixedHeight);
  renderMenuBar("kasper2", HB_MAIN_MENU_BAR_FLAG_HORIZONTAL, 0.0f, horizontalMenuBarFixedHeight);
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
  ImGui::Text("HBUI -- (%f) -- and dear imgui says hello! (%s) (%d) ", HBUI_VERSION, IMGUI_VERSION,
              IMGUI_VERSION_NUM);
  ImGui::Spacing();
  if (ImGui::CollapsingHeader("Help")) {
    ImGui::SeparatorText("ABOUT THIS DEMO:");
    ImGui::BulletText("This HBUI Demo Window is a simple example of how to use HBUI.");
    ImGui::BulletText("All the UI elements are available and shown in this window.");

    ImGui::SeparatorText("ABOUT THE LIBRARIES:");
    ImGui::BulletText(
        "The code for this window is located in the example/MainWindow.cpp file.                                    <---- DEMO IS HERE!!!           ");
    ImGui::BulletText(
        "The code for the HBUI library  is located in the HBUI/include/HBUI folder.                                 <---- HBUI IS HERE!!!");
    ImGui::BulletText(
        "The code for the ImGui library is located in the root directory, this will be moved to the include folder. <-- IMVK IS HERE!!!");
    ImGui::BulletText(
        "The code for the ImVk library is located in the ImVK folder. This handles the backend (for now only Vulkan in combination with glfw   <---- IMVK IS HERE!!!");
  }
  if (ImGui::CollapsingHeader("Example Options")) {
    ImGui::SeparatorText("Windows");
    ImGui::Checkbox("Show Debug Window     |   ", &showDebugWindow);
    ImGui::Checkbox("Show Demo Window      |   ", &showDemoWindow);
    ImGui::Checkbox("With Dock Space       |   ", &withDockSpace);
    ImGui::Separator();

    ImGui::SeparatorText("Menu Bars");

    ImGui::Checkbox("With Menu  Bar          |   ", &withMenuBar);
    ImGui::Checkbox("Vertical   Menu Bar     |   ", &verticalMenuBar);
    ImGui::DragFloat("Vertical Menu Bar Width |   ", &verticalMenuBarFixedWidth, 1.0f, 0.0f, 1000.0f);
    ImGui::Separator();
    ImGui::Checkbox("Horizontal Menu Bar   |   ", &horizontalMenuBar);
    ImGui::DragFloat("Horizontal Menu Bar Height", &horizontalMenuBarFixedHeight, 1.0f, 0.0f, 1000.0f);
  }
  ImGui::End();

  ImGui::Begin("Fonts");
  ImGui::Text(ICON_FA_H " Home");  // Example of using a home icon
  ImGui::Button(ICON_FA_PLAY " Play");  // Example of using a play icon in a button
  ImGui::End();

  ImGui::Begin("My Window");
  ImGui::Text(ICON_FA_HOUSE " Home");
  ImGui::Button(ICON_FA_PLAY " Play");
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
