//
// Created by Kasper de Bruin on 03/03/2024.
//

#ifndef IMGUI_HBITEMBUILDER_H
#define IMGUI_HBITEMBUILDER_H

#include <HBUI/HBUI.h>

namespace HBUI {
	namespace Builder {
		class HBWidgetColorPropertiesBuilder;
		class HBWidgetLayoutPropertiesBuilder;


		class HBItemBuilder {
	private:
			HBItemBase *p_CurrentWidget = nullptr;

	public:
			HBItemBuilder();
			HBItemBuilder &setID(const ImGuiID id) {
				p_CurrentWidget->m_ID = id;
				return *this;
			}

			HBItemBuilder &setLabel(const std::string &label) {
				p_CurrentWidget->m_Label = label;
				return *this;
			}

			HBItemBuilder &setScreenPosition(const ImVec2 &screenPosition) {
				p_CurrentWidget->m_ScreenPosition = screenPosition;
				return *this;
			}

			HBItemBuilder &setSize(const ImVec2 &size) {
				p_CurrentWidget->m_Size = size;
				return *this;
			}

			HBItemBuilder &setType(const HBUIType_ type) {
				p_CurrentWidget->m_Type = type;
				return *this;
			}

			HBItemBuilder &setParent(HBItemBase *parent) {
				p_CurrentWidget->cp_Parent = parent;
				return *this;
			}

			//----------------------------------------------------------------------
			// [Section]Properties
			//----------------------------------------------------------------------
			HBItemBuilder &setColorProperties(const HBUIType_ type, HBDrawFlags_ drawFlags, HBColorPropertiesFlags flags);
			HBItemBuilder &setColorProperties(WidgetColorProperties *colorProperties) {
				p_CurrentWidget->m_ColorProperties = colorProperties;
				return *this;
			}

			HBItemBuilder &setLayoutProperties(WidgetLayoutProperties *layoutProperties) {
				p_CurrentWidget->m_LayoutProperties = layoutProperties;
				return *this;
			}

			virtual HBItemBase *build() {
				IM_ASSERT(p_CurrentWidget != nullptr &&
				          "Widget is nullptr something went wrong....");
				HBItemBase *item = p_CurrentWidget;
				p_CurrentWidget  = nullptr;
				return item;
			}
		};
	}// namespace Builder
}// namespace HBUI


#endif//IMGUI_HBITEMBUILDER_H
