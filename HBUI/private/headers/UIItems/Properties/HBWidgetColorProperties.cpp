//
// Created by Kasper de Bruin on 04/03/2024.
//

// clang-format off

#include <UIItems/Builders/HBWidgetColorPropertiesBuilder.h>

#include "HBWidgetColorProperties.h"

// clang-format on

namespace HBUI {
	namespace Properties {
		[[maybe_unused]] WidgetColorProperties::WidgetColorProperties(
		    const ImColor &backgroundColor,
		    const ImColor &hoverColor,
		    const ImColor &textColor,
		    const ImColor &borderColor,
		    const bool withBackground,
		    const HBDrawFlags_ drawLocation,
		    const bool withBorder)
		    : m_BackgroundColor(backgroundColor),
		      m_TextColor(textColor),
		      m_BorderColor(borderColor),
		      m_HoverColor(hoverColor),
		      m_WithBackground(withBackground),
		      m_WithBorder(withBorder),
		      m_DrawLocation(drawLocation) {
			m_IsSet = true;
		}
	};// namespace Properties
};  // namespace HBUI