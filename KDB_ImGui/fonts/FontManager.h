//
// Created by Kasper de Bruin on 24/01/2024.
//

#pragma once

#include <imgui.h>
#include <string>
#include <filesystem>

namespace HBUI {

  class FontManager {
public:
    static bool addFont(const std::filesystem::path &fontPath, const std::string& fontName, const float fontSize);
    static bool addFont(const std::filesystem::path &fontPath, const std::string& fontName, const float fontSize, const ImFontConfig &fontConfig);

    static ImFont* getFont(const std::string &fontName);
  };

}// namespace KDB_ImGui


