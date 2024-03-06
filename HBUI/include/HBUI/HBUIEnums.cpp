//
// Created by Kasper de Bruin on 01/03/2024.
//
#include "imgui.h"
#include <string>

//forward declartions
struct ImGuiWindow;
//-----------------------------------------------------------------------------
// [SECTION] Enums
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// [SECTION] Backend Window
//-----------------------------------------------------------------------------
//TODO: Move this to an shared between the backend and the UI source file

typedef int HBBackendWindowFlags;
enum HBBackendWindowFlags_ {
	HBBackendWindowFlags_None         = 0,
	HBBackendWindowFlags_NoDecoration = 1 << 0,
	HBBackendWindowFlags_NoTitleBar   = 1 << 1,
	HBBackendWindowFlags_NoResize     = 1 << 2,
};

//-----------------------------------------------------------------------------
//[SECTION] Properties
//-----------------------------------------------------------------------------
typedef int HBColorPropertiesFlags;
/**
 * @brief The flags of the color properties of the UI element, can be combined with bitwise OR (|)
 * @see HBWidgetColorProperties
 * @def HBColorPropertiesFlags_Default
 */
enum HBColorPropertiesFlags_ : int {
	HBColorPropertiesFlags_None         = 0,
	HBColorPropertiesFlags_NoBackground = 1 << 0,
	HBColorPropertiesFlags_NoBorder     = 1 << 1,
	HBColorPropertiesFlags_NoHover      = 1 << 2,
	HBColorPropertiesFlags_Default      = HBColorPropertiesFlags_NoBorder | HBColorPropertiesFlags_NoHover//with background, no border, no// hover color
};

//-----------------------------------------------------------------------------
// [SECTION] Widgets
//-----------------------------------------------------------------------------

/**
 * @brief The flags of the draw location of the UI element, only use one at a time
 */
enum HBDrawFlags_ : int {
	HBDrawFlags_None     = 0,   //defualt
	HBDrawFlags_OnParent = 1,   //append to parent drawlist
	HBDrawFlags_MainImGuiWindow,//the base window that gets created by HBUI
};

/**
	* @brief The direction of the UI element, only use one at a time
	*/
enum HBDirection_ : int {
	HBDirection_Left = 0,
	HBDirection_Right,
	HBDirection_Up,
	HBDirection_Down
};

/**
 * @brief The type of the layout of the UI element, only use one at a time
 */
enum HBLayoutType_ : int {
	HBLayoutType_None       = 0,
	HBLayoutType_Horizontal = 1,
	HBLayoutType_Vertical   = 2
};

typedef int HBSizeFlags;
/**
 * @brief The flags of the size of the UI element, can be combined with bitwise OR (|)
 */
enum HBSizeFlags_ {
	//both axis
	HBSizeFlags_ScaleToChildren    = 0,//default
	HBSizeFlags_FillAvailableSpace = 1 << 1,
	HBSizeFlags_Fixed              = 1 << 2,
	HBSizeFlags_FullScreen         = 1 << 3,

	//Specific axis
	HBSizeFlags_FillAvailableSpaceX = 1 << 4,
	HBSizeFlags_FillAvailableSpaceY = 1 << 5,
	HBSizeFlags_FixedX              = 1 << 6,
	HBSizeFlags_FixedY              = 1 << 7,
	HBSizeFlags_ScaleToChildrenX    = 1 << 8,
	HBSizeFlags_ScaleToChildrenY    = 1 << 9
};

/**
 * @brief The base type of the UI element, only use one at a timen
 */
enum HBUIBaseType_ : int {
	HBUIBaseType_Unknown,
	HBUIBaseType_Item,
	HBUIBaseType_Window,
	HBUIBaseType_Panel,
	HBUIBaseType_Menu,
};

/**
 * @brief The type of the UI element, only use one at a time
 */
enum HBUIType_ : int {
	HBUIType_None = 0,
	HBUIType_SideBar,
	HBUIType_Button,
	HBUIType_SideBarButton,
	HBUIType_DockSpaceWindow,
};


//-----------------------------------------------------------------------------
// [SECTION] Structs
//-----------------------------------------------------------------------------
struct HBPlatformWindowData {
	ImColor clearColor = ImColor(0, 0, 0, 255);
};
/**
 * @brief The padding of a UI element
 */
struct HBEdgeInset {
	/*(left, top, right, bottom)*/
	float left, top, right, bottom;

	constexpr HBEdgeInset(float all) : left(all), top(all), right(all), bottom(all) {}
	constexpr HBEdgeInset(float leftRight, float topBottom) : left(leftRight), top(topBottom), right(leftRight), bottom(topBottom) {}
	constexpr HBEdgeInset(float left, float top, float right, float bottom) : left(left), top(top), right(right), bottom(bottom) {}

	inline static HBEdgeInset Zero() {
		return HBEdgeInset(0);
	}

