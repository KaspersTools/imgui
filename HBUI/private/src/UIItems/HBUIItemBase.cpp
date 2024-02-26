//
// Created by Kasper de Bruin on 21/02/2024.
//
#include <HBUI/HBUI.h>
#include <UIItems/HBUIItemBase.h>

void HBWidgetManager::appendWidget(IWidgetBase* widget) {
	auto type = widget->getUIType();
	IM_ASSERT(type != HBUIType_None && "The widget type is not set");

	if (sp_AppendingWidget != nullptr) {
		sp_AppendingWidget = 	sp_AppendingWidget->appendChild(widget);
	}else{
		sp_AppendingWidget = widget;
	}
}

void HBWidgetManager::endAppendingWidget(const HBUIType type) {

	if(sp_AppendingWidget == nullptr) {
		IM_ASSERT(false && "No widget to end");
		return;
	}

	if(sp_AppendingWidget->getParent() == nullptr) {
		sp_AppendingWidget->render();
		delete sp_AppendingWidget;
		sp_AppendingWidget = nullptr;
	}else{
		sp_AppendingWidget = sp_AppendingWidget->getParent();
	}
}

void IWidgetBase::render() {
	for (auto &child: m_Children) {
		child->render();
	}
}