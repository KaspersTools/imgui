//
// Created by Kasper de Bruin on 01/03/2024.
//

#ifndef IMGUI_HBWIDGETLAYOUTPROPERTIES_H
#define IMGUI_HBWIDGETLAYOUTPROPERTIES_H

namespace HBUI {
	struct WidgetLayoutProperties {
		HBLayoutType_ m_LayoutType = HBLayoutType_::HBLayoutType_Horizontal;
		ImVec2 m_CursorPosStart   = ImVec2(0.f, 0.f);
		ImVec2 m_CursorPosEnd     = ImVec2(0.f, 0.f);

		ImVec2 m_CursorPos  = ImVec2(0.f, 0.f);
		ImVec2 m_Size       = ImVec2(0.f, 0.f);
		HBPadding m_Padding = HBPadding(0.f);
		HBPadding m_Margin  = HBPadding(0.f);
	};
}// namespace HBUI
#endif//IMGUI_HBWIDGETLAYOUTPROPERTIES_H
