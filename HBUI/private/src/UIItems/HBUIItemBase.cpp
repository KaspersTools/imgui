////
//// Created by Kasper de Bruin on 21/02/2024.
////
//
//// clang-format off
//
//#include <UIItems/Builders/HBItemBuilder.h>
//#include <UIItems/HBUIItemBase.h>
//
//// clang-format on
//
//namespace HBUI {
//	[[maybe_unused]] void HBWidgetManager::appendWidget(HBUI::HBItemBase *widget) {
//		IM_ASSERT(widget->getType() != HBUIType_None && "This type of widget is not allowed to be appended without a parent");
//		if (this->sp_AppendingWidget != nullptr) {
//			this->sp_AppendingWidget->addChild(widget);
//			return;
//		}
//		switch (HBUI::typeToBaseType(widget->getType())) {
//			case HBUIBaseType_Window:
//				this->sp_AppendingWidget = widget;
//				break;
//			case HBUIBaseType_Item:
//			case HBUIBaseType_Menu:
//				IM_ASSERT(false && "This type of widget is not allowed to be appended without a parent");
//			case HBUIBaseType_Panel:
//				break;
//		}
//	}
//
//	[[maybe_unused]] void HBWidgetManager::endAppendingWidget([[maybe_unused]] const HBUIType_ type) {
//		//todo: add checks
//		//todo: delete ptr
//	}
//
//	//-------------------------------------------------------------------------
//	// [SECTION] HBItemBase
//	//-------------------------------------------------------------------------
//
//
//	HBItemBase::HBItemBase(const ImGuiID Id,
//	                       const std::string &Label,
//	                       const HBUIType_ Type,
//	                       HBUI::Properties::WidgetColorProperties *ColorProperties,
//	                       HBUI::Properties::WidgetLayoutProperties *LayoutProperties,
//	                       HBItemBase *Parent,
//	                       const std::vector<HBItemBase *> &Children) {
//		m_ID               = Id;
//		m_Label            = Label;
//		m_Type             = Type;
//		m_ColorProperties  = ColorProperties;
//		m_LayoutProperties = LayoutProperties;
//		p_Parent           = Parent;
//		m_Children         = Children;
//	}
//
//	[[maybe_unused]] ImVec2 HBItemBase::getMainWindowCursorPos() const {
//		return m_LayoutProperties->getMainWindowCursorPos();
//	}
//	[[maybe_unused]] ImGuiID HBItemBase::getId() const {
//		return m_ID;
//	}
//	[[maybe_unused]] HBUIType_ HBItemBase::getType() const {
//		return m_Type;
//	}
//	[[maybe_unused]] const std::string &HBItemBase::getLabel() const {
//		return m_Label;
//	}
//	[[maybe_unused]] Properties::WidgetColorProperties &HBItemBase::getColorProperties() const {
//		return *m_ColorProperties;
//	}
//	[[maybe_unused]] Properties::WidgetLayoutProperties &HBItemBase::getLayoutProperties() const {
//		return *m_LayoutProperties;
//	}
//	[[maybe_unused]] HBItemBase *HBItemBase::getParent() const {
//		return p_Parent;
//	}
//	[[maybe_unused]] const std::vector<HBItemBase *> &HBItemBase::getChildren() const {
//		return m_Children;
//	}
//	[[maybe_unused]] void HBItemBase::addChild(HBItemBase *child) {
//	}
//
//
//}// namespace HBUI