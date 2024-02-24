//
// Created by Kasper de Bruin on 24/02/2024.
//

//ImFontConfig cfg;
//cfg.RasterizerDensity  = 2.0f;
//cfg.RasterizerMultiply = 1.0f;
//const float baseFontSize = 16.0f; // 13.0f is the size of the default font. Change to the font size you use.
//imIo.Fonts->AddFontFromFileTTF("assets/fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf", baseFontSize, &cfg);
//ImFontConfig icons_config; // This is the trick to load the icons font
//icons_config.MergeMode = true;
//icons_config.PixelSnapH = true;
//icons::Font::Load(ImGui::GetIO(), baseFontSize, &icons_config);


#ifndef HB_DefaultFontIconsLoader
#define HB_DefaultFontIconsLoader "fa-brands/fontAwesome_loader.hpp"
#endif
#ifndef HB_DefaultFontIconsCodes
#define HB_DefaultFontIconsCodes "fa-brands/fontAwesome_codes.hpp"
#endif

#include HB_DefaultFontIconsLoader
#include HB_DefaultFontIconsCodes

#include <fonts/FontLoader.h>
#include <imgui.h>

namespace HBUI::Fonts {

	FontLoader::FontLoader(const bool loadTheDefaultFonts, const bool loadTheDefaultIcons) {
		if (loadTheDefaultFonts) {
			loadDefaultFonts();
		}

		if(loadTheDefaultIcons) {
			loadDefaultIcons();
		}
	}

	FontLoader::~FontLoader() {
	}

	void FontLoader::loadDefaultFonts() {
		loadFont(m_Config.getDefaultFont());
	}

	void FontLoader::loadDefaultIcons() {
		loadIcons();
	}

	void FontLoader::loadIcons() {
		// Then merge the icons
		ImFontConfig cfg;
		cfg.MergeMode = true;
		cfg.PixelSnapH = true;
		icons::Font::Load(ImGui::GetIO(), 40w, &cfg);//fixme use current font size
	}

	void FontLoader::loadFont(const HBFont &font) {
		loadFont(font.name, font.path, font.size, font.type, font.imFontConfig, true);
	}

	void FontLoader::loadFont(const std::string &fontName, const std::filesystem::path& path,
	                          const int fontSize, const HBFontType type, const ImFontConfig &config, const bool makeDefault) {
		IM_ASSERT(!fontName.empty() && "Font name cannot be empty");
		IM_ASSERT(!path.empty() && "Font path cannot be empty");
		IM_ASSERT(fontSize > 0 && "Font size must be greater than 0");

		IM_ASSERT(m_Fonts.find(fontName) == m_Fonts.end() && "Font already loaded");
		IM_ASSERT(m_Fonts.size() < m_Config.c_MaxFonts && "Maximum number of fonts loaded");

		auto io = ImGui::GetIO();
		auto fontImGui = io.Fonts->AddFontFromFileTTF(path.string().c_str(), fontSize, &config);
		IM_ASSERT(fontImGui != nullptr && "Failed to load font");

		HBFont font = HBFont(path, fontName, fontSize, type, config);
		m_Fonts[fontName] = std::make_shared<HBFont>(font);

		if(makeDefault){
			if(m_CurrentFont != nullptr){popFont();}
			ImGui::GetIO().FontDefault = fontImGui;
			m_CurrentFont = m_Fonts[fontName];
		}
	}

	void FontLoader::pushFont(const std::string& fontName){
		auto font = m_Fonts.find(fontName);
		IM_ASSERT(font != m_Fonts.end() && "Font not found");
		ImGui::PushFont(font->second->font);
		m_CurrentFont = font->second;
	}

	void FontLoader::popFont(){
		IM_ASSERT(m_CurrentFont == nullptr && "No font to pop");
		ImGui::PopFont();
	}
}// namespace HBUI::Fonts
