//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once
#include <imgui.h>

namespace KDB_ImGui {
  namespace Themes {
    enum ImGuiTheme_ {
      ImGuiTheme_ImGuiColorsClassic = 0,
      ImGuiTheme_ImGuiColorsDark,
      ImGuiTheme_ImGuiColorsLight,
      ImGuiTheme_MaterialFlat,
      ImGuiTheme_PhotoshopStyle,
      ImGuiTheme_GrayVariations,
      ImGuiTheme_GrayVariations_Darker,
      ImGuiTheme_MicrosoftStyle,
      ImGuiTheme_Cherry,
      ImGuiTheme_Darcula,
      ImGuiTheme_DarculaDarker,
      ImGuiTheme_LightRounded,
      ImGuiTheme_SoDark_AccentBlue,
      ImGuiTheme_SoDark_AccentYellow,
      ImGuiTheme_SoDark_AccentRed,
      ImGuiTheme_BlackIsBlack,
      ImGuiTheme_WhiteIsWhite,
      ImGuiTheme_Count
    };

    struct ThemeInfo {
      ImGuiTheme_ Theme;
      char Name[256];
      ImGuiStyle Style;
    };

    struct ImGuiThemeTweaks {
      // Common rounding for widgets. If < 0, this is ignored.
      float Rounding = -1.f;
      // If rounding is applied, scrollbar rounding needs to be adjusted to be visually pleasing in conjunction with other widgets roundings. Only applied if Rounding > 0.f)
      float RoundingScrollbarRatio = 4.f;
      // Change the alpha that will be applied to windows, popups, etc. If < 0, this is ignored.
      float AlphaMultiplier = -1.f;

      //
      // HSV Color tweaks
      //
      // Change the hue of all widgets (gray widgets will remain gray, since their saturation is zero). If < 0, this is ignored.
      float Hue = -1.f;
      // Multiply the saturation of all widgets (gray widgets will remain gray, since their saturation is zero). If < 0, this is ignored.
      float SaturationMultiplier = -1.f;
      // Multiply the value (luminance) of all front widgets. If < 0, this is ignored.
      float ValueMultiplierFront = -1.f;
      // Multiply the value (luminance) of all backgrounds. If < 0, this is ignored.
      float ValueMultiplierBg = -1.f;
      // Multiply the value (luminance) of text. If < 0, this is ignored.
      float ValueMultiplierText = -1.f;
      // Multiply the value (luminance) of FrameBg. If < 0, this is ignored.
      // (Background of checkbox, radio button, plot, slider, text input)
      float ValueMultiplierFrameBg = -1.f;

      ImGuiThemeTweaks() {}
    };

    struct ImGuiTweakedTheme {
      ImGuiTheme_ Theme = ImGuiTheme_DarculaDarker;
      ImGuiThemeTweaks Tweaks = ImGuiThemeTweaks();
    };

    class ThemeManager {
  public:
      static int currentThemeIDX;
      static ImGuiTweakedTheme tweakedTheme;
      static void showThemeManager(bool *p_open);
      static void applyTheme(ImGuiTheme_ theme);
    };
  }// namespace Themes
}// namespace KDB_ImGui
