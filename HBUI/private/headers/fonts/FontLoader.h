//
// Created by Kasper de Bruin on 22/02/2024.
//

#ifndef IMGUI_FONTLOADER_H
#define IMGUI_FONTLOADER_H


#include <array>
#include <filesystem>
#include <unordered_map>

#include <fonts/IconsFontAwesome6.h>
#include <fonts/IconsFontAwesome6Brands.h>

using ImWcharPair = std::array<ImWchar, 2>;

inline static void StoreStaticGlyphRange(ImFontConfig *font_cfg, const ImVector<ImWchar> &glyph_ranges = {}) {
	// *Static* storage for pointers that will be used by ImGui::GetIO().Fonts->AddFontFromMemoryTTF, and which are required to persist until the Font Atlas is built, which happens much later after calling AddFontFromXXX
	//
	//                  *Open question: should the storage use "static" or "thread_local"?*
	//
	static ImVector<ImVector<ImWchar>> all_glyph_ranges;// This is kept alive, until the app exits. However, it is small.
	constexpr int max_size = 100;                       // Arbitrary limit: if you load more than 100 fonts, you may want to increase this number
	bool first_time        = all_glyph_ranges.Capacity == 0;

	if (first_time) {
		// Make sure that our static vector will not be moved in memory, unless the user load more than 100 fonts
		all_glyph_ranges.reserve(max_size);
	}

	// Add font config to static storage
	if (glyph_ranges.empty())
		font_cfg->GlyphRanges = nullptr;
	else {
		all_glyph_ranges.push_back({});
		ImVector<ImWchar> &glyph_ranges_static = all_glyph_ranges.back();
		glyph_ranges_static                    = glyph_ranges;
		font_cfg->GlyphRanges                  = glyph_ranges_static.Data;
	}

	IM_ASSERT(all_glyph_ranges.Size <= max_size && "Too many fonts loaded");
}

inline ImFont *AddFontFromFileTTF_2(const char *filename, float font_size_pixels, ImFontConfig *font_cfg = NULL, const ImVector<ImWchar> &glyph_ranges = {}) {
	if (font_cfg != NULL && !glyph_ranges.empty())
		StoreStaticGlyphRange(font_cfg, glyph_ranges);
	return ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, font_size_pixels, font_cfg);
//	return ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, font_size_pixels, font_cfg);
}

struct ImFont;
struct HBFontIdentifier {
	const std::string name;
	const float size;

	explicit HBFontIdentifier(const std::string &name, float size) : name(name), size(size) {
	}

	~HBFontIdentifier() = default;

	// Overload the == operator for equality comparison
	bool operator==(const HBFontIdentifier &other) const {
		return name == other.name && size == other.size;
	}

	// Define the hash function within the struct
	struct hash {
		size_t operator()(const HBFontIdentifier &id) const {
			return std::hash<std::string>()(id.name) ^ (std::hash<float>()(id.size) << 1);
		}
	};
};
struct HBFont {
	float fontSize;                                       // Current size of the font
	ImFontConfig fontConfig              = ImFontConfig();// Configuration for the font
	std::vector<ImWcharPair> glyphRanges = {};            // Glyph ranges

	const HBFontStyle_ style;// Style of the font

	const std::string fontPath   = "";   // Path to the font file
	const bool mergeToLastFont   = false;// Merge to the last font
	const bool useFullGlyphRange = false;// Load glyphs

	const ImFont *imguiFont = nullptr;// Pointer to the ImGui font

	HBFont(const std::string &path, float size,
	       HBFontStyle_ style, const ImFontConfig &config = {},
	       const bool mergeToLastFont                  = false,
	       const bool useFullGlyphRange                = false,
	       const std::vector<ImWcharPair> &glyphRanges = {})
	    : fontSize(size),
	      fontConfig(config),
	      glyphRanges(glyphRanges),
	      style(style),
	      fontPath(path),
	      mergeToLastFont(mergeToLastFont), useFullGlyphRange(useFullGlyphRange) {
	}

