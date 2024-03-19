//
// Created by Kasper de Bruin on 19/01/2024.
//
#include "MainWindow.h"

#include <HBUI/HBUI.h>


std::string GEN_RANDOM_TEXT(const int len) {
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  std::string tmp_s;
  tmp_s.reserve(len);

  for (int i = 0; i < len; ++i) {
    tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return tmp_s;
}

void MainWindow::addTextButton() {
  textButtonLabels.push_back(GEN_RANDOM_TEXT(textButtonTextLength));
}

void MainWindow::addIconButton() {
  iconButtons.push_back(GEN_RANDOM_TEXT(1));
}

void MainWindow::removeTextButton() {
  if (!textButtonLabels.empty()) {
    textButtonLabels.pop_back();
  }
}

void MainWindow::removeIconButton() {
  if (!iconButtons.empty()) {
    iconButtons.pop_back();
  }
}

void MainWindow::init() {
  HBUI::initialize("Hello, World!", 1280, 720);

  for (int i = 0; i < textButtonCount; i++) {
    textButtonLabels.push_back(GEN_RANDOM_TEXT(textButtonTextLength));
  }

  for (int i = 0; i < iconButtonCount; i++) {
    iconButtons.push_back(GEN_RANDOM_TEXT(1));
  }

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

  HBUI::beginTaskBar("HorizontalTaskBar", {0, 0});

    if (showTextButtons) {
      for (const auto &label: textButtonLabels) {
        if (HBUI::textButton(label)) {
          textButtonLabels.erase(std::remove(textButtonLabels.begin(), textButtonLabels.end(), label), textButtonLabels.end());
        }
      }
    }
    if (showIconButtons) {
      for(int i = 0; i < iconButtonCount; ++i) {
        int count = HBUI::getDefaultIconNames().size() - 1;
        int rand =  std::rand() % count;
        if (HBUI::iconButton(HBUI::getIcon(HBUI::getDefaultIconNames()[rand]))) {
        }
      }
    }
    HBUI::endTaskBar();


  if (showHUIDemoWindow) {
    ImGui::Begin("DemoWindow", &showHUIDemoWindow);

    ImGui::Text("This is a simple demo of the library.");
    if (ImGui::CollapsingHeader("IMGUI Help")) {
      ImGui::SeparatorText("ABOUT THIS DEMO:");
      ImGui::BulletText("Sections below are demonstrating many aspects of the library.");
      ImGui::SeparatorText("IMGUI USER GUIDE:");
      ImGui::ShowUserGuide();
    }

    if (ImGui::CollapsingHeader("HBUI Help")) {
      ImGui::SeparatorText("ABOUT HBUI:");
      ImGui::BulletText("HBUI is a wrapper around IMGUI to make it easier to use.");
      ImGui::BulletText("It has a lot of features and is easy to use.");
      ImGui::BulletText("It is still in development and is not yet stable.");
      ImGui::BulletText("It is not recommended to use it in production.");
    }

    if (ImGui::CollapsingHeader("HBUI Debug")) {
      ImGui::Checkbox("Show HBUI Debug Widgets", &showHBUIDebugWidgets);
    }

    if (ImGui::CollapsingHeader("Menu Bar Items")) {
      ImGui::SeparatorText("Text Buttons");
      ImGui::Checkbox("Show Text Buttons", &showTextButtons);
      if (ImGui::InputInt("Text Button Count", &textButtonCount)) {
        if (textButtonCount < 0) {
          textButtonCount = 0;
        }
        if (textButtonCount > textButtonLabels.size()) {
          addTextButton();
        } else if (textButtonCount < textButtonLabels.size()) {
          removeTextButton();
        }
      }
      if (ImGui::InputInt("Text Button Text Length", &textButtonTextLength)) {
        if (textButtonTextLength <= 1) {
          textButtonTextLength = 2;
        }
        for (auto &label: textButtonLabels) {
          label = GEN_RANDOM_TEXT(textButtonTextLength);
        }
      }

      ImGui::SeparatorText("Icon Buttons");
      ImGui::Checkbox("Show Icon Buttons", &showIconButtons);
      if (ImGui::InputInt("Icon Button Count", &iconButtonCount)) {
        if (iconButtonCount < 0) {
          iconButtonCount = 0;
        }
        if (iconButtonCount > iconButtons.size()) {
          addIconButton();
        } else if (iconButtonCount < iconButtons.size()) {
          removeIconButton();
        }
      }
    }

    if (ImGui::CollapsingHeader("Style")) {
      ImGui::SetCursorPosX(40);

      ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5f);
      if (ImGui::CollapsingHeader("HBUI Style Editor")) {
        ImGui::SetCursorPosX(40);
        ImGui::Button("test");
      }
      ImGui::SetCursorPosX(40);
      ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5f);
      if (ImGui::CollapsingHeader("ImGui Style Editor")) {
        ImGui::SetCursorPosX(40);
        ImGui::BeginChild("ImGuiStyleEditor", {0, 0}, true);
        ImGui::ShowStyleEditor();
        ImGui::EndChild();
      }
    }

    ImGui::End();
  }

  HBUI::endFrame();
}

void MainWindow::shutdown() {
  HBUI::shutdown();
}
