//
// Created by Kasper de Bruin on 01/03/2024.
//

#ifndef IMGUI_HBWIDGETCOLORPROPERTIES_H
#define IMGUI_HBWIDGETCOLORPROPERTIES_H

#include <HBUI/HBUI.h>

namespace HBUI {
	namespace Builder {
		class HBWidgetColorPropertiesBuilder;
	};// namespace Builder

	namespace Properties {
		struct [[maybe_unused]] WidgetColorProperties {
			friend class HBUI::Builder::HBWidgetColorPropertiesBuilder;

	private:
			//		friend class Builder::HBWidgetColorPropertiesBuilder;
			[[maybe_unused]] WidgetColorProperties() = default;
			[[maybe_unused]] [[maybe_unused]] WidgetColorProperties(const ImColor &backgroundColor,
			                                                        const ImColor &hoverColor,
			                                                        const ImColor &textColor,
			                                                        const ImColor &borderColor,
			                                                        const bool withBackground       = true,
			                                                        const HBDrawFlags_ drawLocation = HBDrawFlags_OnParent,
			                                                        const bool withBorder           = false);

	public:
			const bool isSet() const {
				if (m_DrawLocation == HBDrawFlags_None) { return false; }
				return m_IsSet;
			}
			const HBDrawFlags_ getDrawLocation() const {
				return m_DrawLocation;
			}

	private:
			bool m_IsSet = false;

	protected:
			ImColor m_TextColor   = ImColor(0, 255, 0, 255);
			ImColor m_BorderColor = ImColor(0, 0, 255, 255);
			ImColor m_HoverColor  = ImColor(140, 140, 140, 255);

			bool m_WithBackground = true;
			bool m_WithBorder     = true;

			HBDrawFlags_ m_DrawLocation = HBDrawFlags_None;

			HBColorPropertiesFlags m_Flags = HBColorPropertiesFlags_Default;
			ImColor m_BackgroundColor      = ImColor(255, 0, 0, 255);
		};
	};  // namespace Properties
};    // namespace HBUI
#endif//IMGUI_HBWIDGETCOLORPROPERTIES_H
