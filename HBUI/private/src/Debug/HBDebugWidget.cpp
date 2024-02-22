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
      ImGuiID     id    = anim.first;
      std::string title = "Animation " + anim.second->getId();


      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::SeparatorText(title.c_str());
      //id data
      {
        ImGui::Text("imgui id: %f", anim.first);
      }
      auto currentData = std::dynamic_pointer_cast<HBUI::Animation::HBAnim<ImVec2>>(anim.second);

      if (currentData) {
        ImGui::Text("type:   ImVec2");
        ImGui::SeparatorText("ImVec2 StartData");
        ImGui::Text("start   value:  %f, %f", currentData->getDefaultStartValue().x,
                    currentData->getDefaultStartValue().y);
        ImGui::Text("end     value:  %f, %f", currentData->getDefaultEndValue().x, currentData->getDefaultEndValue().y);
        ImGui::Text("current value:  %f, %f", currentData->getCurrentValue().x, currentData->getCurrentValue().y);
        ImGui::Text("duration:       %f", currentData->getDuration());
        ImGui::Text("playbackSpeed:  %f", currentData->getPlaybackSpeed());
        ImGui::Text("looping:        %s", currentData->getDefaultLooping() ? "true" : "false");

        std::string  type  = "type: ";
        HB_AnimType_ type_ = currentData->getDefaultType();
        switch (type_) {
          case HB_AnimType_Linear:
            type += "Linear";
            break;
          default:
            type += "Unknown";
            break;
        }
        ImGui::Text(type.c_str());

        std::string       dir       = "direction: ";
        HB_AnimDirection_ direction = currentData->getDefaultDirection();
        switch (direction) {
          case HB_AnimDirection_Forward:
            dir += "Forward";
            break;
          case HB_AnimDirection_Backward:
            dir += "Backward";
            break;
          default:
            dir += "Unknown";
            break;
        }
        ImGui::Text(dir.c_str());

        auto        state = currentData->getDefaultState();
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
      }

      //Current
      ImGui::SeparatorText("Current");

      ImGui::DragFloat("current time", &currentData->getCurrentTime());
      ImGui::DragFloat2("current value", &currentData->getCurrentValue().x);
      ImGui::DragFloat2("current start", &currentData->getStartValue().x);
      ImGui::DragFloat2("current end", &currentData->getEndValue().x);
      bool looping = currentData->isLooping();
      if (ImGui::Checkbox("looping", &looping)) {
        currentData->setLooping(looping);
      }
      float playbackSpeed = currentData->getPlaybackSpeed();
      if (ImGui::DragFloat("playbackSpeed", &playbackSpeed, 0.1f, 0, 40)) {
        currentData->setPlaybackSpeed(playbackSpeed);
      }

      ImGui::Spacing();

      //state data
      {
        auto        state = currentData->getState();
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


        //reset
        if (ImGui::Button("Reset")) {
          currentData->reset();
        }
        if (ImGui::IsItemHovered()) {
          ImGui::BeginTooltip();
          ImGui::Text("Reset the starting values of the animation");
          ImGui::EndTooltip();
        }

        //stop
        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
          currentData->stop();
        }
        if (ImGui::IsItemHovered()) {
          ImGui::BeginTooltip();
          ImGui::Text("Stop the animation");
          ImGui::EndTooltip();
        }

        //pause
        ImGui::SameLine();
        if (ImGui::Button("Pause")) {
          currentData->pause();
        }
        if (ImGui::IsItemHovered()) {
          ImGui::BeginTooltip();
          ImGui::Text("Pause the animation");
          ImGui::EndTooltip();
        }

        //play
        ImGui::SameLine();
        if (ImGui::Button("Play")) {
          currentData->play();
        }
        if (ImGui::IsItemHovered()) {
          ImGui::BeginTooltip();
          ImGui::Text("Play the animation");
          ImGui::EndTooltip();
        }

        //current
        ImGui::SameLine();
        ImGui::Text(txt.c_str());
      }
      ImGui::Spacing();

      //dir
      {
        std::string       dir       = "direction: ";
        HB_AnimDirection_ direction = currentData->getDirection();
        switch (direction) {
          case HB_AnimDirection_Forward:
            dir += "Forward";
            break;
          case HB_AnimDirection_Backward:
            dir += "Backward";
            break;
          default:
            dir += "Unknown";
            break;
        }
        ImGui::Text("direction: %s", dir.c_str());
        if (ImGui::Button("Forward")) {
          currentData->setDirection(HB_AnimDirection_Forward);
        }
        ImGui::SameLine();
        if (ImGui::Button("Backward")) {
          currentData->setDirection(HB_AnimDirection_Backward);
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

//    for (auto &bar: drawData.sideBars) {
//      ImGui::SeparatorText(bar->idString.c_str());
//
//      ImGui::Text("pos  = %f, %f", bar->windowPos.x, bar->windowPos.y);
//      ImGui::Text("size = %f, %f", bar->windowSize.x, bar->windowSize.y);
//
//      {
//        ImGui::BeginDisabled();
//        bool hor = bar->isHorizontal();
//        bool ver = bar->isVertical();
//        ImGui::Checkbox("horizontal:   ", &hor);
//        ImGui::Checkbox("vertical  :   ", &ver);
//        ImGui::EndDisabled();
//      }
//      ImGui::Spacing();
//      ImGui::Text("childs");
//      ImGui::Spacing();
//      if(bar->items.empty()){
//        ImGui::Text("No items");
//      }else {
//        for (auto &child: bar->items) {
//          ImGui::Text("id: %s", child->idString.c_str());
//          ImGui::Text("start  = %f, %f", child->m_primitive.getStart().x, child->m_primitive.getStart().y);
//          ImGui::Text("end    = %f, %f", child->m_primitive.getEnd().x, child->m_primitive.getEnd().y);
//        }
//      }
//    }
  }

  void drawDebugDrawDataNode() {
    const HBContext  &ctx      = *HBUI::getCurrentContext();
    const HBDrawData &drawData = *ctx.drawData;

    ImGui::Text("DrawData");
    ImGui::Spacing();
  }

  HBUI_API void printVec2(const ImVec2 &vec2, const std::string &name) {
    // Set the width for the first column
    int width = 70;

    std::string type = "ImVec2";
    ImVec2      val  = vec2;

    ImGui::LogText("%s %s = {%.2ff, %.2ff};" IM_NEWLINE, type.c_str(), name.c_str(), val.x, val.y);
    //todo: add a log to file option
  }


  HBUI_API void showDebugWindow(bool *p_open) {
    HBContext *ctx = HBUI::getCurrentContext();
    if (!ImGui::Begin("Debug HBUI Window", p_open)) {
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
          {"ctx.mainWindowSettings.MainWindowFlags:   No Decoration                 ", HBMainWindowFlags_NoDecoration},
          {"ctx.mainWindowSettings.MainWindowFlags:   No Resize                     ", HBMainWindowFlags_NoResize},
          {"ctx.mainWindowSettings.MainWindowFlags:   No Move                       ", HBMainWindowFlags_NoMove},
          {"ctx.mainWindowSettings.MainWindowFlags:   No TitleBar                   ", HBMeinWindowFlags_NoTitleBar},
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
//          ImGui::DragFloat2("mainMenuBar   ---VerticalFirstItemOffset----   | ",
//                            (float *) &style.sideBarVerticalFirstItemOffset);
//          ImGui::DragFloat2("mainMenuBar   ---HorizontalFirstItemOffset---  | ",
//                            (float *) &style.mainMenuBarHorizontalFirstItemOffset);
          ImGui::ColorEdit4("mainMenuBar   ---Color------------------------ | ", (float *) &style.sideBarColor);
          ImGui::Checkbox("mainMenuBar   ---UseMenuBarColor-------------  | ",
                          (bool *) &style.useHBUIStyleMenuBarColor);

          ImGui::ColorEdit4("mainMenuBar   ---ItemColor-------------------- | ",
                            (float *) &style.sideBarItemColor);
//          ImGui::Checkbox("mainMenuBar   ---UseItemColor----------------- | ",
//                          (bool *) &style.useHBUIStyleMainMenuItemColor);

          ImGui::DragFloat2("mainMenuItem  ---Size------------------------- | ", (float *) &style.menuItemSide);
//          ImGui::DragFloat2("mainMenuItems ---Padding---------------------- | ",
//                            (float *) &style.menuItemPadding);
          ImGui::DragFloat2("mainMenuItems ---Spacing---------------------- | ",
                            (float *) &style.menuItemSpacing);

          if (ImGui::Button("Reset Style")) {
            style = HBStyle();
          }

          ImGui::SameLine();
          if (ImGui::Button("Print Style")) {
//            getStyle().logToTTY();
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

  HBUI_API void printVec4(const ImVec4 &vec4, const std::string &name) {
    std::string type = "ImVec4";
    ImVec4      val  = vec4;
    ImGui::LogText("%s %s = {%.2ff, %.2ff, %.2ff, %.2ff};" IM_NEWLINE, type.c_str(), name.c_str(), val.x, val.y,
                   val.z,
                   val.w);
  }
}// namespace HBUI
