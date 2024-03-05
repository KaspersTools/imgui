//
// Created by Kasper de Bruin on 04/03/2024.
//

//clang-format off
#include "HBWidgetColorPropertiesBuilder.h"
#include <memory>

//clang-format on
namespace HBUI {
	namespace Builder {
		[[maybe_unused]] HBWidgetColorPropertiesBuilder &HBWidgetColorPropertiesBuilder::fromImGuiStyle(HBUIType_ type,
		                                                                                                const HBDrawFlags_ drawLocation,
		                                                                                                const HBColorPropertiesFlags flags,

		                                                                                                const ImColor &backgroundColor,
		                                                                                                const ImColor &borderColor,
		                                                                                                const ImColor &hoverColor) {
			//set color based on imgui style
			switch (type) {
				case HBUIType_None:
					IM_ASSERT(false && "HBUIType_None is not a valid type");
					break;
				case HBUIType_SideBar:
					//this->setBackgroundColor(ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg));
					//this->setDrawLocation(HBDrawFlags_OnParent);
					break;
				case HBUIType_SideBarButton:
				case HBUIType_Button:
					//this->setDrawLocation(HBDrawFlags_OnParent);
					//this->setBackgroundColor(ImGui::GetStyleColorVec4(ImGuiCol_Button));
					//this->setHoverColor(ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
					break;
				case HBUIType_DockSpaceWindow:
					//this->setDrawLocation(HBDrawFlags_OnParent);
					//this->setBackgroundColor(ImGui::GetStyleColorVec4(ImGuiCol_DockingEmptyBg));
					break;
				default:
					IM_ASSERT(false && "HBUIType not implemented");
					break;
			}

			//check flags
			const bool noBackground = ((flags & HBColorPropertiesFlags_NoBackground) != 0);
			const bool noBorder     = ((flags & HBColorPropertiesFlags_NoBorder) != 0);
			const bool noHover      = ((flags & HBColorPropertiesFlags_NoHover) != 0);

			if (noBackground) {
				this->setWithBackground(false);
			}
			if (backgroundColor.Value != ImVec4(-1, -1, -1, -1)) {
				this->setBackgroundColor(backgroundColor);
			}

			if (noBorder) {
				this->setWithBorder(false);
			}
			if (borderColor.Value != ImVec4(-1, -1, -1, -1)) {
				this->setBorderColor(ImGui::GetStyleColorVec4(ImGuiCol_Border));
			}

			if (noHover) {
				this->setHoverColor(ImColor(-1, -1, -1, -1));
			}
			if (hoverColor.Value != ImVec4(-1, -1, -1, -1)) {
				this->setHoverColor(hoverColor);
			}

			return *this;
		}

		HBUI::Properties::WidgetColorProperties *HBWidgetColorPropertiesBuilder::build() {
			HBUI::Properties::WidgetColorProperties* properties = m_ColorProperties;
			m_ColorProperties = nullptr;
			return properties;
		}
	}// namespace Builder
}// namespace HBUI