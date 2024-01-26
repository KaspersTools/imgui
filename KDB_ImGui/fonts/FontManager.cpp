//
// Created by Kasper de Bruin on 24/01/2024.
//

#include "FontManager.h"
#include <iostream>
#include <unordered_map>

namespace KDB_ImGui {
  static std::unordered_map<std::string, ImFont *> fonts;

  bool FontManager::addFont(const std::filesystem::path &fontPath, const std::string& fontName, const float fontSize) {
    return addFont(fontPath, fontName, fontSize, ImFontConfig());
  }

  bool FontManager::addFont(const std::filesystem::path &fontPath, const std::string &fontName, const float fontSize, const ImFontConfig &fontConfig) {
    ImFont *font = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, &fontConfig);
    if (font == nullptr) {
      std::cerr << "Failed to load font: " << fontPath << std::endl;
      return false;
    }

    fonts[fontName] = font;
  }

  ImFont *FontManager::getFont(const std::string &fontName) {
    if (fonts.find(fontName) == fonts.end()) {
      std::cerr << "Font not found: " << fontName << std::endl;
      return nullptr;
    }
    return fonts[fontName];
  }
}// namespace KDB_ImGui