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


  //fixme fix this with the right fonts and headers.
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

void MainWindow::render() {
  HBUI::startFrame();

  static bool drawMultiRowSideBar = false;
  static bool drawIconSideBar     = true;
  //sidebars
  {
    if (drawMultiRowSideBar) {
      //Top Full screen "Side Bar"
      if (HBUI::beginSideBar(         //begin the side bar
          ImGui::GetID("SideBar"),    //id
          HBSideBarFlags_Vertical |   //flags -> horizontal = direction
          HBSideBarFlags_FullSize,    //full size = take the full screen width/height from the satart pos
          {0, 0},                     //start position
          {0,
           0}                      //size -> 0,0 means in combination with the flags it will take the full screen width, and the height will be calculated from the content
      )) {
        HBUI::sideBarBarButton( //add a button to the side bar
            ImGui::GetID("BTN1"), "BTN1",//id
            {0, 0},   //start position 0,0 for auto positioning //fixme: padding
            {32, 32}  //size
        );

        HBUI::sideBarBarButton( //add a button to the side bar
            ImGui::GetID("BTN(2)"), "BTN(2)", //id
            {0, 0},   //start position 0,0 for auto positioning
            {32, 64}  //size
        );

        HBUI::sideBarBarButton(ImGui::GetID("BTN3"), "BTN3", {0, 0}, {32, 32});
        HBUI::sideBarBarButton(ImGui::GetID("BTN4"), "BTN4", {0, 0}, {32, 32});
        HBUI::sideBarBarButton(ImGui::GetID("BTN5"), "BTN5", {0, 0}, {32, 32});
        HBUI::sideBarBarButton(ImGui::GetID("BTN6"), "BTN6", {0, 0}, {32, 32});
        HBUI::newLine();
        HBUI::sideBarBarButton(ImGui::GetID("BTN7"), "BTN7", {0, 0}, {32, 32});
        HBUI::sideBarBarButton(ImGui::GetID("BTN8"), "BTN8", {0, 0}, {32, 32});
        HBUI::newLine();
        HBUI::sideBarBarButton(ImGui::GetID("BTN9"), "BTN9", {0, 0}, {32, 32});
        HBUI::sideBarBarButton(ImGui::GetID("BT10"), "BT10", {0, 0}, {32, 32});
        HBUI::sideBarBarButton(ImGui::GetID("BT11"), "BT11", {0, 0}, {32, 32});
        HBUI::endSideBar(); //end the side bar

        //|~~~~~RESULT       ~~~~~~~~|
        //|--------------------------|
        //|[BTN1]    [BTN7]  [BTN9]  |
        //|[BTN(2)]  [BTN8]  [BT10]  |
        //|[BTN(2)]          [BT11]  |
        //|[BTN3]                    |
        //|[BTN4]                    |
        //|[BTN5]                    |
        //|[BTN6]                    |
        //|                          |
        //|                          |
        //|                          |
        //|                          |
        //|                          |
        //|                          |
        //|~~~~~~~~~~~~~~~~~~~~~~~~~~|
      }
    }
    if (drawIconSideBar) {
      if (HBUI::beginSideBar(         //begin the side bar
          ImGui::GetID("SideBar"),    //id
          HBSideBarFlags_Vertical |   //flags -> horizontal = direction
          HBSideBarFlags_FullSize,    //full size = take the full screen width/height from the satart pos
          {0, 0},                     //start position
          {0,
           0}                      //size -> 0,0 means in combination with the flags it will take the full screen width, and the height will be calculated from the content
      )) {
        HBUI::sideBarBarButton( //add a button to the side bar
            ImGui::GetID("BTN1"), "BTN1",//id
            {0, 0},   //start position 0,0 for auto positioning //fixme: padding
            {64, 64}  //size
        );

        HBUI::sideBarBarButton(ImGui::GetID("BTN2"), "BTN2", {0, 0}, {64, 64});
        HBUI::sideBarBarButton(ImGui::GetID("BTN3"), "BTN3", {0, 0}, {64, 64});
        HBUI::sideBarBarButton(ImGui::GetID("BTN4"), "BTN4", {0, 0}, {64, 64});
        HBUI::sideBarBarButton(ImGui::GetID("BTN5"), "BTN5", {0, 0}, {64, 64});
        HBUI::sideBarBarButton(ImGui::GetID("BTN6"), "BTN6", {0, 0}, {64, 64});

        HBUI::endSideBar();
      }
    }
    if (showDemoWindow) {
      ImGui::ShowDemoWindow(&showDemoWindow);
    }

#ifdef HBUI_WITH_DEBUG_WINDOW
    //only call this function at the end of a frame.
    HBUI::showDebugWindow(&showDebugWindow);
#endif

    HBUI::endFrame();
  }
}

void MainWindow::shutdown() {
  HBUI::shutdown();
}


