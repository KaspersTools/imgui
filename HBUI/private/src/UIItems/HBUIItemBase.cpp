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
		if(widget->getDrawData().m_Position.x == 0 || widget->getDrawData().m_Position.y == 0) {
			widget->setPosition(s_cursorPos);
		}

		sp_AppendingWidget = widget;
		ImGui::Begin(widget->getLabel().c_str());
	}
}

void HBWidgetManager::endAppendingWidget(const HBUIType type) {
	if(sp_AppendingWidget == nullptr) {
		IM_ASSERT(false && "No widget to end");
		return;
	}

	if(sp_AppendingWidget->getParent() == nullptr) {
		sp_AppendingWidget->render();

		ImGui::Text("Screen min %f %f", sp_AppendingWidget->getScreenPosMin().x, sp_AppendingWidget->getScreenPosMin().y);
		ImGui::Text("Screen max %f %f", sp_AppendingWidget->getScreenPosMax().x, sp_AppendingWidget->getScreenPosMax().y);

		ImGui::Text("Size %f %f", sp_AppendingWidget->getXSize(), sp_AppendingWidget->getYSize());
		ImGui::Text("Position %f %f", sp_AppendingWidget->getDrawData().m_Position.x, sp_AppendingWidget->getDrawData().m_Position.y);

		ImGui::Text("End of %s", sp_AppendingWidget->getLabel().c_str());
		ImGui::End();

		if(s_layoutType == HBLayoutType_Horizontal)
		{
			s_cursorPos.x += sp_AppendingWidget->getXSize();
		}
		else if(s_layoutType == HBLayoutType_Vertical)
		{
			s_cursorPos.y += sp_AppendingWidget->getYSize();
		}
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