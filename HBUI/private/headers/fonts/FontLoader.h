//
// Created by Kasper de Bruin on 22/02/2024.
//

#ifndef IMGUI_FONTLOADER_H
#define IMGUI_FONTLOADER_H
#include <array>
#include <set>
#include <utility>
#include <vector>

namespace HBUI::Fonts {

  struct HBIcon {
    std::string name;
    ImWchar     glyph;
  };

  struct LoadFontData {
    HBLoadFontFlags flags;
    std::string     name;
    std::string     path;
    float           size;
  };

  struct FontBuilder {
    LoadFontData data;
    explicit FontBuilder(HBLoadFontFlags flags, std::string name, std::string path, float size) : data({flags, std::move(name), path, size}) {}
    LoadFontData *build();
  };


  struct HBFont {
    ImFont      *font;
    LoadFontData data;

    explicit HBFont(ImFont *font, LoadFontData data) : font(font), data(std::move(data)) {}
    static HBIcon *getIcon(const std::string &name);

    static std::vector<HBIcon>  DEFAULT_ICONS;
    static std::vector<ImWchar> DEFAULT_TEXT_GLYPHS;

    static std::vector<std::string> getDefaultIconNames();
  };
  class FontLoader {
  public:
    explicit FontLoader(bool loadDefaultFonts = true);
    ~FontLoader();

    void    loadDefaultFonts();
    HBFont *loadFont(const LoadFontData &data);

    HBFont *activateFont(float fontSize, const std::string &fontName, HBActivateFontFlags flags);
    HBFont *activateFont(HBFont* font);

    HBFont *activateBigFont();
    HBFont *activateDefaultFont();
    void    activateFontSize(float fontSize);

    inline static ImGuiID getId(const std::string &name, float fontSize) {
      const std::string fontIdStr = std::to_string((float) fontSize) + name;
      const char       *str       = fontIdStr.c_str();
      const char       *str_end   = str + fontIdStr.size();
      ImGuiID           id        = ImHashStr(str, str_end ? (str_end - str) : 0, 0);
      return id;
    }

    [[nodiscard]] HBFont *getActiveFont() const {
      return activeFont;
    }

    [[nodiscard]] HBFont *getDefaultFont() const {
      return m_DefaultFont;
    }

    [[nodiscard]] HBFont *getBigFont() const {
      return m_BigFont;
    }

    [[nodiscard]] HBFont *getFont(float fontSize, HBLoadFontFlags flags) {
      if (fontSize == m_DefaultFont->data.size)
        return m_DefaultFont;
      if (fontSize == m_BigFont->data.size)
        return m_BigFont;
    }

  private:
    std::map<ImGuiID, HBFont *> m_LoadedFonts = std::map<ImGuiID, HBFont *>();// Map of loaded fonts

    HBFont *activeFont    = nullptr;
    HBFont *m_BigFont     = nullptr;
    HBFont *m_DefaultFont = nullptr;

    std::vector<LoadFontData> m_LoadDataNextFrame = {};
  };
}// namespace HBUI::Fonts


#endif//IMGUI_FONTLOADER_H