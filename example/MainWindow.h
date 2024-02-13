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
  void render();
  void shutdown();

  private:
    bool showDemoWindow = true;

  private:
    bool showAboutWindow = false;
    bool showStyleEditor = true;
    bool showMetricsWindow = false;
    bool showDebugLogWindow = false;
    bool showIDStackToolWindow = false;
    bool showFontSelector = false;
    bool showUserGuide = false;
};


#endif//IMGUI_MAINWINDOW_H
