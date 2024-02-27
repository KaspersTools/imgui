//
// Created by Kasper de Bruin on 06/02/2024.
//

#pragma once
#ifndef HBUI_API
#define HBUI_API
#endif

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "Sources/HBUIEnums.cpp"

template<typename T>
class HBAnimProps;

namespace HBUI::Fonts {
	class FontLoader;
}
struct HBStyle {
	bool useHBUIStyleMenuBarColor = false;// Use the HBUI style for the menus

	ImVec2 menuItemSide = {45,
	                       45};// The size of the menuitem items (width, height)

	ImVec2 menuItemSpacing = {
	    5, 9};// The spacing for the menu items (top, right, bottom, left). If
	          // not set the imgui frame spacing

	ImColor sideBarColor     = {163, 255, 0, 255};
	ImColor sideBarItemColor = {255, 0, 266, 255};// The color/Tint of the menu items
};

struct HBIO {
	std::string title = "ImVK";

	int width  = 1280;
	int height = 720;

	float dpiWindowSizeFactor = 1.0f;
	float fontRenderingScale  = 1.0f;

	MainWindowFlags mainWindowFlags = HBMainWindowFlags_None;
};

namespace HBUI::Animation {
	class HBAnimManager;
}
class IWidgetBase;

class HBWidgetManager;

struct HBContext {
	bool initialized = false;
	HBIO io          = {};
	HBStyle style    = {};
	HBTime time      = {};

	// Draw Data
	HBUI::Animation::HBAnimManager *animManager = nullptr;
	HBWidgetManager *widgetManager              = nullptr;
	HBUI::Fonts::FontLoader *fontLoader         = nullptr;

	~HBContext();

	void initialize();

	void startFrame();

	void endFrame();

	void update() {}

	bool hasAnimation(const ImGuiID &id);

	bool hasAnimation(const std::string &id);

	ImVec2 addAnimation(const std::string &id, HBAnimProps<ImVec2> props);
};

namespace HBUI {
	HBUI_API HBContext *getCurrentContext();

	HBUI_API HBContext *initialize(const std::string &title, int width,
	                               int height, MainWindowFlags flags);

	HBUI_API void afterBackendInitialized();

	HBUI_API void setCurrentContext(HBContext *ctx);

	HBUI_API void setCurrentImGuiContext(ImGuiContext *ctx);

	HBUI_API void clearContext();

	HBUI_API HBStyle &getStyle();

	HBUI_API HBIO &getIO();

	HBUI_API ImVec2 getCursorViewportPos();

	HBUI_API ImVec2 getContentRegionAvail();

	HBUI_API ImVec2 getWindowSize();

	HBUI_API ImVec2 getViewportPos();

	HBUI_API ImVec2 getViewportSize();

	HBUI_API void setMainWindowFlags(MainWindowFlags flags);

	HBUI_API bool isMaximized();

	/**
	 * @brief Jump to the next line
	 * @details Jumps to the next line of the current appending widget.
	 * @example This is an example of how to use the newLine function.s
	 * @code
	 *  if (HBUI::beginSideBar(ImGui::GetID("SideBar"), HBSideBarFlags_Horizontal,
	 * {510, 510}, {150, 150}, )) { HBUI::sideBarBarButton(ImGui::GetID("BTN1"),
	 * {0, 0}, {32, 32}); HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0, 0},
	 * {32, 32}); HBUI::newLine(); HBUI::sideBarBarButton(ImGui::GetID("BTN1"),
	 * {0, 0}, {32, 32}); HBUI::endSideBar();
	 *  }
	 * @endcode
	 * @param size       The height of the current line, can be 0
	 * @param resizeType The resize type of the current line. If set to
	 * HBWidgetResizeType_ScaleToChildren the height/width gets calculated based
	 * on the biggest child of the current line. If set to
	 * HBWidgetResizeType_Fixed the height/width is the size parameter.
	 */
	HBUI_API void newLine(float size = 0, HBWidgetResizeType_ resizeType =
	                                          HBWidgetResizeType_ScaleToChildren);

	//---------------------------------------------------------------------------------
	// [SECTION] Panels/Bars
	//---------------------------------------------------------------------------------
	HBUI_API bool beginFlexPanel(const std::string &id,
	                             const ImVec2 &position  = {0, 0},
	                             const ImVec2 &size      = {0, 0},
	                             const ImVec2 &cursorPos = {0, 0});
	HBUI_API void endFlexPanel();

	HBUI_API bool beginSideBar(const ImGuiID &id, const HBSideBarFlags flags,
	                           const ImVec2 &position,
	                           ImVec2 size,
	                           const ImVec2 &cursorPos,
	                           const ImVec4 &padding,
	                           const std::string &label, const HBDrawLocation itemFlags);

	IMGUI_API void endSideBar();

	IMGUI_API bool sideBarBarButton(const ImGuiID id, const std::string &label,
	                                const ImVec2 &position  = {0, 0},
	                                const ImVec2 &size      = {0, 0},
	                                const ImVec2 &cursorPos = {0, 0},
	                                const ImVec4 padding    = {0, 0, 0, 0});

	//---------------------------------------------------------------------------------
	// [SECTION] Sample/Debug Windows
	//---------------------------------------------------------------------------------
	HBUI_API void showDebugWindow(bool *p_open);

	HBUI_API void addDebugWidget(const std::string &name, std::shared_ptr<IWidgetBase> widget);

	//---------------------------------------------------------------------------------
	// [SECTION] Updating
	//---------------------------------------------------------------------------------
	HBUI_API void update(float deltatime);

	//---------------------------------------------------------------------------------
	// [SECTION] Rendering
	//---------------------------------------------------------------------------------
	HBUI_API void startFrame();

	HBUI_API void endFrame();

	HBUI_API bool wantToClose();

	HBUI_API void shutdown();

	//---------------------------------------------------------------------------------
	// [SECTION] Input
	//---------------------------------------------------------------------------------
	HBUI_API bool mouseOverRect(const ImVec2 &start, const ImVec2 &end);

	HBUI_API bool isFlagSet(int *flags, int flag);

	HBUI_API void toggleFlag(int flag);

	//---------------------------------------------------------------------------------
	// [SECTION] Fonts
	//---------------------------------------------------------------------------------
	HBUI_API bool loadFont(const std::string &fontPath, float fontSize,
	                       const std::string &fontName);

	HBUI_API bool loadFont(const std::string &fontPath, float fontSize,
	                       const std::string &fontName,
	                       const std::string &fontGlyphRanges);

	//---------------------------------------------------------------------------------
	// [SECTION] IDS
	//---------------------------------------------------------------------------------
	HBUI_API ImGuiID getID(const char *str, const char *str_end);

}// namespace HBUI
