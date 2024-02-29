//
// Created by Kasper de Bruin on 19/01/2024.
//
#include "MainWindow.h"

#include <HBUI/HBUI.h>


void MainWindow::init() {
	HBUI::initialize("Hello, World!", 1280, 720, HBMeinWindowFlags_NoTitleBar);
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

		if (HBUI::beginSideBar(             //begin the side bar
		        ImGui::GetID("SideBar"),    //id
		        HBSideBarFlags_Vertical |   //flags -> horizontal = direction
		            HBSideBarFlags_FullSize,//full size =w take the full screen width/height from the satart pos
		        ImVec2(0, 0),               //start position
		        ImVec2(0, 0),               //size -> 0,0 means in combination with the flags it will take the full screen width, and the/
		        ImVec2(0, 0),               //cursorStartPos
		        m_Padding,
		        ImVec4(0, 0, 0, 0),//margin
		        "SideBar",         //label
		        HBDrawFlags_MainImguiWindowDrawlist)) {
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
	}


	//	{
	//
	//		if (HBUI::beginSideBar(             //begin the side bar
	//		        ImGui::GetID("SideBar"),    //id
	//		        HBSideBarFlags_Horizontal |   //flags -> horizontal = direction
	//		            HBSideBarFlags_FullSize,//full size = take the full screen width/height from the satart pos
	//		        ImVec2(0, 0),               //start position
	//		        ImVec2(0, 0),               //size -> 0,0 means in combination with the flags it will take the full screen width, and the/
	//		        ImVec2(0, 0),               //cursorStartPos
	//		        m_PaddingTopBar,
	//		        "SideBar",//label
	//		        HBDrawFlags_ForegroundDrawList)) {
	//			HBUI::sideBarBarButton(ImGui::GetID("BTN2"), "BTN2", {0, 0}, {54, 54}, {0, 0}, {0, 0, 0, 0});
	//			HBUI::sideBarBarButton(ImGui::GetID("BTN3"), "BTN3", {0, 0}, {54, 54}, {0, 0}, {0, 0, 0, 0});
	//			HBUI::sideBarBarButton(ImGui::GetID("BTN4"), "BTN4", {0, 0}, {54, 54}, {0, 0}, {0, 0, 0, 0});
	//			HBUI::sideBarBarButton(ImGui::GetID("BTN5"), "BTN5", {0, 0}, {54, 54}, {0, 0}, {0, 0, 0, 0});
	//			HBUI::sideBarBarButton(ImGui::GetID("BTN6"), "BTN6", {0, 0}, {54, 54}, {0, 0}, {0, 0, 0, 0});
	//
	//			HBUI::endSideBar();
	//		}
	//	}

	ImGui::Begin("Hello, World!");
	ImGui::Text("This is some useful text.");

	ImGui::Text("Vertical");

	ImGui::BeginChild("Vertical", ImVec2(ImGui::GetContentRegionAvail().x, 0), true);
	ImGui::SeparatorText("padding ");
	ImGui::Text("Side bar");
	ImGui::DragFloat("Left Padding", &m_Padding.x, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Top Padding", &m_Padding.y, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Right Padding", &m_Padding.z, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Bottom Padding", &m_Padding.w, 1.0f, 0.0f, 100.0f);

	ImGui::SeparatorText("Margin");
	ImGui::Text("Buttons");
	ImGui::DragFloat("Left Button Margin", &m_ButtonMargin.x, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Top Button Margin", &m_ButtonMargin.y, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Right Button Margin", &m_ButtonMargin.z, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Bottom Button Margin", &m_ButtonMargin.w, 1.0f, 0.0f, 100.0f);

	ImGui::DragFloat("Left Button Padding", &m_PaddingButtons.x, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Top Button Padding", &m_PaddingButtons.y, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Right Button Padding", &m_PaddingButtons.z, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Bottom Button Padding", &m_PaddingButtons.w, 1.0f, 0.0f, 100.0f);
	ImGui::EndChild();

	ImGui::Text("Horizontal");
	ImGui::BeginChild("Horizontal", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0), true);
	ImGui::Text("Top Bar Padding: %.1f", m_PaddingTopBar.y);
	ImGui::DragFloat("Left Padding", &m_PaddingTopBar.x, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Top Padding", &m_PaddingTopBar.y, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Right Padding", &m_PaddingTopBar.z, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Bottom Padding", &m_PaddingTopBar.w, 1.0f, 0.0f, 100.0f);
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
