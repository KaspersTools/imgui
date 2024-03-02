//
// Created by Kasper de Bruin on 21/02/2024.
//
#include <UIItems/HBUIItemBase.h>

namespace HBUI {
	//-------------------------------------------------------------------------
	// [SECTION] HBWidgetManager
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
		}
	}
	void HBWidgetManager::endAppendingWidget(const HBUIType_ type) {
		//todo: add checks
		//todo: delete ptr
	}

	//-------------------------------------------------------------------------
	// [SECTION] HBItemBase
	//-------------------------------------------------------------------------
	ImVec2 HBItemBase::getNextChildPosition() const {
		return m_LayoutProperties.m_CursorPos;
	}

	ImVec2 HBItemBase::getCursorPos() const {
		return m_LayoutProperties.m_CursorPos;
	}

	ImVec2 HBItemBase::getContentRegionAvail(const bool includePadding) const {
		ImVec2 size             = m_LayoutProperties.m_Size;
		ImVec2 currentCursorPos = getCursorPos();
		ImVec2 result           = ImVec2(size.x - currentCursorPos.x, size.y - currentCursorPos.y);

		return result;
	}

	ImVec2 HBItemBase::getContentRegionMax(const bool includePadding) const {
		return m_LayoutProperties.m_CursorPos;
	}

	void HBItemBase::setCursorPos(const ImVec2 &newPos) {}
}// namespace HBUI