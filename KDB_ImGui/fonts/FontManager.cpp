//
// Created by Kasper de Bruin on 24/01/2024.
//

#include "FontManager.h"
#include <iostream>
#include <unordered_map>

namespace HBUI {
    static std::unordered_map<std::string, ImFont *> fonts;

    bool
    FontManager::addFont(const std::filesystem::path &fontPath, const std::string &fontName, const float fontSize) {
        return addFont(fontPath, fontName, fontSize, ImFontConfig());
    }

    bool FontManager::addFont(const std::filesystem::path &fontPath, const std::string &fontName, const float fontSize,
                              const ImFontConfig &fontConfig) {
        if(!std::filesystem::exists(fontPath)) {
            std::cerr << "Font not found: " << fontPath << std::endl;
            return false;
        }
        if (fonts.find(fontName) != fonts.end()) {
            std::cerr << "Font already loaded, or another that you are trying to give the same name: " << fontName << std::endl;
            return false;
        }
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