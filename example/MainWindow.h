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

  void addTextButton();
  void addIconButton();
  void removeTextButton();
  void removeIconButton();

private:
  bool showHBUIDebugWidgets = true;
  bool showDemoWindow       = true;
  bool showHUIDemoWindow    = true;

  bool showTextButtons = false;
  bool showIconButtons = true;

  int iconButtonCount      = 1;
  int textButtonCount      = 1;
  int textButtonTextLength = 20;

  std::vector<std::string> iconButtons      = {};
  std::vector<std::string> textButtonLabels = {};

  logger_t customLogger = nullptr;
  HBUI::Windows::Logging::LogWindow* logWindow = nullptr;
};


#endif//IMGUI_MAINWINDOW_H
