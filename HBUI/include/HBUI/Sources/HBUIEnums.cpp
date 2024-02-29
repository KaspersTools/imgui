//
// Created by Kasper de Bruin on 22/02/2024.
//

#include <imgui.h>

//-----------------------------------------------------------------------------
// [SECTION] HBWidgetManager
//-----------------------------------------------------------------------------
enum HBDrawLocation {
	HBDrawFlags_NonDrawable = 0,
	HBDrawFlags_MainImguiWindowDrawlist,//the base window that gets created by HBUI
	HBDrawFlags_DrawOnParent,
	HBDrawFlags_CreateOwnImGuiWindow,
	HBDrawFlags_BackgroundDrawList,
	HBDrawFlags_CurrentViewportForegroundDrawList,
	HBDrawFlags_CurrentViewportBackgroundDrawList,
	HBDrawFlags_CurrentWindowDrawList,
};

enum HBWidgetResizeOptions_ {
	HBWidgetResizeOptions_ScaleToChildren = 0,//default
	HBWidgetResizeOptions_Fixed
};

typedef int HBSideBarFlags;
enum HBSideBarFlags_ {
	HBSideBarFlags_None       = 0,
	HBSideBarFlags_Horizontal = 1 << 1,//default
	HBSideBarFlags_Vertical   = 1 << 2,
	HBSideBarFlags_Animated   = 1 << 3,
	HBSideBarFlags_FullSize   = 1 << 4,//if horizontal then full width, if vertical then full height
};

enum HBLayoutType_ {
	HBLayoutType_None       = 0,
	HBLayoutType_Horizontal = 1,
	HBLayoutType_Vertical   = 2
};

typedef int HBItemFlags;
enum HBItemFlags_ {
	HBItemFlags_None                         = 0,
	HBItemFlags_DrawChildrenCentered         = 1 << 1,
	HBItemFlags_ResizeChildrenToBiggestChild = 1 << 2,
};

typedef int HBUIType;
enum HBUIType_ {
	HBUIType_None = 0,
	HBUIType_SideBar,
	HBUIType_SideBarButton,
	HBUIType_NewLine,
};

//-----------------------------------------------------------------------------
// [SECTION] MAIN Window
//-----------------------------------------------------------------------------
typedef int MainWindowFlags;
enum HBMainWindowFlags_ {
	HBMainWindowFlags_None         = 0,
	HBMainWindowFlags_NoDecoration = 1 << 1,
	HBMainWindowFlags_NoResize     = 1 << 2,
	HBMainWindowFlags_NoMove       = 1 << 3,
	HBMeinWindowFlags_NoTitleBar   = 1 << 4
};

//-----------------------------------------------------------------------------
// [SECTION] Time
//-----------------------------------------------------------------------------
struct HBTime {
	inline static float deltaTime = 0.0f;
	inline static float lastTime  = 0.0f;
	inline static float frameTime = 0.0f;

	inline static float timerTime = 0;

	inline static void init() {
		lastTime = ImGui::GetTime();
	}

	inline static void startFrame() {
		float time = ImGui::GetTime();
		deltaTime  = time - lastTime;
		lastTime   = time;
	}

	inline static void endFrame() {
		frameTime = ImGui::GetTime() - lastTime;
	}

	inline static void startTimer() {
		timerTime = ImGui::GetTime();
	}

	inline static float endTimer() {
		return ImGui::GetTime() - timerTime;
	}
};


//-----------------------------------------------------------------------------
// [SECTION] Fonts
//-----------------------------------------------------------------------------
typedef unsigned int HBFontID;// A unique ID used by widgets (typically the result of hashing a stack of string)
enum HBFontStyle_ {
	HBFontStyle_Regular,
	HBFontStyle_Bold,
	HBFontStyle_Italic,
	HBFontStyle_BoldItalic
};
