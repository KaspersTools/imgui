//
// Created by Kasper de Bruin on 12/01/2024.
//
/////COPYRIGHT NOTICE
//
// Theme tweak utilities for ImGui.
// Reuse and adaptation of imgui_theme.h and imgui_theme.cpp file is granted for other projects
// provided the origin of those files is stated in the copied version.
// Some themes were adapted by themes posted by ImGui users at https://github.com/ocornut/imgui/issues/707
//// Part of ImGui Bundle - MIT License - Copyright (c) 2022-2024 Pascal Thomet - https://github.com/pthom/imgui_bundle

#pragma once

#include "ThemeManager.h"
#include <string>

namespace KDB_ImGui {
  namespace Themes {

    const char *ImGuiTheme_Name(ImGuiTheme_ theme);

    ImGuiTheme_ ImGuiTheme_FromName(const char *themeName);

    ImGuiStyle ThemeToStyle(ImGuiTheme_ theme);

    ImGuiStyle TweakedThemeThemeToStyle(const ImGuiTweakedTheme &tweaked_theme);

    void ApplyTweakedTheme(const ImGuiTweakedTheme &tweaked_theme);

    // Show the theme selection listbox, the theme tweak widgets, as well as ImGui::ShowStyleEditor. Returns true if modified (Warning, when using ShowStyleEditor, no info about modification is transmitted)
    bool ShowThemeTweakGui(ImGuiTweakedTheme *tweaked_theme);

    // Some tweakable themes
    ImGuiStyle SoDark(float hue);

    ImGuiStyle
    ShadesOfGray(float rounding = 0.f, float value_multiplier_front = 1.f, float value_multiplier_bg = 1.f);

    ImGuiStyle Darcula(
            float rounding = 1.f,
            float hue = -1.f,
            float saturation_multiplier = 1.f,
            float value_multiplier_front = 1.f,
            float value_multiplier_bg = 1.f,
            float alpha_bg_transparency = 1.f);
  };// namespace Themes
}// namespace KDB_ImGui