	/**
 * @brief Create a HBEdgeInset from the ImGui style
 * @param type The base type of the UI element. It should be an instance of HBUIBaseType.
 * @return HBEdgeInset The created HBEdgeInset object
 */
	inline static HBEdgeInset FromImGuiStyle(HBUIBaseType_ type) {
		return HBEdgeInset::Zero();
	}
};

/**
 * @brief The padding of a UI element
 */
struct HBPadding : public HBEdgeInset {

	constexpr HBPadding(float all) : HBEdgeInset(all) {}
	constexpr HBPadding(float leftRight, float topBottom) : HBEdgeInset(leftRight, topBottom) {}
	constexpr HBPadding(float left, float top, float right, float bottom) : HBEdgeInset(left, top, right, bottom) {}

	/**
	 * @brief Create a HBPadding from the ImGui style
	 * @param type The base type of the UI element. It should be an instance of HBUIBaseType.
	 * @return HBPadding The created HBPadding object
	 */
	inline static HBEdgeInset FromImGuiStyle(HBUIBaseType_ type) {
		ImGuiStyle *style = &ImGui::GetStyle();
		if (style == nullptr)
			return HBEdgeInset::Zero();

		switch (type) {
			case HBUIBaseType_Item:
				HBEdgeInset(style->FramePadding.x, style->FramePadding.y);
				break;
			case HBUIBaseType_Window:
				HBEdgeInset(style->FramePadding.x, style->FramePadding.y);
				break;
			case HBUIBaseType_Menu:
				HBEdgeInset(style->FramePadding.x, style->FramePadding.y);
				break;
			default:
				return HBEdgeInset::Zero();
		}
		return HBEdgeInset::Zero();
	}
};

/**
 * @brief The margin of a UI element
 */
struct HBMargin : public HBEdgeInset {
	constexpr HBMargin(float all) : HBEdgeInset(all) {}
	constexpr HBMargin(float leftRight, float topBottom) : HBEdgeInset(leftRight, topBottom) {}
	constexpr HBMargin(float left, float top, float right, float bottom) : HBEdgeInset(left, top, right, bottom) {}


	/**
	 * @brief Create a HBMargin from the ImGui style
	 * @param type The base type of the UI element. It should be an instance of HBUIBaseType.
	 * @return HBMargin The created HBMargin object
	 */
	inline static HBEdgeInset FromImGuiStyle(HBUIBaseType_ type) {
		ImGuiStyle *style = &ImGui::GetStyle();
		if (style == nullptr)
			return HBEdgeInset::Zero();

		switch (type) {
			case HBUIBaseType_Item:
				HBEdgeInset(style->DisplayWindowPadding.x, style->ItemSpacing.y);
				break;
			case HBUIBaseType_Window:
				HBEdgeInset(style->ItemSpacing.x, style->ItemSpacing.y);
				break;
			case HBUIBaseType_Menu:
				HBEdgeInset(style->ItemSpacing.x, style->ItemSpacing.y);
				break;
			default:
				return HBEdgeInset::Zero();
		}

		return HBEdgeInset::Zero();
	}
};

/**
 * @brief The time struct
 */
struct HBTime {
	inline static float deltaTime = 0.0f;//The delta time
	inline static float lastTime  = 0.0f;//The time of the last frame
	inline static float frameTime = 0.0f;//The time it took to render the last frame
	inline static float timerTime = 0;   //The time that has passed since the timer started

	/**
	 * @brief Initialize the time
	 */
	inline static void init() {
		lastTime = ImGui::GetTime();
	}

	/**
	 * @brief runs on start of a new frame, gets automatically called by HBUI
	 */
	inline static void startFrame() {
		float time = ImGui::GetTime();
		deltaTime  = time - lastTime;
		lastTime   = time;
	}

	/**
	 * @brief runs on end of a new frame, gets automatically called by HBUI
	 */
	inline static void endFrame() {
		frameTime = ImGui::GetTime() - lastTime;
	}

	/**
	 * @brief Start a timer
	 */
	inline static void startTimer() {
		timerTime = ImGui::GetTime();
	}

	/**
	 * @brief End a timer
	 * @return float The time that has passed since the timer started
	 */
	inline static float endTimer() {
		return ImGui::GetTime() - timerTime;
	}
};

/**
 * @brief The IO struct
 */
struct HBIO {
	std::string title = "ImVK";

	int width  = 1280;
	int height = 720;

	float dpiWindowSizeFactor = 1.0f;
	float fontRenderingScale  = 1.0f;
};

template<typename T>
struct HBAnimProps;

namespace HBUI {
	class HBWidgetManager;

	namespace Fonts {
		class FontLoader;
	}

	namespace Animation {
		class HBAnimManager;
	}
}// namespace HBUI

struct HBContext {
	bool initialized = false;
	HBIO io          = {};
	HBTime time      = {};

	HBPlatformWindowData windowData = {};

	ImGuiWindow *mainWindow = nullptr;

	// Draw Data
	HBUI::Animation::HBAnimManager *animManager = nullptr;
	HBUI::HBWidgetManager *widgetManager        = nullptr;
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