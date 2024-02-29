//
// Created by Kasper de Bruin on 24/02/2024.
//

//////////NOT LOADED////////////
//		    0x23FB, 0x23FE,  // IEC Power Symbols
//		    0x2665, 0x2665,  // Octicons
//		    0x26A1, 0x26A1,  // Octicons
//		    0x2B58, 0x2B58,  // IEC Power Symbols
//		    0xE000, 0xE00A,  // Pomicons
//		    0xE0A0, 0xE0A2,  // Powerline
//		    0xE0A3, 0xE0A3,  // Powerline Extra
//		    0xE0B0, 0xE0B3,  // Powerline
//		    0xE0B4, 0xE0C8,  // Powerline Extra
//		    0xE0CA, 0xE0CA,  // Powerline Extra
//		    0xE0CC, 0xE0D4,  // Powerline Extra
//		    0xE300, 0xE3E3,  // Weather Icons
//		    0xE5FA, 0xE6B1,  // Seti-UI + Custom
//		    0xEA60, 0xEBEB,  // Codicons

#define TEXT_LATIN_MIN 0x0020
#define TEXT_LATIN_MAX 0x00FF// Basic Latin + Latin Supplement

#define ICON_MIN_FA 0xf000
#define ICON_MAX_FA 0xf2e0

#define ICON_MIN_FAE 0xE200
#define ICON_MAX_FAE 0xE2A9

#define ICON_MIN_DEV 0xe700
#define ICON_MAX_DEV 0xe7c5

#define ICON_MIN_MATERIALDESIGN1 0xF500
#define ICON_MAX_MATERIALDESIGN1 0xFD46// Material Design

#define ICON_MIN_MATERIALDESIGN2 0xF0001
#define ICON_MAX_MATERIALDESIGN2 0xF1AF0// Material Design

#include <HBUI/HBUI.h>

#include <algorithm>
#include <fonts/FontLoader.h>
#include <functional>
#include <imgui.h>
#include <iterator>
#include <memory>

namespace HBUI::Fonts {
	FontLoader::FontLoader(const bool _loadDefaultFonts) {
		HBIO &hbio = HBUI::getIO();
		hbio.dpiWindowSizeFactor       = HBUI::getWindowSizeDpiScaleFactor();
		float fontSizeIncreaseFactor = 1.f;

#ifdef __APPLE__
		// Crisp fonts on macOS:
		// cf https://github.com/ocornut/imgui/issues/5301
		// Issue with macOS is that it pretends screen has 2x fewer pixels than it actually does.
		// This simplifies application development in most cases, but in our case we happen to render fonts at 1x scale
		// while screen renders at 2x scale.
		fontSizeIncreaseFactor = HBUI::getFontSizeIncreaseFactor();
#endif

		hbio.fontRenderingScale = 1.0f / fontSizeIncreaseFactor;

		ImGui::GetIO().FontGlobalScale         = hbio.fontRenderingScale;
		ImGui::GetIO().DisplayFramebufferScale = HBUI::getWindowScaleFactor();

		if (_loadDefaultFonts)
			loadDefaultFonts();
	}

	FontLoader::~FontLoader() {
	}

	void FontLoader::loadDefaultFonts() {
		ImGuiIO &io = ImGui::GetIO();

		io.Fonts->Clear();
		io.Fonts->ClearInputData();
		io.Fonts->ClearTexData();

		m_DefaultFont = loadFont(16);
		m_BigFont     = loadFont(50);

		//set default font
		io.FontDefault = m_DefaultFont;
	}

	ImFont *FontLoader::loadFont(float fontSize) {
		HBIO &hbio  = HBUI::getIO();
		ImGuiIO &io = ImGui::GetIO();

		ImWchar *defaultTextRange = new ImWchar[DefaultTextGlyphs.size()];

		for (int i = 0; i < DefaultTextGlyphs.size(); i++) {
			defaultTextRange[i] = DefaultTextGlyphs[i];
		}

		ImFontGlyphRangesBuilder builder;
		builder.AddRanges(defaultTextRange);
		static ImVector<ImWchar> *out_ranges = new ImVector<ImWchar>();
		builder.BuildRanges(out_ranges);

		ImWchar *icon = new ImWchar[out_ranges->Size];
		for (int i = 0; i < out_ranges->Size; i++) {
			icon[i] = out_ranges->Data[i];
		}

		float baseFontSize = fontSize;
		baseFontSize *= hbio.dpiWindowSizeFactor / hbio.fontRenderingScale;

		ImFontConfig baseConfig = ImFontConfig();
		baseConfig.OversampleH  = 1;
		baseConfig.OversampleV  = 1;
		fillIconRange();

		ImFont* added = io.Fonts->AddFontFromFileTTF("Assets/Fonts/test/JetBrainsMonoNerdFontPropo-Regular.ttf", baseFontSize,
		                                      &baseConfig, icon);
		io.Fonts->Build();

		return added;
	}

	void FontLoader::fillIconRange() {
		return;
	}

	ImWchar FontLoader::textToIconGlyph(const std::string &text) const {
	}
	bool FontLoader::hasIcon(const std::string &iconName, float fontSize) const {
	}


}// namespace HBUI::Fonts