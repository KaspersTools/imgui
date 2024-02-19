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

  void drawAnimDebugNode() {
    const HBContext  &ctx      = *HBUI::getCurrentContext();
    const HBDrawData &drawData = *ctx.drawData;

    ImGui::Text("Anim");
    ImGui::Spacing();

    for (auto &anim: ctx.animManager->getAnimations()) {
      const std::string title = "Animation: " + anim.second->getId();
      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::SeparatorText(title.c_str());
      //id data
      {
        ImGui::Text("id:     %f", anim.first);
        ImGui::Text("id str: %s", anim.second->getId().c_str());
      }
      //ImVec2 data
      auto animPtr = std::dynamic_pointer_cast<HBAnim<ImVec2>>(anim.second);
      if (animPtr) {
        ImGui::Text("type:   ImVec2");

        ImGui::Text("current value:  %f, %f", animPtr->getCurrentValue().x, animPtr->getCurrentValue().y);
        ImGui::Text("start   value:  %f, %f", animPtr->getStartValue().x, animPtr->getStartValue().y);
        ImGui::Text("end     value:  %f, %f", animPtr->getEndValue().x, animPtr->getEndValue().y);
      }
      //state data
      {
        auto        state = animPtr->getState();
        std::string txt   = "state: ";
        switch (state) {
          case HB_AnimState_Idle:
            txt += "Idle";
            break;
          case HB_AnimState_Playing:
            txt += "Playing";
            break;
          case HB_AnimState_Paused:
            txt += "Paused";
            break;
          case HB_AnimState_Stopped:
            txt += "Stopped";
            break;
          case HB_AnimState_Finished:
            txt += "Finished";
            break;
          default:
            txt += "Unknown";
            break;
        }

        ImGui::Text(txt.c_str());
        //play
        if (ImGui::Button("Play")) {
          animPtr->play();
        }
      }
      ImGui::Spacing();

      //time data
      {
        bool looping = animPtr->isLooping();
        if (ImGui::Checkbox("looping", &looping)) {
          animPtr->setLooping(looping);
        }

        float duration = animPtr->getDuration();
        if (ImGui::DragFloat("durations", &duration, .1f, 0, 40)) {
          animPtr->setDuration(duration);
        }

        float playbackSpeed = animPtr->getPlaybackSpeed();
        if (ImGui::DragFloat("playbackSpeed", &playbackSpeed, 0.1f, 0, 40)) {
          animPtr->setPlaybackSpeed(playbackSpeed);
        }
      }

      //dir
      {
        const std::string dir = animPtr->getDirection();
        ImGui::Text("direction: %s", dir.c_str());
        if (ImGui::Button("Forward")) {
          animPtr->setDirection(HB_AnimDirection_Forward);
        }
        ImGui::SameLine();
        ImGui::Dummy({20, 0});
        if (ImGui::Button("Backward")) {
          animPtr->setDirection(HB_AnimDirection_Backward);
        }
      }
      ImGui::Spacing();

    }
  }

  void drawMainMenubarDebugNode() {
    const HBContext  &ctx      = *HBUI::getCurrentContext();
    const HBDrawData &drawData = *ctx.drawData;

    ImGui::Text("MainMenuBar");
    ImGui::Spacing();

    for (auto &bar: drawData.mainMenuBars) {
      ImGui::SeparatorText(bar->idString.c_str());

      ImGui::Text("pos  = %f, %f", bar->windowPos.x, bar->windowPos.y);
      ImGui::Text("size = %f, %f", bar->windowSize.x, bar->windowSize.y);
      {
        ImGui::BeginDisabled();
        bool hor = bar->isHorizontal();
        bool ver = bar->isVertical();
        ImGui::Checkbox("horizontal:   ", &hor);
        ImGui::Checkbox("vertical  :   ", &ver);
        ImGui::EndDisabled();
      }

      ImGui::Spacing();
    }
  }

  void drawDebugDrawDataNode() {
    const HBContext  &ctx      = *HBUI::getCurrentContext();
    const HBDrawData &drawData = *ctx.drawData;

    ImGui::Text("DrawData");
    ImGui::Spacing();

    ImGui::Text("DockspaceFlags: %d", drawData.dockspaceFlags);
    ImGui::Text("currentAppendingMenuBar: %p", drawData.currentAppendingMenuBar.get());
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
    if (ImGui::CollapsingHeader("Debug Animations")) {
      drawAnimDebugNode();
    }
    if (ImGui::CollapsingHeader("Debug Main Menu Bars")) {
      drawMainMenubarDebugNode();
    }
    if (ImGui::CollapsingHeader("Backend Flags")) {
      HelpMarker(
          "These flags are used by the backends (ImVK) to specify their capabilities.\n");

      ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "ctx.backendFlags:");
      ImGui::TextColored(ImVec4(1.f, 0.5f, 0.5f, 1.0f),
                         "Best to set these before window creation but they are here for testing purposes only!:");

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
          ImGui::DragFloat2("mainMenuBar   ---VerticalFirstItemOffset----   | ",
                            (float *) &style.mainMenuBarVerticalFirstItemOffset);
          ImGui::DragFloat2("mainMenuBar   ---HorizontalFirstItemOffset---  | ",
                            (float *) &style.mainMenuBarHorizontalFirstItemOffset);
          ImGui::ColorEdit4("mainMenuBar   ---Color------------------------ | ", (float *) &style.menuBarColor);
          ImGui::Checkbox("mainMenuBar   ---UseMenuBarColor-------------  | ",
                          (bool *) &style.useHBUIStyleMenuBarColor);

          ImGui::ColorEdit4("mainMenuBar   ---ItemColor-------------------- | ",
                            (float *) &style.mainMenuBarItemColor);
          ImGui::Checkbox("mainMenuBar   ---UseItemColor----------------- | ",
                          (bool *) &style.useHBUIStyleMainMenuItemColor);

          ImGui::DragFloat2("mainMenuItem  ---Size------------------------- | ", (float *) &style.mainMenuItemSize);
          ImGui::DragFloat2("mainMenuItems ---Padding---------------------- | ",
                            (float *) &style.mainMenuItemsPadding);
          ImGui::DragFloat2("mainMenuItems ---Spacing---------------------- | ",
                            (float *) &style.mainMenuItemsSpacing);

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
    ImVec2      val  = vec2;

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
    ImVec4      val  = vec4;
    ImGui::LogText("%s %s = {%.2ff, %.2ff, %.2ff, %.2ff};" IM_NEWLINE, type.c_str(), name.c_str(), val.x, val.y,
                   val.z,
                   val.w);
  }
}// namespace HBUI
