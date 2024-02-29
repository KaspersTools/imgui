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

#include "fonts/FontHeaders/IconsNerdFont.h"

template<typename T>
class HBAnimProps;

inline static float fontDefaultSize = 16;//fixme: move to config file
inline static float iconDefaultSize = 24;//fixme: move to config file


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

	ImGuiWindow *mainWindow = nullptr;

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
	HBUI_API std::string wchar32ToUtf8(const ImWchar &wchar);

	HBUI_API HBContext *getCurrentContext();

	HBUI_API HBContext *initialize(const std::string &title, int width,
	                               int height, MainWindowFlags flags);

	HBUI_API void afterBackendInitialized();

	HBUI_API HBStyle &getStyle();

	HBUI_API HBIO &getIO();

	HBUI_API ImGuiWindow *getMainImGuiWindow();

	HBUI_API ImVec2 getWindowSize();

	HBUI_API ImVec2 getViewportPos();

	HBUI_API ImVec2 getViewportSize();

//	/**
//	 * @brief Jump to the next line
//	 * @details Jumps to the next line of the current appending widget.
//	 * @example This is an example of how to use the newLine function.s
//	 * @code
//	 * @endcode
//	 * @param size       The height of the current line, can be 0
//	 * @param resizeType The resize type of the current line. If set to
//	 * HBWidgetResizeType_ScaleToChildren the height/width gets calculated based
//	 * on the biggest child of the current line. If set to
//	 * HBWidgetResizeType_Fixed the height/width is the size parameter.
//	 */
	HBUI_API void newLine(float size = 0, HBWidgetResizeOptions_ resizeType =
	                                          HBWidgetResizeOptions_ScaleToChildren);

	//---------------------------------------------------------------------------------
	// [SECTION] Basic Panels
	//---------------------------------------------------------------------------------
	HBUI_API bool beginFlexPanel(const std::string &id,
	                             const ImVec2 &position  = {0, 0},
	                             const ImVec2 &size      = {0, 0},
	                             const ImVec2 &cursorPos = {0, 0});
	HBUI_API void endFlexPanel();

	//---------------------------------------------------------------------------------
	// [SECTION] Side And Menu Bars
	//---------------------------------------------------------------------------------

	//	* @deprecated
	//	    * @example @code
	//	        * if (HBUI::beginSideBar(ImGui::GetID("SideBar"), HBSideBarFlags_Horizontal, {510, 510}, {150, 150}, ))
	//	            * {
	//		* 	HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0, 0}, {32, 32});
	//		* 	HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0, 0}, {32, 32});
	//		* 	HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0, 0}, {32, 32});
	//		* 	HBUI::endSideBar();
	//		*  }
	//	*  @endcode

	/**
	 * @brief Begin a new side bar
	 * @param id
	 * @param flags
	 * @param backgroundColor
	 * @param position
	 * @param size
	 * @param padding
	 * @param margin
	 * @return nothing for now
	 */
	    HBUI_API bool beginSideBar(const std::string &id,
	                           const HBSideBarFlags flags,
	                           const ImColor &backgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg),
	                           const ImVec2 &position         = {0, 0},
	                           const ImVec2 &size             = {0, 0},
	                           const ImVec4 &padding          = {0, 0, 0, 0},
	                           const ImVec4 &margin           = {0, 0, 0, 0});

	/**
	 * @brief Begin a new side bar
	 * @param id
	 * @param backgroundColor
	 * @param flags
	 * @param position
	 * @param size
	 * @param cursorPos
	 * @param padding
	 * @param margin
	 * @param label
	 * @param itemFlags
	 * @param drawLocationFlag
	 * @return nothing for now
	 */
	HBUI_API bool beginSideBar(const ImGuiID &id,
	                           const ImColor &backgroundColor,
	                           const HBSideBarFlags flags,
	                           const ImVec2 &position,
	                           ImVec2 size,
	                           const ImVec2 &cursorPos,
	                           const ImVec4 &padding,
	                           const ImVec4 &margin,
	                           const std::string &label,
	                           const HBItemFlags itemFlags,
	                           const HBDrawLocation drawLocationFlag);

	IMGUI_API void endSideBar();

	/**
	 * @brief Begin a new side bar
	 * @param id
	 * @param flags
	 * @param position
	 * @param size
	 * @param cursorPos
	 * @param padding
	 * @param margin
	 * @param label
	 * @param itemFlags
	 * @param drawLocationFlag
	 * @return nothing for now
	 */
	IMGUI_API bool sideBarBarButton(const ImGuiID id, const ImWchar label,
	                                const ImVec2 &position  = {0, 0},
	                                const ImVec2 &size      = {0, 0},
	                                const float fontSize    = ImGui::GetFontSize(),
	                                const ImVec2 &cursorPos = {0, 0},
	                                const ImVec4 padding    = {0, 0, 0, 0},
	                                const ImVec4 margin     = {0, 0, 0, 0},
	                                ImFont *font            = nullptr);

	/**
	 * @brief Begin a new side bar
	 * @param id
	 * @param flags
	 * @param position
	 * @param size
	 * @param cursorPos
	 * @param padding
	 * @param margin
	 * @param label
	 * @param itemFlags
	 * @param drawLocationFlag
	 * @return nothing for now
	 */
	IMGUI_API bool sideBarBarButton(const ImGuiID id, const std::string &label,
	                                const ImVec2 &position  = {0, 0},
	                                const ImVec2 &size      = {0, 0},
	                                const float fontSize    = ImGui::GetFontSize(),
	                                const ImVec2 &cursorPos = {0, 0},
	                                const ImVec4 padding    = {0, 0, 0, 0},
	                                const ImVec4 margin     = {0, 0, 0, 0},
	                                ImFont *font            = nullptr);

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


	//---------------------------------------------------------------------------------
	// [SECTION] Flags
	//---------------------------------------------------------------------------------
	HBUI_API bool isFlagSet(int *flags, int flag);
	HBUI_API void toggleFlag(int flag);


	//---------------------------------------------------------------------------------
	// [SECTION] Fonts
	//---------------------------------------------------------------------------------
	HBUI_API float getWindowSizeDpiScaleFactor();
	HBUI_API float getFontSizeIncreaseFactor();
	HBUI_API ImVec2 getWindowScaleFactor();
	HBUI_API ImFont *getBigFont();

}// namespace HBUI
