//
// Created by Kasper de Bruin on 19/01/2024.
//
#include "MainWindow.h"

#include <HBUI/HBUI.h>


void MainWindow::init() {
	HBUI::initialize("Hello, World!", 1280, 720, HBMeinWindowFlags_NoTitleBar);

	m_ColorVerticalBar = m_ColorHorizontalBar = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg);

	run();
}

bool MainWindow::run() {
	while (!HBUI::wantToClose()) {
		render();
	}
	return true;
}


void MainWindow::render() {
	HBUI::startFrame();

	static bool drawMultiRowSideBar = false;
	static bool drawIconSideBar     = true;
	//sidebars
	{

		//SIDE BAR FLAGS
		ImGuiID id                  = ImGui::GetID("SideBar01");               //id
		HBSideBarFlags sideBarFlags =                                          //Side bar specific flags
		    HBSideBarFlags_Vertical |                                          //Vertical = direction
		    HBSideBarFlags_FullSize;                                           //full size = take the full screen width/height from the satart pos
		ImVec2 position       = {0, 0};                                        //start position
		ImVec2 size           = {0, 0};                                        //size -> 0,0 means in combination with the flags it will resize to the children
		ImVec2 cursorPos      = {0, 0};                                        //cursorStartPos
		ImVec4 padding        = m_PaddingVerticalBar;                          //padding
		ImVec4 margin         = m_MarginVerticalBar;                           //margin
		std::string label     = "SideBar01";                                   //label
		HBItemFlags itemFlags = HBItemFlags_ResizeChildrenToBiggestChild;      //itemFlags = shared item flags, that cn be used
		                                                                       // on every widget
		HBDrawLocation drawLocationFlags = HBDrawFlags_MainImguiWindowDrawlist;//drawLocationFlag = where to draw the side bar

		//Vertical side bar
		if (HBUI::beginSideBar(
		        id,
		        m_ColorVerticalBar,
		        sideBarFlags,
		        position,
		        size,
		        cursorPos,
		        padding,
		        margin,
		        label,
		        itemFlags,
		        drawLocationFlags)) {

			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);

			HBUI::endSideBar();
		}

		//horizontal side bar
		ImGuiID horId = ImGui::GetID("SideBar02");//id
		sideBarFlags  =                           //Side bar specific flags
		    HBSideBarFlags_Horizontal |           //Horizontal = direction
		    HBSideBarFlags_FullSize;              //full size = take the full screen width/height from the satart pos
		position = {0, 0};                        //start position
		label    = "SideBar02";                   //label
		margin   = m_MarginHorizontalBar;         //margin
		padding  = m_PaddingHorizontalBar;        //padding

		if (HBUI::beginSideBar(
		        horId,
		        m_ColorHorizontalBar,
		        sideBarFlags,
		        position,
		        size,
		        cursorPos,
		        padding,
		        margin,
		        label,
		        itemFlags,
		        drawLocationFlags)) {

			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);
			HBUI::sideBarBarButton(ImGui::GetID("BTN1"), NF_ICON_github, {0, 0}, {0, 0}, 42, {0, 0}, m_PaddingButtons, m_ButtonMargin);

			HBUI::endSideBar();
		}
	}


	ImGui::Begin("Side Bar(s) Settings");

	ImGui::BeginChild("SideBarSettings", ImVec2(0, 0), true);
	ImGui::SeparatorText("Colors and drawing ");
	ImGui::Text("Side bar Vertical");
	ImGui::ColorPicker4("Color Vertical", (float *) &m_ColorVerticalBar);
	ImGui::Separator();
	ImGui::Text("Side bar Vertical");
	ImGui::ColorPicker4("Color Horizontal", (float *) &m_ColorHorizontalBar);

	ImGui::SeparatorText("padding ");
	ImGui::Text("Side bar Vertical");
	ImGui::DragFloat("Left Padding Vertical", &m_PaddingVerticalBar.x, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Top Padding Vertical", &m_PaddingVerticalBar.y, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Right Padding Vertical", &m_PaddingVerticalBar.z, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Bottom Padding Vertical", &m_PaddingVerticalBar.w, 1.0f, 0.0f, 100.0f);
	ImGui::Separator();
	ImGui::Text("Side bar Horizontal");
	ImGui::DragFloat("Left Padding Horizontal", &m_PaddingHorizontalBar.x, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Top Padding Horizontal", &m_PaddingHorizontalBar.y, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Right Padding Horizontal", &m_PaddingHorizontalBar.z, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Bottom Padding Horizontal", &m_PaddingHorizontalBar.w, 1.0f, 0.0f, 100.0f);

	ImGui::EndChild();

	ImGui::BeginChild("Buttons", ImVec2(0,0), true);
	ImGui::SeparatorText("Margin");
	ImGui::Text("Side bar Vertical");
	ImGui::DragFloat("Left Button Margin", &m_ButtonMargin.x, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Top Button Margin", &m_ButtonMargin.y, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Right Button Margin", &m_ButtonMargin.z, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Bottom Button Margin", &m_ButtonMargin.w, 1.0f, 0.0f, 100.0f);

	ImGui::SeparatorText("Padding");
	ImGui::Text("Side bar Vertical");
	ImGui::DragFloat("Left Button Padding", &m_PaddingButtons.x, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Top Button Padding", &m_PaddingButtons.y, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Right Button Padding", &m_PaddingButtons.z, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Bottom Button Padding", &m_PaddingButtons.w, 1.0f, 0.0f, 100.0f);
	ImGui::EndChild();

	ImGui::End();

	if (showDemoWindow) {
		ImGui::ShowDemoWindow(&showDemoWindow);
	}


#ifdef HBUI_WITH_DEBUG_WINDOW
	//only call this function at the end of a frame.
	HBUI::showDebugWindow(&showDebugWindow);
#endif

	HBUI::endFrame();
}

void MainWindow::shutdown() {
	HBUI::shutdown();
}