	// Load the font with the current settings
	ImFont *loadFont() {
		IM_ASSERT(fontPath != "");// Make sure the font path is not empty
		IM_ASSERT(fontSize > 0);  // Make sure the font size is valid

		ImGuiIO &imIo = ImGui::GetIO();

				HBIO &hbio = HBUI::getIO();
				fontSize *= hbio.dpiWindowSizeFactor / hbio.fontRenderingScale;

		if (imguiFont != nullptr) {
			// Unload the font if it's already loaded
			imIo.Fonts->Clear();
			imguiFont = nullptr;
		}

		if (useFullGlyphRange) {
			glyphRanges.clear();
			glyphRanges.push_back({0x0001, 0xFFFF});
		}

		fontConfig.MergeMode = mergeToLastFont;
		fontConfig.OversampleH = fontConfig.OversampleV = 1;

		ImFont *font = nullptr;

		// Populate the glyph ranges for ImGui:
		// 2 value per range, values are inclusive, zero-terminated list
		ImVector<ImWchar> glyphRangesImVector;
		if (!glyphRanges.empty()) {
			for (const auto &glyph_range_interval: glyphRanges) {
				glyphRangesImVector.push_back(glyph_range_interval[0]);
				glyphRangesImVector.push_back(glyph_range_interval[1]);
			}
			glyphRangesImVector.push_back(0);// Zero-terminate the array
		}

		font = AddFontFromFileTTF_2(
		    fontPath.c_str(), fontSize, &fontConfig, glyphRangesImVector);

		return font;
	}

	// Change the font size and reload
	bool changeFontSize(float newSize) {
		if (newSize == fontSize) {
			return true;// No change needed
		}

		fontSize = newSize;
		return loadFont();// Reload the font with the new size
	}
};

struct HBFontConfig {
	HBFontConfig() {
		defaultImGUIFontConfig                    = ImFontConfig();
		defaultImGUIFontConfig.RasterizerDensity  = 1.0f;
		defaultImGUIFontConfig.RasterizerMultiply = 1.0f;

		defaultImGUIIconFontConfig            = ImFontConfig();
		defaultImGUIIconFontConfig.MergeMode  = true;
		defaultImGUIIconFontConfig.PixelSnapH = true;
	}

	const int c_MaxFonts          = 10;
	const int c_MaxFontNameLength = 32;

	const HBFontStyle_ c_DefaultFontStyle = HBFontStyle_Regular;

	//sizes
	const float c_MinFontSize = 8;
	const float c_MaxFontSize = 42;

	float defaultFontSize = 15;
	float defaultIconSize = 15;

	ImFontConfig defaultImGUIFontConfig;
	ImFontConfig defaultImGUIIconFontConfig;


	const HBFont getDefaultFont() const {
		return HBFont("assets/fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf",
		              defaultFontSize,
		              c_DefaultFontStyle,
		              defaultImGUIFontConfig);
	}

	const HBFont getDefaultIconFont(const std::string& filename) const {
		return HBFont(filename,
		              defaultIconSize,
		              c_DefaultFontStyle,
		              defaultImGUIIconFontConfig,
		              true,
		              true);
	}
};

namespace HBUI::Fonts {
	class FontLoader {
public:
		FontLoader(const bool loadDefaulHBUItFont = false, const bool loadFA6 = false);
		~FontLoader();

		void loadDefaultFonts(const bool withFontAwesome = true);

		void loadFont(const std::string &fontId, HBFont &font);

		ImFont *AddFontFromFileTTF_2(const char *filename, float font_size_pixels, ImFontConfig *font_cfg = NULL, const ImVector<ImWchar> &glyph_ranges = {}) {
			if (font_cfg != NULL && !glyph_ranges.empty())
				StoreStaticGlyphRange(font_cfg, glyph_ranges);
			return ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, font_size_pixels, font_cfg);
		}

		void renderDebugWindow();

private:
		std::unordered_map<HBFontIdentifier, std::shared_ptr<HBFont>, HBFontIdentifier::hash> fonts;
	};
}// namespace HBUI::Fonts


#endif//IMGUI_FONTLOADER_H