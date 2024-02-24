//
// Created by Kasper de Bruin on 22/02/2024.
//

#ifndef IMGUI_FONTLOADER_H
#define IMGUI_FONTLOADER_H


#include <filesystem>
#include <unordered_map>

struct ImFont;

enum HBFontType {
	HBFontType_Regular,
	HBFontType_Bold,
	HBFontType_Italic,
	HBFontType_BoldItalic
};

struct HBFont {
	const std::string name;
	const std::filesystem::path path;
	const int size;
	const HBFontType type;
	const ImFontConfig imFontConfig;

	HBFont(const std::string &name, const std::filesystem::path &path, int size, HBFontType type, ImFontConfig imFontConfig)
	    : name(name), path(path), size(size), type(type), imFontConfig(imFontConfig) {
	}

	~HBFont() {
		if (font)
			delete font;
	}

	ImFont *font;
};

struct HBIcon{

};

struct HBFontConfig {
	HBFontConfig() {
		defaultImGUIFontConfig                    = ImFontConfig();
		defaultImGUIFontConfig.RasterizerDensity  = 2.0f;
		defaultImGUIFontConfig.RasterizerMultiply = 1.0f;

		defaultImGUIIconFontConfig            = ImFontConfig();
		defaultImGUIIconFontConfig.MergeMode  = true;
		defaultImGUIIconFontConfig.PixelSnapH = true;
	}

	const int c_MaxFonts          = 10;
	const int c_MaxFontNameLength = 32;

	//sizes
	const int c_MinFontSize = 8;
	const int c_MaxFontSize = 42;

	ImFontConfig defaultImGUIFontConfig;
	ImFontConfig defaultImGUIIconFontConfig;

	int defaultFontSize        = 24;
	int defaultIconSize        = 24;
	HBFontType defaultFontType = HBFontType_Regular;

	const HBFont getDefaultFont() const {
		HBFont font(
		    "JetBrainsMonoNerdFontPropo-Regular",
		    "assets/fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf",
		    defaultFontSize,
		    HBFontType_Regular,
		    defaultImGUIFontConfig);
		return font;
	}

	ImFontConfig getDefaulImGUIIconFontConfig() const {
		return defaultImGUIIconFontConfig;
	}
};

namespace HBUI::Fonts {
	class FontLoader {
public:
		FontLoader(const bool loadDefaultFonts = true, const bool loadDefaultIcons = true);
		~FontLoader();


		void loadDefaultFonts();
		void loadDefaultIcons();

		void loadFont(const HBFont &font);
		void loadFont(const std::string &fontName, const std::filesystem::path &path,
		              const int fontSize, const HBFontType type, const ImFontConfig &config, const bool makeDefault);

		void loadIcons();
		void pushFont(const std::string &name);
		void popFont();
		/*
				void loadDefaultFont(const std::string &name, const std::filesystem::path &path, int size, HBFontType type);
				void loadDefaultFont(const HBFont &font);

				void loadFont(const std::string &name, const std::filesystem::path &path, int size, HBFontType type);
				void loadFont(const HBFont &font);


				void unloadFont(const std::string &name);
				void unloadFont(const HBFont &font);
				void unloadAllFonts();

				ImFont* getFont(const std::string &name);
				ImFont* getFont(const HBFont &font);
				ImFont* getDefaultFont(const std::string &name);
				ImFont* getDefaultFont(const HBFont &font);

				void setDefaultFont(const std::string &name, const std::filesystem::path &path, int size, HBFontType type);
				void setDefaultFont(const std::string &name);
				void setDefaultFont(const HBFont &font);

				void pushFont(const std::string &name);
				void pushFont(const HBFont &font);

				void popFont();
			 */
private:
		std::shared_ptr<HBFont> m_DefaultFont = nullptr;
		std::shared_ptr<HBFont> m_CurrentFont = nullptr;

		std::unordered_map<std::string, std::shared_ptr<HBFont>> m_Fonts;

		HBFontConfig m_Config = HBFontConfig();
	};
}// namespace HBUI::Fonts


#endif//IMGUI_FONTLOADER_H