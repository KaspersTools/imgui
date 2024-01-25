//
// Created by Kasper de Bruin on 24/01/2024.
//

#include "FontManager.h"
#include <iostream>
#include <unordered_map>

namespace KDB_ImGui {
  static FontManager *instance = nullptr;
  static std::unordered_map<std::string, ImFont *> fonts;

  FontManager::FontManager() {
    if (instance != nullptr) {
      //todo: logging
      std::cerr << "FontManager already exists!" << std::endl;
      return;
    }
    instance = this;
  }

  FontManager::~FontManager() {
    delete instance;
  }

  bool FontManager::addFont(const std::filesystem::path &fontPath, const float fontSize) {
    return addFont(fontPath, fontSize, ImFontConfig());
  }

  bool FontManager::addFont(const std::filesystem::path &fontPath, const float fontSize, const ImFontConfig &fontConfig) {
    ImFont *font = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, &fontConfig);
    if (font == nullptr) {
      std::cerr << "Failed to load font: " << fontPath << std::endl;
      return false;
    }

    fonts[fontPath] = font;
  }

  ImFont *FontManager::getFont(const std::string &fontName) {
    if (fonts.find(fontName) == fonts.end()) {
      std::cerr << "Font not found: " << fontName << std::endl;
      return nullptr;
    }
    return fonts[fontName];
  }


  FontManager *FontManager::getInstance() {
    if (instance == nullptr) {
      instance = new FontManager();
    }
    return instance;
  }
}// namespace KDB_ImGui