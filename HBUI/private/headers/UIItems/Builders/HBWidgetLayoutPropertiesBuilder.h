//
// Created by Kasper de Bruin on 04/03/2024.
//

#ifndef IMGUI_HBWIDGETLAYOUTPROPERTIESBUILDER_H
#define IMGUI_HBWIDGETLAYOUTPROPERTIESBUILDER_H

#include "UIItems/Properties/HBWidgetLayoutProperties.h"

namespace HBUI {
	namespace Builder {
		class HBWidgetLayoutPropertiesBuilder {
	private:
			HBUI::Properties::WidgetLayoutProperties *m_LayoutProperties = nullptr;

	public:
			HBWidgetLayoutPropertiesBuilder() {
				m_LayoutProperties = new HBUI::Properties::WidgetLayoutProperties();
			}

			~HBWidgetLayoutPropertiesBuilder() {
				delete m_LayoutProperties;
			}

			HBWidgetLayoutPropertiesBuilder &fromImGuiStyle(const HBUIType_ type);

			HBUI::Properties::WidgetLayoutProperties *build();
		};
	}// namespace Builder
}// namespace HBUI


#endif//IMGUI_HBWIDGETLAYOUTPROPERTIESBUILDER_H
