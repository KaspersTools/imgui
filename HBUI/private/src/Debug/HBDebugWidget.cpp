//
// Created by Kasper de Bruin on 14/02/2024.
//

#include "HBUI/HBUI.h"
#include <iostream>
#include <map>

namespace HBUI {
  struct debugData{

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

    ImGui::SeparatorText("Horizontal MainMenuBar");
    ImGui::Text("mainMenuBarHorizontal: %p", drawData.mainMenuBarHorizontal.get());
    if( ctx.drawData->mainMenuBarHorizontal.get() != nullptr && ctx.drawData->mainMenuBarHorizontal.get()->items.size() > 0) {

      for (auto &child: ctx.drawData->mainMenuBarHorizontal.get()->items) {
        ImGui::SeparatorText("Child");
        ImGui::Text("child: %p", child.get());
        ImGui::Text("child->start: %f, %f", child->pos.x, child->pos.y);
        ImGui::Text("child->size: %f, %f", child->size.x, child->size.y);
      }
    }

    ImGui::SeparatorText("Vertical MainMenuBar");
    ImGui::Text("mainMenuBarVertical: %p", drawData.mainMenuBarVertical.get());
    if( ctx.drawData->mainMenuBarVertical.get() != nullptr && ctx.drawData->mainMenuBarVertical.get()->items.size() > 0) {

      for (auto &child: ctx.drawData->mainMenuBarVertical.get()->items) {
        ImGui::SeparatorText("Child");
        ImGui::Text("child: %p", child.get());
        ImGui::Text("child->start: %f, %f", child->pos.x, child->pos.y);
        ImGui::Text("child->size: %f, %f", child->size.x, child->size.y);
      }
    }

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

    if(ImGui::CollapsingHeader("Debug Draw Data")){
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
    if (ImGui::CollapsingHeader("Style"))         {
      ImGui::Text("Style");
      ImGui::Spacing();
      HBStyle &style = getStyle();

      if (ImGui::BeginTabBar("StyleTabBar")) {
        if (ImGui::BeginTabItem("HBUIStyle")) {
          ImGui::DragFloat2("mainMenuBar   ---VerticalFirstItemOffset----   | ", (float *) &style.mainMenuBarVerticalFirstItemOffset);
          ImGui::DragFloat2("mainMenuBar   ---HorizontalFirstItemOffset---  | ", (float *) &style.mainMenuBarHorizontalFirstItemOffset);
          ImGui::ColorEdit4("mainMenuBar   ---Color------------------------ | ", (float *) &style.menuBarColor        );
          ImGui::ColorEdit4("mainMenuBar   ---ItemColor-------------------- | ", (float *) &style.mainMenuBarItemColor);

          ImGui::DragFloat2("mainMenuItem  ---Size------------------------- | ", (float *) &style.mainMenuItemSize);
          ImGui::DragFloat2("mainMenuItems ---Padding---------------------- | ", (float *) &style.mainMenuItemsPadding);
          ImGui::DragFloat2("mainMenuItems ---Spacing---------------------- | ", (float *) &style.mainMenuItemsSpacing);
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("ImGuiStyle")) {
          ImGui::ShowStyleEditor();
          ImGui::EndTabItem();
        }


        ImGui::EndTabBar();
      }
    }

    if(ImGui::CollapsingHeader("Degubg Info")){
      ImGui::SeparatorText("ABOUT THIS DEMO:");
      ImGui::BulletText("This HBUI Debug Window is a simple example of how to use HBUI.");
      ImGui::BulletText("All the UI elements are available and shown in this window.");
    }

    ImGui::End();
  }
}// namespace HBUI
