//
// Created by Kasper de Bruin on 01/11/2023.
//

#include "ThemeManager.h"
#include "Themes.h"
#include <string>
#include <vector>

namespace KDB_IMGUI {
  namespace Themes {

    extern ThemeInfo gThemeInfos[];
    int KDB_IMGUI::Themes::ThemeManager::currentThemeIDX = 0;
    ImGuiTweakedTheme KDB_IMGUI::Themes::ThemeManager::tweakedTheme = {};

    void ThemeManager::showThemeManager(bool *p_open) {
      ImGui::Begin("Theme Manager", p_open, ImGuiWindowFlags_MenuBar);
      if (ImGui::BeginMenuBar()) {
        if (ImGui::MenuItem("Save Theme")) {
          //Todo:
          // SaveThemeToFile(tweakedTheme);
        }
        if (ImGui::MenuItem("Load Theme")) {
          //Todo:
          // LoadThemeFromFile(tweakedTheme);
        }
        ImGui::EndMenuBar();
      }
      ImGui::Text("Theme");
      ImGui::Separator();// ---------------------------------------------------------
      ImGui::Text("Select a theme from the list below.");
      ImGui::Text("The theme will be applied to all ImGui windows.");
      ImGui::Text("You can also tweak the theme by changing the values below.");
      ImGui::Separator();// ---------------------------------------------------------
      if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
        for (int n = 0; n < ImGuiTheme_Count; ++n) {
          const bool is_selected = (currentThemeIDX == n);
          if (ImGui::Selectable(gThemeInfos[n].Name, is_selected)) {
            applyTheme(gThemeInfos[n].Theme);
          }

          // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
      }

      ImGui::Separator();// ---------------------------------------------------------
      ImGui::Text("Tweaks");
      ImGui::Separator();// ---------------------------------------------------------
      ShowThemeTweakGui(&tweakedTheme);
      if (ImGui::Button("Apply Tweaks")) {
        ApplyTweakedTheme(tweakedTheme);
      }

      ImGui::End();
    }

    void ThemeManager::applyTheme(ImGuiTheme_ theme) {
      if (theme == currentThemeIDX) return;
      if (theme < 0 || theme > ImGuiTheme_Count) return;

      ImGuiStyle style = ThemeToStyle(theme);
      currentThemeIDX = theme;
//
      auto io = ImGui::GetIO();
      // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        style.WindowPadding = ImVec2(10.0f, 10.0f);
        style.FramePadding = ImVec2(8.0f, 6.0f);
        style.ItemSpacing = ImVec2(6.0f, 6.0f);
        style.PopupRounding = 6.0f;
        style.FrameRounding = 6.0f;
        style.ChildRounding = 0;
        style.WindowRounding = 6;
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
      }

      ImGui::GetStyle() = style;

      tweakedTheme = {};
      tweakedTheme.Theme = theme;
    }
  }// namespace Themes
}// namespace KDB_ImGui