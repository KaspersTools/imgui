//
// Created by Kasper de Bruin on 03/03/2024.
//

// clang-format off
#include <UIItems/Builders/HBWidgetColorPropertiesBuilder.h>
#include <UIItems/Builders/HBWidgetLayoutPropertiesBuilder.h>

#include "HBItemBuilder.h"
// clang-format on
namespace HBUI::Builder{
	HBItemBuilder &HBItemBuilder::setColorProperties(const HBUIType_ type, HBDrawFlags_ drawFlags, HBColorPropertiesFlags flags) {

		setType(type);
		p_CurrentWidget->m_ColorProperties = Builder::HBWidgetColorPropertiesBuilder().setFromImGuiStyle(type, drawFlags, flags).build();
		return *this;
	}
}// namespace HBUI::Builder