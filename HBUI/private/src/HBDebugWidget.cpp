//
// Created by Kasper de Bruin on 14/02/2024.
//

#include <HBUI/HBUI.h>
#include <iostream>
#include <map>

namespace HBUI {
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


  HBUI_API void showDebugWindow(bool *p_open) {
    if (!ImGui::Begin("Debug HBUI Window", p_open)) {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      return;
    }

    ImGui::Text("HBUI says hello! (%s)", HBUI_VERSION);
    ImGui::Text("also dear imgui says hello ;)! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
    ImGui::Spacing();

    // Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
    // e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
    //ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
    // e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    HBContext *ctx = HBUI::getCurrentContext();

    if (ImGui::TreeNode("Backend Flags")) {
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

      ImGui::TreePop();
      ImGui::Spacing();
    }

    if (ImGui::TreeNode("Style")) {
      ImGui::Text("Style");
      ImGui::Spacing();

      HBStyle &style = getStyle();

      if (ImGui::BeginTabBar("StyleTabBar")) {

        if (ImGui::BeginTabItem("HBUII sizes")) {
          ImGui::SliderFloat("verticalMainMenuBarWidth             | //The width of the vertical mainmenubar                                                                              ", &style.mainMenuVerticalWidth,    0, 200);
          ImGui::SliderFloat("horizontalMainMenuBarHeight          | //The height of the horizontal mainmenubar                                                                           ", &style.mainMenuHorizontalHeight, 0, 200);
          ImGui::EndTabItem();/////////////////////////////////////|//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////"
         }//////////////////////////////////////////////////////////|//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////"
///////////////////////////////////////////////////////////////////|//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////"
        if (ImGui::BeginTabItem("Colors")){////////////////////////|//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////"
          ImGui::ColorEdit4("WindowBg                              | //By default the menubarbg of imgui is used. if you want you can overwite the main menu bar color with this stlye    ", (float *) &style.menuBarColor);
          ImGui::SameLine();
          if (ImGui::Button("reset")) {
            style.menuBarColor = ImVec4(-1, -1, -1, 255);
          }
          ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("ImGuiStyle")) {
          ImGui::ShowStyleEditor();
          ImGui::EndTabItem();
        }


        ImGui::EndTabBar();
      }


      ImGui::TreePop();
    }
    ImGui::End();
  }
}// namespace HBUI
