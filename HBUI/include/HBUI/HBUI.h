//
// Created by Kasper de Bruin on 06/02/2024.
//

#pragma once
#include "../../include/HBUI/HBUIEnums.cpp"

#ifndef HBUI_API
#define HBUI_API
#endif

#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>

#include "Fonts/IconsNerdFont.h"

namespace HBUI {
	HBUI_API std::string wchar32ToUtf8(const ImWchar &wchar);

	HBUI_API HBContext *getCurrentContext();

	HBUI_API HBContext *initialize(const std::string &title,
	                               int width, int height,
	                               int HBBackendWindowFlags = 0);

	HBUI_API void afterBackendInitialized();

	HBUI_API HBIO &getIO();

	//---------------------------------------------------------------------------------
	// [SECTION] Main Window
	//---------------------------------------------------------------------------------
	HBUI_API ImGuiWindow *getMainImGuiWindow();

	//(imgui fullscreen) Window
	HBUI_API ImVec2 getMainWindowSize();
	HBUI_API ImVec2 getMainWindowPos();
	HBUI_API float getMainWindowDpiScale();
	HBUI_API ImVec2 getMainWindowDpiScaleFactor();

	//(backend) Window
	//(backend) Window
	HBUI_API ImVec2 getNativeWindowPos();
	HBUI_API ImVec2 getNativeWindowSize();
	HBUI_API ImColor& getNativeWindowClearColor();
	HBUI_API void setNativeWindowClearColor(const ImColor &color);

	//ImGui Viewports
	HBUI_API ImGuiViewport *getCurrentViewport();
	HBUI_API ImGuiViewport *getMainViewport();

	HBUI_API ImVec2 getViewportPos(ImGuiViewport *viewport);
	HBUI_API ImVec2 getMainViewportPos();
	HBUI_API ImVec2 getMainViewportSize();
	HBUI_API ImVec2 getCurrentViewportPos();

	//---------------------------------------------------------------------------------
	// [SECTION] Cursor
	//---------------------------------------------------------------------------------
	HBUI_API ImVec2 getCursorPos();
	HBUI_API ImVec2 getCursorScreenPos();
	//todo:	HBUI_API ImVec2 getCursorScreenPos(ImGuiViewport *viewport);

	HBUI_API ImVec2 getContentRegionMaxMainWindow();
	HBUI_API ImVec2 getContentRegionAvailMainWindow();
	//todo: HBUI_API ImVec2 getContentRegionAvail(ImGuiViewport *viewport);

	HBUI_API HBLayoutType_ getCurrentLayout();
	HBUI_API void setLayout(HBLayoutType_ layoutType);

	//---------------------------------------------------------------------------------
	// [SECTION] Widgets
	//---------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	// [SECTION] Helpers
	//---------------------------------------------------------------------------------
	HBUI_API HBUIBaseType_ typeToBaseType(HBUIType_ type);

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
	HBUI_API bool beginSideBar(const std::string &id);
	IMGUI_API void endSideBar();

	IMGUI_API bool sideBarBarButton(const ImGuiID id, const ImWchar label,
	                                const ImVec2 &position  = {0, 0},
	                                const ImVec2 &size      = {0, 0},
	                                const float fontSize    = ImGui::GetFontSize(),
	                                const ImVec2 &cursorPos = {0, 0},
	                                const ImVec4 padding    = {0, 0, 0, 0},
	                                const ImVec4 margin     = {0, 0, 0, 0},
	                                ImFont *font            = nullptr);
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

	//---------------------------------------------------------------------------------
	// [SECTION] Fonts
	//---------------------------------------------------------------------------------
	HBUI_API float getWindowSizeDpiScaleFactor();
	HBUI_API float getFontSizeIncreaseFactor();
	HBUI_API ImVec2 getWindowScaleFactor();
	HBUI_API ImFont *getBigFont();

}// namespace HBUI