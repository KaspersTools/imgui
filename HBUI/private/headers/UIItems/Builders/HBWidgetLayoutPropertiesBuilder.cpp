//
// Created by Kasper de Bruin on 04/03/2024.
//

#include "HBWidgetLayoutPropertiesBuilder.h"

namespace HBUI {
	namespace Builder {

		HBWidgetLayoutPropertiesBuilder &HBWidgetLayoutPropertiesBuilder::fromImGuiStyle(const HBUIType_ type) {
			ImGuiStyle &style = ImGui::GetStyle();
			switch (type) {
				case HBUIType_None:
					IM_ASSERT(false && "HBUIType_None is not a valid type for layout properties");
					break;
				case HBUIType_SideBar:
				case HBUIType_Button:
				case HBUIType_SideBarButton:
					m_LayoutProperties->m_Padding = HBPadding(style.FramePadding.x, style.FramePadding.y);
					//					m_LayoutProperties->m_Margin  = HBMargin(style.FramePadding.x, style.FramePadding.y);
					break;
				case HBUIType_DockSpaceWindow:
					m_LayoutProperties->m_Padding = HBPadding(style.WindowPadding.x, style.WindowPadding.y);
					break;
			}
			return *this;
		}

		HBUI::Properties::WidgetLayoutProperties *HBWidgetLayoutPropertiesBuilder::build() {
			HBUI::Properties::WidgetLayoutProperties *properties = m_LayoutProperties;
			m_LayoutProperties = nullptr;
			return properties;
		}
	}// namespace Builder
}// namespace HBUI