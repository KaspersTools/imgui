//
// Created by Kasper de Bruin on 03/03/2024.
//

// clang-format off
#include "HBItemBuilder.h"
#include <UIItems/Builders/HBWidgetColorPropertiesBuilder.h>
#include <UIItems/Builders/HBWidgetLayoutPropertiesBuilder.h>
// clang-format on

namespace HBUI::Builder{

	HBItemBuilder &HBItemBuilder::setColorProperties(const HBDrawFlags_ drawFlags, const HBColorPropertiesFlags flags) {
		IM_ASSERT(p_CurrentWidget->getType() != HBUIType_None && "Type is unknown, cannot set layout properties");
		HBWidgetColorPropertiesBuilder colorPropertiesBuilder;
		p_CurrentWidget->m_ColorProperties = colorPropertiesBuilder.fromImGuiStyle(p_CurrentWidget->getType(), drawFlags, flags).build();
		return *this;
	}
	HBItemBuilder &HBItemBuilder::setLayoutProperties() {
		IM_ASSERT(p_CurrentWidget->getType() != HBUIType_None && "Type is unknown, cannot set layout properties");
		HBWidgetLayoutPropertiesBuilder layoutPropertiesBuilder;
		p_CurrentWidget->m_LayoutProperties = layoutPropertiesBuilder.fromImGuiStyle(p_CurrentWidget->getType()).build();
		return *this;
	}
	HBItemBuilder &HBItemBuilder::fromImGuiStyle(const HBDrawFlags_ drawFlags, const HBColorPropertiesFlags flags) {
		IM_ASSERT(p_CurrentWidget->getType() != HBUIType_None && "Type is unknown, cannot set layout properties");
		setColorProperties(drawFlags, flags);
		setLayoutProperties();
		return *this;
	}
}// namespace HBUI::Builder