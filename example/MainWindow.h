//
// Created by Kasper de Bruin on 19/01/2024.
//

#ifndef IMGUI_MAINWINDOW_H
#define IMGUI_MAINWINDOW_H

#include <HBUI/HBUI.h>

class MainWindow {
  public:
  MainWindow() {
    init();
  }

  ~MainWindow() {
    shutdown();
  }

  bool run();

  private:
  void init();

  void createMainMenuBars();

  void createDockSpace();

  void render();

  void shutdown();

  private:
  bool showDebugWindow = true;
  bool showDemoWindow = true;
  bool withMenuBar = true;
  bool horizontalMenuBar = true;
  bool verticalMenuBar = true;
  bool withDockSpace = true;


  float horizontalMenuBarFixedHeight = 0;
  float verticalMenuBarFixedWidth = 0;

  const  HBAnimProps<ImVec2> sidebarAnimationData = {
      .start          = {25, 0},  //calculated from left top corner
      .end            = {225, 0}, //calculated from left top corner
      .current        = {25, 0},
      .duration       = 1.f,
      .playbackSpeed  = 1.f,
      .looping        = false,
      .type           = HB_AnimType_Linear,
      .direction      = HB_AnimDirection_Forward, //start direction
      .state          = HB_AnimState_Idle,
      .effect         = HB_AnimEffect_Expand, //if set to move you can change it to anywhere on the screen
  };
};


#endif//IMGUI_MAINWINDOW_H
