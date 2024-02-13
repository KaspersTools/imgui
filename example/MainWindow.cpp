//
// Created by Kasper de Bruin on 19/01/2024.
//
#include "MainWindow.h"
#include <HBUI/HBUI.h>

static int passes = 0;

void MainWindow::init() {
  HBUIContext *ctx = HBUI::initialize();
  HBUI::setCurrentContext(ctx);

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
  ImGui::Begin("Logs");
  ImGui::Text("Hello, world!");
  ImGui::Text("This is some useful text.");
  ImGui::End();

  if(passes < 100) {
    passes++;
  }
  HBUI::endFrame();
}

void MainWindow::shutdown() {
  HBUI::shutdown();
}
