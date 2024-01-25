//
// Created by Kasper de Bruin on 24/01/2024.
//

#ifndef IMGUI_FONTMANAGER_H
#define IMGUI_FONTMANAGER_H

#include <imgui.h>
#include <string>
#include <filesystem>

namespace KDB_ImGui {

  class FontManager {
private:
    FontManager();
    ~FontManager();
public:
    bool addFont(const std::filesystem::path &fontPath, const float fontSize);
    bool addFont(const std::filesystem::path &fontPath, const float fontSize, const ImFontConfig &fontConfig);

    ImFont* getFont(const std::string &fontName);
    static FontManager* getInstance();
  };

}// namespace KDB_ImGui

#endif//IMGUI_FONTMANAGER_H
