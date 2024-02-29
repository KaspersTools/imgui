//
// Created by Kasper de Bruin on 22/02/2024.
//

#ifndef IMGUI_FONTLOADER_H
#define IMGUI_FONTLOADER_H
#include <set>
#include <array>

struct HBIcon {
	const std::string name;
	const ImWchar code;

	HBIcon(const std::string &name, ImWchar) : name(name), code(code) {}
};

namespace HBUI::Fonts {
	class FontLoader {
public:
		FontLoader(const bool loadDefaultFonts = true);
		~FontLoader();

		void loadDefaultFonts();
		ImFont* loadFont(float fontSize);

		void fillIconRange();//const std::vector<HBIcon> &icons§
		bool hasIcon(const std::string &iconName, float fontSize) const;
		ImWchar textToIconGlyph(const std::string &text) const;

		ImFont *getBigFont() const{
			return m_BigFont;
		}

private:
		ImFont *m_BigFont = nullptr;
		ImFont *m_DefaultFont  = nullptr;

		std::set<ImWchar> m_LoadedIcons = {};
		inline static const std::vector<ImWchar> DefaultTextGlyphs{
		    NF_ICON_space,//NF_ICON_space,
		    NF_ICON_exclam,//NF_ICON_exclam,
		    NF_ICON_quotedbl,//NF_ICON_quotedbl,
		    NF_ICON_numbersign,//NF_ICON_numbersign,
		    NF_ICON_dollar,//NF_ICON_dollar,
		    NF_ICON_percent,//NF_ICON_percent,
		    NF_ICON_ampersand,//NF_ICON_ampersand,
		    NF_ICON_quotesingle,//NF_ICON_quotesingle,
		    NF_ICON_parenleft,//NF_ICON_parenleft,
		    NF_ICON_parenright,//NF_ICON_parenright,
		    NF_ICON_asterisk,//NF_ICON_asterisk,
		    NF_ICON_plus,//NF_ICON_plus,
		    NF_ICON_comma,//NF_ICON_comma,
		    NF_ICON_hyphen,//NF_ICON_hyphen,
		    NF_ICON_period,//NF_ICON_period,
		    NF_ICON_slash,//NF_ICON_slash,
		    NF_ICON_zero,//NF_ICON_zero,
		    NF_ICON_one,//NF_ICON_one,
		    NF_ICON_two,//NF_ICON_two,
		    NF_ICON_three,//NF_ICON_three,
		    NF_ICON_four,//NF_ICON_four,
		    NF_ICON_five,//NF_ICON_five,
		    NF_ICON_six,//NF_ICON_six,
		    NF_ICON_seven,//NF_ICON_seven,
		    NF_ICON_eight,//NF_ICON_eight,
		    NF_ICON_nine,//NF_ICON_nine,
		    NF_ICON_colon,//NF_ICON_colon,
		    NF_ICON_semicolon,//NF_ICON_semicolon,
		    NF_ICON_less,//NF_ICON_less,
		    NF_ICON_equal,//NF_ICON_equal,
		    NF_ICON_greater,//NF_ICON_greater,
		    NF_ICON_question,//NF_ICON_question,
		    NF_ICON_at,//NF_ICON_at,
		    NF_ICON_A,//NF_ICON_A,
		    NF_ICON_B,//NF_ICON_B,
		    NF_ICON_C,//NF_ICON_C,
		    NF_ICON_D,//NF_ICON_D,
		    NF_ICON_E,//NF_ICON_E,
		    NF_ICON_F,//NF_ICON_F,
		    NF_ICON_G,//NF_ICON_G,
		    NF_ICON_H,//NF_ICON_H,
		    NF_ICON_I,//NF_ICON_I,
		    NF_ICON_J,//NF_ICON_J,
		    NF_ICON_K,//NF_ICON_K,
		    NF_ICON_L,//NF_ICON_L,
		    NF_ICON_M,//NF_ICON_M,
		    NF_ICON_N,//NF_ICON_N,
		    NF_ICON_O,//NF_ICON_O,
		    NF_ICON_P,//NF_ICON_P,
		    NF_ICON_Q,//NF_ICON_Q,
		    NF_ICON_R,//NF_ICON_R,
		    NF_ICON_S,//NF_ICON_S,
		    NF_ICON_T,//NF_ICON_T,
		    NF_ICON_U,//NF_ICON_U,
		    NF_ICON_V,//NF_ICON_V,
		    NF_ICON_W,//NF_ICON_W,
		    NF_ICON_X,//NF_ICON_X,
		    NF_ICON_Y,//NF_ICON_Y,
		    NF_ICON_Z,//NF_ICON_Z,
		    NF_ICON_bracketleft,//NF_ICON_bracketleft,
		    NF_ICON_backslash,//NF_ICON_backslash,
		    NF_ICON_bracketright,//NF_ICON_bracketright,
		    NF_ICON_asciicircum,//NF_ICON_asciicircum,
		    NF_ICON_underscore,//NF_ICON_underscore,
		    NF_ICON_grave,//NF_ICON_grave,
		    NF_ICON_a,//NF_ICON_a,
		    NF_ICON_b,//NF_ICON_b,
		    NF_ICON_c,//NF_ICON_c,
		    NF_ICON_d,//NF_ICON_d,
		    NF_ICON_e,//NF_ICON_e,
		    NF_ICON_f,//NF_ICON_f,
		    NF_ICON_g,//NF_ICON_g,
		    NF_ICON_h,//NF_ICON_h,
		    NF_ICON_i,//NF_ICON_i,
		    NF_ICON_j,//NF_ICON_j,
		    NF_ICON_k,//NF_ICON_k,
		    NF_ICON_l,//NF_ICON_l,
		    NF_ICON_m,//NF_ICON_m,
		    NF_ICON_n,//NF_ICON_n,
		    NF_ICON_o,//NF_ICON_o,
		    NF_ICON_p,//NF_ICON_p,
		    NF_ICON_q,//NF_ICON_q,
		    NF_ICON_r,//NF_ICON_r,
		    NF_ICON_s,//NF_ICON_s,
		    NF_ICON_t,//NF_ICON_t,
		    NF_ICON_u,//NF_ICON_u,
		    NF_ICON_v,//NF_ICON_v,
		    NF_ICON_w,//NF_ICON_w,
		    NF_ICON_x,//NF_ICON_x,
		    NF_ICON_y,//NF_ICON_y,
		    NF_ICON_z,//NF_ICON_z,
		    NF_ICON_braceleft,//NF_ICON_braceleft,
		    NF_ICON_bar,//NF_ICON_bar,
		    NF_ICON_braceright,//NF_ICON_braceright,
		    NF_ICON_asciitilde,//NF_ICON_asciitilde,
		    NF_ICON_uni00A0,//NF_ICON_uni00A0,
		    NF_ICON_exclamdown,//NF_ICON_exclamdown,
		    NF_ICON_cent,//NF_ICON_cent,
		    NF_ICON_sterling,//NF_ICON_sterling,
		    NF_ICON_currency,//NF_ICON_currency,
		    NF_ICON_yen,//NF_ICON_yen,
		    NF_ICON_brokenbar,//NF_ICON_brokenbar,
		    NF_ICON_section,//NF_ICON_section,
		    NF_ICON_dieresis,//NF_ICON_dieresis,
		    NF_ICON_copyright,//NF_ICON_copyright,
		    NF_ICON_ordfeminine,//NF_ICON_ordfeminine,
		    NF_ICON_guillemotleft,//NF_ICON_guillemotleft,
		    NF_ICON_logicalnot,//NF_ICON_logicalnot,
		    NF_ICON_uni00AD,//NF_ICON_uni00AD,
		    NF_ICON_registered,//NF_ICON_registered,
		    NF_ICON_macron,//NF_ICON_macron,
		    NF_ICON_degree,//NF_ICON_degree,
		    NF_ICON_plusminus,//NF_ICON_plusminus,
		    NF_ICON_uni00B2,//NF_ICON_uni00B2,
		    NF_ICON_uni00B3,//NF_ICON_uni00B3,
		    NF_ICON_acute,//NF_ICON_acute,
		    NF_ICON_uni00B5,//NF_ICON_uni00B5,
		    NF_ICON_paragraph,//NF_ICON_paragraph,
		    NF_ICON_periodcentered,//NF_ICON_periodcentered,
		    NF_ICON_cedilla,//NF_ICON_cedilla,
		    NF_ICON_uni00B9,//NF_ICON_uni00B9,
		    NF_ICON_ordmasculine,//NF_ICON_ordmasculine,
		    NF_ICON_guillemotright,//NF_ICON_guillemotright,
		    NF_ICON_onequarter,//NF_ICON_onequarter,
		    NF_ICON_onehalf,//NF_ICON_onehalf,
		    NF_ICON_threequarters,//NF_ICON_threequarters,
		    NF_ICON_questiondown,//NF_ICON_questiondown,
		    NF_ICON_Agrave,//NF_ICON_Agrave,
		    NF_ICON_Aacute,//NF_ICON_Aacute,
		    NF_ICON_Acircumflex,//NF_ICON_Acircumflex,
		    NF_ICON_Atilde,//NF_ICON_Atilde,
		    NF_ICON_Adieresis,//NF_ICON_Adieresis,
		    NF_ICON_Aring,//NF_ICON_Aring,
		    NF_ICON_AE,//NF_ICON_AE,
		    NF_ICON_Ccedilla,//NF_ICON_Ccedilla,
		    NF_ICON_Egrave,//NF_ICON_Egrave,
		    NF_ICON_Eacute,//NF_ICON_Eacute,
		    NF_ICON_Ecircumflex,//NF_ICON_Ecircumflex,
		    NF_ICON_Edieresis,//NF_ICON_Edieresis,
		    NF_ICON_Igrave,//NF_ICON_Igrave,
		    NF_ICON_Iacute,//NF_ICON_Iacute,
		    NF_ICON_Icircumflex,//NF_ICON_Icircumflex,
		    NF_ICON_Idieresis,//NF_ICON_Idieresis,
		    NF_ICON_Eth,//NF_ICON_Eth,
		    NF_ICON_Ntilde,//NF_ICON_Ntilde,
		    NF_ICON_Ograve,//NF_ICON_Ograve,
		    NF_ICON_Oacute,//NF_ICON_Oacute,
		    NF_ICON_Ocircumflex,//NF_ICON_Ocircumflex,
		    NF_ICON_Otilde,//NF_ICON_Otilde,
		    NF_ICON_Odieresis,//NF_ICON_Odieresis,
		    NF_ICON_multiply,//NF_ICON_multiply,
		    NF_ICON_Oslash,//NF_ICON_Oslash,
		    NF_ICON_Ugrave,//NF_ICON_Ugrave,
		    NF_ICON_Uacute,//NF_ICON_Uacute,
		    NF_ICON_Ucircumflex,//NF_ICON_Ucircumflex,
		    NF_ICON_Udieresis,//NF_ICON_Udieresis,
		    NF_ICON_Yacute,//NF_ICON_Yacute,
		    NF_ICON_Thorn,//NF_ICON_Thorn,
		    NF_ICON_germandbls,//NF_ICON_germandbls,
		    NF_ICON_agrave,//NF_ICON_agrave,
		    NF_ICON_aacute,//NF_ICON_aacute,
		    NF_ICON_acircumflex,//NF_ICON_acircumflex,
		    NF_ICON_atilde,//NF_ICON_atilde,
		    NF_ICON_adieresis,//NF_ICON_adieresis,
		    NF_ICON_aring,//NF_ICON_aring,
		    NF_ICON_ae,//NF_ICON_ae,
		    NF_ICON_ccedilla,//NF_ICON_ccedilla,
		    NF_ICON_egrave,//NF_ICON_egrave,
		    NF_ICON_eacute,//NF_ICON_eacute,
		    NF_ICON_ecircumflex,//NF_ICON_ecircumflex,
		    NF_ICON_edieresis,//NF_ICON_edieresis,
		    NF_ICON_igrave,//NF_ICON_igrave,
		    NF_ICON_iacute,//NF_ICON_iacute,
		    NF_ICON_icircumflex,//NF_ICON_icircumflex,
		    NF_ICON_idieresis,//NF_ICON_idieresis,
		    NF_ICON_eth,//NF_ICON_eth,
		    NF_ICON_ntilde,//NF_ICON_ntilde,
		    NF_ICON_ograve,//NF_ICON_ograve,
		    NF_ICON_oacute,//NF_ICON_oacute,
		    NF_ICON_ocircumflex,//NF_ICON_ocircumflex,
		    NF_ICON_otilde,//NF_ICON_otilde,
		    NF_ICON_odieresis,//NF_ICON_odieresis,
		    NF_ICON_divide,//NF_ICON_divide,
		    NF_ICON_oslash,//NF_ICON_oslash,
		    NF_ICON_ugrave,//NF_ICON_ugrave,
		    NF_ICON_uacute,//NF_ICON_uacute,
		    NF_ICON_ucircumflex,//NF_ICON_ucircumfwlex,
		    NF_ICON_udieresis,//NF_ICON_udieresis,
		    NF_ICON_yacute,//NF_ICON_yacute,
		    NF_ICON_thorn,//NF_ICON_thorn,
		    NF_ICON_ydieresis,0,//NF_ICON_ydieresis,

		    NF_ICON_github,NF_ICON_github,
		    0
		};
	};
}// namespace HBUI::Fonts


#endif//IMGUI_FONTLOADER_H