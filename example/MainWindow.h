//
// Created by Kasper de Bruin on 19/01/2024.
//

#ifndef IMGUI_MAINWINDOW_H
#define IMGUI_MAINWINDOW_H

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
    bool showDemoWindow = false;
    bool showAboutWindow = false;
    bool showStyleEditor = false;
    bool showMetricsWindow = false;
    bool showDebugLogWindow = false;
    bool showIDStackToolWindow = false;
    bool showFontSelector = false;
    bool showStyleSelector = false;
    bool showUserGuide = false;
};


#endif//IMGUI_MAINWINDOW_H
