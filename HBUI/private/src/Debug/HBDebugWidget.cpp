//
// Created by Kasper de Bruin on 14/02/2024.
//

#include "HBUI/HBUI.h"
#include <iomanip>
#include <iostream>
#include <map>

namespace HBUI {
  struct debugData {
  };


  static void HelpMarker(const char *desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(desc);
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }
  }

  std::vector<std::string> flagsNames = {
          "ctx.mainWindowSettings.MainWindowFlags:   No Decoration                 ",
          "ctx.mainWindowSettings.MainWindowFlags:   No Resize                     ",
          "ctx.mainWindowSettings.MainWindowFlags:   No TitleBar                   ",
          "ctx.mainWindowSettings.MainWindowFlags:   No Move                       ",
  };

  void drawDebugDrawDataNode() {
    const HBContext &ctx = *HBUI::getCurrentContext();
    const HBDrawData &drawData = *ctx.drawData;

    ImGui::Text("DrawData");
    ImGui::Spacing();

    ImGui::Text("DockspaceFlags: %d", drawData.dockspaceFlags);
    ImGui::Text("currentAppendingMenuBar: %p", drawData.currentAppendingMenuBar.get());

    ImGui::SeparatorText("Vertical MainMenuBar");
    ImGui::Text("mainMenuBarVertical: %p", drawData.mainMenuBarVertical.get());
    ImGui::DragFloat("   ---Height----------------------- | ", (float *) &drawData.mainMenuBarVertical.get()->height);
    ImGui::DragFloat("   ---Width------------------------ | ", (float *) &drawData.mainMenuBarVertical.get()->width);
    ImGui::Separator();
    //	ImGui::ColorEdit4("   ---Color------------------------ | ", (float *) &drawData.mainMenuBarVertical.get()->color);
    //	ImGui::ColorEdit4("   ---ItemColor-------------------- | ", (float *) &drawData.mainMenuBarVertical.get()->itemColor);
    //	ImGui::DragFloat2("   ---Size------------------------- | ", (float *) &drawData.mainMenuBarVertical.get()->size);
    //	ImGui::DragFloat2("   ---Padding---------------------- | ", (float *) &drawData.mainMenuBarVertical.get()->padding);
    //	ImGui::DragFloat2("   ---Spacing---------------------- | ", (float *) &drawData.mainMenuBarVertical.get()->spacing);
    //    if( ctx.drawData->mainMenuBarVertical.get() != nullptr && ctx.drawData->mainMenuBarVertical.get()->items.size() > 0) {
    //
    //      for (auto &child: ctx.drawData->mainMenuBarVertical.get()->items) {
    //        ImGui::SeparatorText("Child");
    //        ImGui::Text("child: %p", child.get());
    //        ImGui::Text("child->start: %f, %f", child->pos.x, child->pos.y);
    //        ImGui::Text("child->size: %f, %f", child->size.x, child->size.y);
    //      }
    //    }

    ImGui::SeparatorText("Horizontal MainMenuBar");
    ImGui::Text("mainMenuBarHorizontal: %p", drawData.mainMenuBarHorizontal.get());
    ImGui::DragFloat("   ---Height----------------------- | ", (float *) &drawData.mainMenuBarHorizontal.get()->height);
    ImGui::DragFloat("   ---Width------------------------ | ", (float *) &drawData.mainMenuBarHorizontal.get()->width);
    ImGui::Separator();


    ImGui::Text("savedScreenPos: %f, %f", drawData.savedScreenPos.x, drawData.savedScreenPos.y);
  }

  HBUI_API void showDebugWindow(bool *p_open) {
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    HBContext *ctx = HBUI::getCurrentContext();

    if (!ImGui::Begin("Debug HBUI Window", p_open)) {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      return;
    }

    ImGui::Text("HBUI says hello! (%s)", HBUI_VERSION);
    ImGui::Text("also dear imgui says hello ;)! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Debug Draw Data")) {
      drawDebugDrawDataNode();
    }
    if (ImGui::CollapsingHeader("Backend Flags")) {
      HelpMarker(
              "These flags are used by the backends (ImVK) to specify their capabilities.\n");

      ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "ctx.backendFlags:");
      ImGui::TextColored(ImVec4(1.f, 0.5f, 0.5f, 1.0f), "Best to set these before window creation but they are here for testing purposes only!:");

      std::map<std::string, MainWindowFlags> flagsMap = {
              {"ctx.mainWindowSettings.MainWindowFlags:   No Decoration                 ", HBUI_MAIN_WINDOW_FLAG_NO_DECORATION},
              {"ctx.mainWindowSettings.MainWindowFlags:   No Resize                     ", HBUI_MAIN_WINDOW_FLAG_NO_RESIZE},
              {"ctx.mainWindowSettings.MainWindowFlags:   No Move                       ", HBUI_MAIN_WINDOW_FLAG_NO_MOVE},
              {"ctx.mainWindowSettings.MainWindowFlags:   No TitleBar                   ", HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR},
      };

      for (auto &flag: flagsMap) {
        bool flagSet = HBUI::isFlagSet(&ctx->io.mainWindowFlags, flag.second);

        if (ImGui::Checkbox(flag.first.c_str(), &flagSet)) {
          HBUI::toggleFlag(flag.second);
        }
      }
      ImGui::Spacing();
    }
    if (ImGui::CollapsingHeader("Style")) {
      ImGui::Text("Style");
      ImGui::Spacing();
      HBStyle &style = getStyle();

      if (ImGui::BeginTabBar("StyleTabBar")) {
        if (ImGui::BeginTabItem("HBUIStyle")) {
          ImGui::DragFloat2("mainMenuBar   ---VerticalFirstItemOffset----   | ", (float *) &style.mainMenuBarVerticalFirstItemOffset);
          ImGui::DragFloat2("mainMenuBar   ---HorizontalFirstItemOffset---  | ", (float *) &style.mainMenuBarHorizontalFirstItemOffset);
          ImGui::ColorEdit4("mainMenuBar   ---Color------------------------ | ", (float *) &style.menuBarColor);
          ImGui::Checkbox("mainMenuBar   ---UseMenuBarColor-------------  | ", (bool *) &style.useHBUIStyleMenuBarColor);

          ImGui::ColorEdit4("mainMenuBar   ---ItemColor-------------------- | ", (float *) &style.mainMenuBarItemColor);
          ImGui::Checkbox("mainMenuBar   ---UseItemColor----------------- | ", (bool *) &style.useHBUIStyleMainMenuItemColor);

          ImGui::DragFloat2("mainMenuItem  ---Size------------------------- | ", (float *) &style.mainMenuItemSize);
          ImGui::DragFloat2("mainMenuItems ---Padding---------------------- | ", (float *) &style.mainMenuItemsPadding);
          ImGui::DragFloat2("mainMenuItems ---Spacing---------------------- | ", (float *) &style.mainMenuItemsSpacing);

          if (ImGui::Button("Reset Style")) {
            style = HBStyle();
          }

          ImGui::SameLine();
          if (ImGui::Button("Print Style")) {
            getStyle().logToTTY();
          }

          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("ImGuiStyle")) {
          ImGui::ShowStyleEditor();
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
    }

    if (ImGui::CollapsingHeader("Degubg Info")) {
      ImGui::SeparatorText("ABOUT THIS DEMO:");
      ImGui::BulletText("This HBUI Debug Window is a simple example of how to use HBUI.");
      ImGui::BulletText("All the UI elements are available and shown in this window.");
    }

    ImGui::End();
  }


  HBUI_API void printVec2(const ImVec2 &vec2, const std::string &name) {
    // Set the width for the first column
    int width = 70;

    std::string type = "ImVec2";
    ImVec2 val = vec2;

    ImGui::LogText("%s %s = {%.2ff, %.2ff};" IM_NEWLINE, type.c_str(), name.c_str(), val.x, val.y);

    std::cout << std::left << std::setw(width) << "ImVec2 mainMenuBarVerticalFirstItemOffset"
              << " = {" << val.x << ", " << val.y << "}\n";
    std::cout << std::setw(width) << "ImVec2 mainMenuBarHorizontalFirstItemOffset"
              << " = {" << val.x << ", " << val.y << "}\n";
    std::cout << std::setw(width) << "ImVec2 mainMenuItemSize"
              << " = {" << val.x << ", " << val.y << "}\n";
    std::cout << std::setw(width) << "ImVec2 mainMenuItemsPadding"
              << " = {" << val.x << ", " << val.y << "}\n";
    std::cout << std::setw(width) << "ImVec2 mainMenuItemsSpacing"
              << " = {" << val.x << ", " << val.y << "}\n";
  }

  HBUI_API void printVec4(const ImVec4 &vec4, const std::string &name) {
    std::string type = "ImVec4";
    ImVec4 val = vec4;
    ImGui::LogText("%s %s = {%.2ff, %.2ff, %.2ff, %.2ff};" IM_NEWLINE, type.c_str(), name.c_str(), val.x, val.y, val.z, val.w);
  }
}// namespace HBUI
