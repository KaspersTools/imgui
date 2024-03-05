//
// Created by Kasper de Bruin on 03/03/2024.
//

#ifndef IMGUI_HBITEMBUILDER_H
#define IMGUI_HBITEMBUILDER_H

#include "UIItems/HBUIItemBase.h"
#include <HBUI/HBUI.h>

#include <UIItems/Properties/HBWidgetColorProperties.h>
#include <UIItems/Properties/HBWidgetLayoutProperties.h>

namespace HBUI {
	namespace Builder {
		class HBItemBuilder {
	private:
			HBItemBase *p_CurrentWidget = nullptr;

	public:
			HBItemBuilder() {
				p_CurrentWidget = new HBItemBase();
			}

			~HBItemBuilder() {
				delete p_CurrentWidget;
			}

			HBItemBuilder &setID(const ImGuiID id) {
				p_CurrentWidget->m_ID = id;
				return *this;
			}

			HBItemBuilder &setLabel(const std::string &label) {
				p_CurrentWidget->m_Label = label;
				return *this;
			}

			HBItemBuilder &setType(const HBUIType_ type) {
				p_CurrentWidget->m_Type = type;
				return *this;
			}

			HBItemBuilder &setParent(HBItemBase *parent) {
				p_CurrentWidget->p_Parent = parent;
				return *this;
			}

			HBItemBuilder &fromImGuiStyle(const HBDrawFlags_ drawFlags, const HBColorPropertiesFlags flags);
			//----------------------------------------------------------------------
			// [Section]Properties
			//----------------------------------------------------------------------
			HBItemBuilder &setColorProperties(const HBDrawFlags_ drawFlags, const HBColorPropertiesFlags flags);
			HBItemBuilder &setColorProperties(Properties::WidgetColorProperties *colorProperties) {
				p_CurrentWidget->m_ColorProperties = colorProperties;
				return *this;
			}

			HBItemBuilder &setLayoutProperties(Properties::WidgetLayoutProperties *layoutProperties) {
				p_CurrentWidget->m_LayoutProperties = layoutProperties;
				return *this;
			}
			HBItemBuilder &setLayoutProperties();

			virtual HBItemBase *build() {
				IM_ASSERT(p_CurrentWidget->getType() != HBUIType_None && "Type is unknown, cannot build");
				IM_ASSERT(p_CurrentWidget->m_ID != -1 || !p_CurrentWidget->m_Label.empty() && "ID is not set and label is empty");

				if(p_CurrentWidget->m_ColorProperties == nullptr) {
					setColorProperties(HBDrawFlags_None, HBColorPropertiesFlags_Default);
				}
				if(p_CurrentWidget->m_LayoutProperties == nullptr) {
					setLayoutProperties();
				}

				if(p_CurrentWidget->m_ID == -1 && !p_CurrentWidget->m_Label.empty()) {
					p_CurrentWidget->m_ID = ImGui::GetID(p_CurrentWidget->m_Label.c_str());
				}
				if(p_CurrentWidget->m_Label.empty() && p_CurrentWidget->m_ID != -1) {
					p_CurrentWidget->m_Label = "UNNAMED: " + std::to_string(p_CurrentWidget->m_ID);
				}

				HBItemBase *item = p_CurrentWidget;
				p_CurrentWidget  = nullptr;
				return item;
			}
		};
	}// namespace Builder
}// namespace HBUI


#endif//IMGUI_HBITEMBUILDER_H
