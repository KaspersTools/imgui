//
// Created by Kasper de Bruin on 01/03/2024.
//

#ifndef IMGUI_HBWIDGETLAYOUTPROPERTIES_H
#define IMGUI_HBWIDGETLAYOUTPROPERTIES_H

#include <HBUI/HBUI.h>

namespace HBUI {

	namespace Builder {
		class HBWidgetLayoutPropertiesBuilder;
	};// namespace Builder

	namespace Properties {
		struct [[maybe_unused]] WidgetLayoutProperties {
			friend class HBUI::Builder::HBWidgetLayoutPropertiesBuilder;

	public:
			WidgetLayoutProperties() = default;

	private:
			[[maybe_unused]] WidgetLayoutProperties(const HBLayoutType_ layoutType,
			                                        const ImVec2 &cursorPos,
			                                        const ImVec2 &size,
			                                        const HBPadding &padding);

	public:
			HBLayoutType_ getLayoutType() const;
			const ImVec2 &getCursorPos() const;
			const ImVec2 &getSize() const;
			const HBPadding &getPadding() const;


	private:
			HBLayoutType_ m_LayoutType = HBLayoutType_::HBLayoutType_Vertical;
			ImVec2 m_CursorPos         = ImVec2(0.f, 0.f);

			ImVec2 m_Size = ImVec2(0.f, 0.f);

			HBPadding m_Padding = HBPadding(0.f);
			//			HBMargin m_Margin   = HBMargin(0.f); //todo: implement margin
		};
	}// namespace Properties
}// namespace HBUI

#endif//IMGUI_HBWIDGETLAYOUTPROPERTIES_H
