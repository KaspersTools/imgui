//
// Created by Kasper de Bruin on 21/02/2024.
//
#include <UIItems/HBUIItemBase.h>


namespace HBUI {
	//-------------------------------------------------------------------------
	// [SECTION] HBWidgetanager
	//-------------------------------------------------------------------------
	void HBWidgetManager::appendWidget(HBUI::HBItemBase *widget) {
		IM_ASSERT(widget->getType() != HBUIType_None && "This type of widget is not allowed to be appended without a parent");
		if (this->sp_AppendingWidget != nullptr) {
			this->sp_AppendingWidget->addChild(widget);
			return;
		}
		switch (HBUI::typeToBaseType(widget->getType())) {
			case HBUIBaseType_Window:
				this->sp_AppendingWidget = widget;
				break;
			case HBUIBaseType_Item:
			case HBUIBaseType_Menu:
				IM_ASSERT(false && "This type of widget is not allowed to be appended without a parent");
				break;
			case HBUIBaseType_Panel:
				break;
		}
	}
	void HBWidgetManager::endAppendingWidget(const HBUIType_ type) {
		//todo: add checks
		//todo: delete ptr
	}

	//-------------------------------------------------------------------------
	// [SECTION] HBItemBase
	//-------------------------------------------------------------------------
	ImVec2 HBItemBase::getCursorPos() const {
		return m_LayoutProperties->getCursorPos();
	}
	ImGuiID HBItemBase::getId() const {
		return m_ID;
	}
	HBUIType_ HBItemBase::getType() const {
		return m_Type;
	}
	const std::string &HBItemBase::getLabel() const {
		return m_Label;
	}
	Properties::WidgetColorProperties *HBItemBase::getColorProperties() const {
		return m_ColorProperties;
	}
	Properties::WidgetLayoutProperties *HBItemBase::getLayoutProperties() const {
		return m_LayoutProperties;
	}
	HBItemBase *HBItemBase::getParent() const {
		return cp_Parent;
	}
	const std::vector<HBItemBase *> &HBItemBase::getChildren() const {
		return pv_Children;
	}


}// namespace HBUI