//
// Created by Kasper de Bruin on 19/01/2024.
//
#include "MainWindow.h"

#include <HBUI/HBUI.h>

void MainWindow::init() {
  HBUI::initialize("Hello, World!", 1280, 720);

  m_ColorVerticalBar = m_ColorHorizontalBar = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg);

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

  std::map<std::string, ImGuiMenuColumns> m_MenuColumns = {};

  HBUI::beginTaskBar("HorizontalTaskBar", {0, 0});
  HBUI::textButton("Button1");
  HBUI::textButton("Button2");
  HBUI::endTaskBar();
  ImGui::Begin("ImGuiData", nullptr, ImGuiWindowFlags_MenuBar);
  {
    m_MenuColumns["MenuColumns_0"] = ImGui::GetCurrentWindow()->DC.MenuColumns;
    ImGui::BeginMenuBar();
    {
      m_MenuColumns["MenuColumns_1"] = ImGui::GetCurrentWindow()->DC.MenuColumns;
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open", "Ctrl+O")) {

        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {

        }
        if (ImGui::MenuItem("Close", "Ctrl+W")) {
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit")) {
        m_MenuColumns["MenuColumns_2"] = ImGui::GetCurrentWindow()->DC.MenuColumns;
        if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
          }
        if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {
          // Do stuff
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View")) {
        if (ImGui::MenuItem("Show Demo Window", "", &showDemoWindow)) {
          // Do stuff
        }
        if (ImGui::MenuItem("Show HBUI Debug Widgets", "", &showHBUIDebugWidgets)) {
          // Do stuff
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("About", "")) {
          // Do stuff
        }
        ImGui::EndMenu();
      }
    }
    ImGui::EndMenuBar();
  }

  for (auto &menuColumn : m_MenuColumns) {
    ImGui::SeparatorText( menuColumn.first.c_str());

    ImGui::Text("NextTotalWidth %u", menuColumn.second.NextTotalWidth);
    ImGui::Text("Spacing %u", menuColumn.second.Spacing);
    ImGui::Text("OffsetIcon %u", menuColumn.second.OffsetIcon);         // Always zero for now
    ImGui::Text("OffsetLabel %u", menuColumn.second.OffsetLabel);        // Offsets are locked in Update()
    ImGui::Text("OffsetShortcut %u", menuColumn.second.OffsetShortcut);
    ImGui::Text("OffsetMark %u", menuColumn.second.OffsetMark);

    for (int i = 0; i < 4; i++)
      ImGui::Text("Widths %i = %u", i, menuColumn.second.Widths[i]);
  }
  ImGui::End();

  ImGui::Begin("HBUI Settings");
  {
    if (ImGui::CollapsingHeader("Native window")) {
      ImGui::ColorEdit3("Native Window Clear Color 1", &HBUI::getNativeWindowClearColor().Value.x);
    }

    if (ImGui::CollapsingHeader("HBUI Widgets")) {
      if (ImGui::CollapsingHeader("TaskBars")) {
        if (ImGui::TreeNode("Horizontal TaskBar")) {
          ImGui::SliderFloat("Horizontal Bar Height", &m_HorizontalBarHeight, 0, 100);
          ImGui::ColorEdit3("Color Horizontal Bar", &m_ColorHorizontalBar.x);
          ImGui::TreePop();
        }
        if (ImGui::TreeNode("Vertical TaskBar")) {
          ImGui::SliderFloat("Vertical Bar Width", &m_VerticalBarWidth, 0, 100);
          ImGui::ColorEdit3("Color Vertical Bar", &m_ColorVerticalBar.x);
          ImGui::TreePop();
        }
      }
    }

    if (ImGui::CollapsingHeader("ImGui Widgets")) {
      if (ImGui::TreeNode("Style")) {
        ImGui::ShowStyleEditor();
        ImGui::TreePop();
      }
    }


    if (ImGui::CollapsingHeader("Test Widgets")) {
      ImGui::Text("textt");
      ImGui::Button("button");
      ImGui::SliderFloat2("button size", &m_ButtonSize.x, 0, 100);
      ImGui::Button("button2", m_ButtonSize);
      ImGui::Button("Button With Elipsis", m_ButtonSize);
    }

    ImGui::End();
  }

  ImGui::ShowDemoWindow(&showDemoWindow);
  HBUI::showWidgetDebugWindow(&showHBUIDebugWidgets);

  HBUI::endFrame();
}

void MainWindow::shutdown() {
  HBUI::shutdown();
}
