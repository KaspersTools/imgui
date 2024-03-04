//
// Created by Kasper de Bruin on 04/03/2024.
//

// clang-format off
#include <UIItems/Builders/HBWidgetLayoutPropertiesBuilder.h>

#include "HBWidgetLayoutProperties.h"
// clang-format on

namespace HBUI {
	namespace Properties {
		[[maybe_unused]] WidgetLayoutProperties::WidgetLayoutProperties(const HBLayoutType_ layoutType,
		                                                                const ImVec2 &cursorPos,
		                                                                const ImVec2 &size,
		                                                                const HBPadding &padding)
		    : m_LayoutType(layoutType),
		      m_CursorPos(cursorPos),
		      m_Size(size),
		      m_Padding(padding) {
		}

		HBLayoutType_ WidgetLayoutProperties::getLayoutType() const {
			return m_LayoutType;
		}
		const ImVec2 &WidgetLayoutProperties::getCursorPos() const {
			return m_CursorPos;
		}
		const ImVec2 &WidgetLayoutProperties::getSize() const {
			return m_Size;
		}
		const HBPadding &WidgetLayoutProperties::getPadding() const {
			return m_Padding;
		}
	}// namespace Properties
}; // namespace HBUI