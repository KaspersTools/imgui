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
  void createDockSpace();
  void render();
  void shutdown();

  private:
    bool showDemoWindow = true;

  private:
      bool withMenuBar        = true;
      bool withDockSpace      = true;
      bool horizontalMenuBar  = true;
      bool verticalMenuBar    = false;
};


#endif//IMGUI_MAINWINDOW_H
