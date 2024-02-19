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

void renderMenuBar(const std::string &id,
                   HBItemFlags itemFlags, MainMenuBarFlags flags,
                   ImVec2 pos, ImVec2 size) {
}

void MainWindow::createMainMenuBars() {
//  renderMenuBar("vertical", HBItemFlags_None, HB_MAIN_MENU_BAR_FLAG_VERTICAL, {}, {});
//  renderMenuBar("horizontal", HBItemFlags_None, HB_MAIN_MENU_BAR_FLAG_HORIZONTAL, {}, {});
//  renderMenuBar("somewherehorizontal", HBItemFlags_None, HB_MAIN_MENU_BAR_FLAG_HORIZONTAL, {}, {});

//  const std::string &id,
//  HBItemFlags itemFlags, MainMenuBarFlags flags,
//      Vec2Anim animProps, ImVec2 windowSize;
  bool looping = false;
  HBAnimProps<ImVec2> animProps = {
      .start          = {0, 0},
      .end            = {0, 500},
      .current        = {0, 0},
      .duration       = 10.f,
      .playbackSpeed  = 1.f,
      .looping        = false,
      .type           = HB_AnimType_Linear,
      .direction      = HB_AnimDirection_Forward,
      .state          = HB_AnimState_Idle,
      .effect         = HB_AnimEffect_Expand,
  };

  if (HBUI::beginMainMenuBar("mainMenuBar", animProps, HBItemFlags_None,
                             HB_MAIN_MENU_BAR_FLAG_HORIZONTAL | HB_Main_Menu_Bar_Flag_Animated)) {
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

void MainWindow::createDockSpace() {
  if (HBUI::beginFullScreenDockspace(HB_DOCKSPACE_FLAG_NONE)) {
  }
}

void MainWindow::render() {
  HBUI::startFrame();


  if (withMenuBar) { createMainMenuBars(); }
  if (withDockSpace) { createDockSpace(); }

  ImGui::ShowDemoWindow(&showDemoWindow);
  ImGui::Begin("Fonts");
//  ImGui::Text  (ICON_FA_0  " Home");  // Example of using a home icon
//  ImGui::Text  (ICON_FA_AWARD  " Home");  // Example of using a home icon
//  ImGui::Button(ICON_FA_QRCODE  " Play");  // Example of using a play icon in a button
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
