//
// Created by Kasper de bruin on 08/03/2024.
//

#ifndef IMGUI_HBMAINWINDOW_H
#define IMGUI_HBMAINWINDOW_H

#include "HBUI/HBWindow.h"

namespace HBUI::Windows {

  class HBMainWindow : public HBWindow {
  public:
    explicit HBMainWindow([[maybe_unused]] std::string label = "Main Window HBUI",
                          HBUIWindowFlags              flags =
                              HBUIWindowFlags_HasTaskBar |
                              HBUIWindowFlags_HasDock |
                              HBUIWindowFlags_NoOuterBorders,
                          ImGuiWindowFlags imguiWindowFlags =
                              ImGuiWindowFlags_NoDocking |
                              ImGuiWindowFlags_NoTitleBar |
                              ImGuiWindowFlags_NoCollapse |
                              ImGuiWindowFlags_NoResize |
                              ImGuiWindowFlags_NoMove |
                              ImGuiWindowFlags_NoBringToFrontOnFocus |
                              ImGuiWindowFlags_NoNavFocus)
        : HBWindow(std::move(label),
                   ImGui::GetID(label.c_str()),
                   flags,
                   imguiWindowFlags,
                   ImVec2(0, 0),
                   ImVec2(0, 0),
                   HBDirection_TopToBottom,
                   ImVec2(0, 0),
                   HBStyleFlags_StretchHorizontal | HBStyleFlags_StretchVertical,
                   nullptr,
                   HBUIType_Window) {
    }

  protected:
//    ImVec2 getScreenPos() override {
//      return HBUI::getMainViewportPos();
//    }

//    ImVec2 getSize() override {
//      return HBUI::getMainViewportSize();
//    }
  };

}// namespace HBUI::Windows


#endif//IMGUI_HBMAINWINDOW_H
