//
// Created by Kasper de Bruin on 01/03/2024.
//
#include "imgui.h"
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

//forward declartions
struct ImGuiWindow;
//-----------------------------------------------------------------------------
// [SECTION] Enums
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// [SECTION] Backend Window
//-----------------------------------------------------------------------------
typedef int HBBackendWindowFlags;
enum HBBackendWindowFlags_ : int {
  HBBackendWindowFlags_None         = 0,
  HBBackendWindowFlags_NoDecoration = 1 << 0,
  HBBackendWindowFlags_NoTitleBar   = 1 << 1,
  HBBackendWindowFlags_NoResize     = 1 << 2,
};

//-----------------------------------------------------------------------------
// [SECTION] Styles
//-----------------------------------------------------------------------------
typedef int HBStyleFlags;
/**
 * @brief The flags of the style of the UI element, can be combined with bitwise OR (|)
 * @see HBWidgetStyle
 * @def HBStyleFlags_Default
 */
enum HBStyleFlags_ : int {
  HBStyleFlags_None                  = 0,
  HBStyleFlags_StretchHorizontal     = 1 << 0,
  HBStyleFlags_StretchVertical       = 1 << 1,
  HBStyleFlags_NoBackground          = 1 << 2,
  HBStyleFlags_NoBorder              = 1 << 3,
  HBStyleFlags_CanHover              = 1 << 4,
  HBStyleFlags_CenterItemsHorizontal = 1 << 5,
  HBStyleFlags_CenterItemsVertical   = 1 << 6,
};

//-----------------------------------------------------------------------------
// [SECTION] Buttons
//-----------------------------------------------------------------------------
enum HBButtonType_ : int {
  HBButtonType_Text,
  HBButtonType_Icon,
};

enum HBButtonState_ : int {
  HBButtonState_None,
  HBButtonState_Hovered,
  HBButtonState_Clicked,
  HBButtonState_Active,
};

//-----------------------------------------------------------------------------
// [SECTION] Widgets
//-----------------------------------------------------------------------------

enum HBDrawCallFunctionLocation : int {
  //  HBDrawCallFunctionLocation_End   = 0,
  //  HBDrawCallFunctionLocation_Begin = 1,
};

typedef int HBUIWindowFlags;
enum HBUIWindowFlags_ : int {
  HBUIWindowFlags_None           = 0,
  HBUIWindowFlags_HasTaskBar     = 1 << 0,
  HBUIWindowFlags_HasDock        = 1 << 1,
  HBUIWindowFlags_NoOuterBorders = 1 << 2,
  HBUIWindowFlags_MainWindow     = 1 << 4,
};

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
  HBDirection_LeftToRight = 0,
  HBDirection_RightToLeft,
  HBDirection_TopToBottom,
  HBDirection_BottomToTop
};

/**
 * @brief The type of the layout of the UI element, only use one at a time
 */
enum HBLayoutType_ : int {
  HBLayoutType_None       = 0,
  HBLayoutType_Horizontal = 1,
  HBLayoutType_Vertical   = 2
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
  HBUIType_TaskBar,
  HBUIType_Button,
  HBUIType_SideBarButton,
  HBUIType_Window,
};

//-----------------------------------------------------------------------------
// [SECTION] Fonts
//-----------------------------------------------------------------------------
typedef int HBLoadFontFlags;
enum HBLoadFontFlags_ : int {
  HBLoadFontFlags_None = 0,
  HBLoadFontFlags_LoadDefaultFonts,
  HBLoadFontFlags_LoadDefaultIcons,
  HBLoadFontFlags_LoadDefaultTextGlyphs,
  HBLoadFontFlags_ActivateFont
};

typedef int HBActivateFontFlags;
enum HBActivateFontFlags_ : int {
  HBActivateFontFlags_None = 0,
  HBActivateFontFlags_LoadIfNotFound,
};
//-----------------------------------------------------------------------------
// [SECTION] Structs
//-----------------------------------------------------------------------------
struct HBPlatformWindowData {
  ImColor ClearColor = ImColor(0, 0, 0, 255);
};


//-----------------------------------------------------------------------------
// [SECTION] Fonts
//-----------------------------------------------------------------------------


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

enum HBMouseButtons_ : int {
  HBMouseButtons_Left = 0,
  HBMouseButtons_Right,
  HBMouseButtons_Middle,
  HBMouseButtons_COUNT
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

  ImVec2 mousePos                        = ImVec2(-1, -1);
  bool   mouseDown[HBMouseButtons_COUNT] = {false, false, false};
};

template<typename T>
struct HBAnimProps;

namespace HBUI {
  class HBWidgetManager;
  namespace Debuggers {

    struct widgetDebugData {
      HBUIType_    c_Type = HBUIType_None;
      ImGuiID      c_ID;
      bool         c_CanBeOwnParent;
      ImVec2       c_InputSize;
      std::string  c_Label;
      ImVec2       m_Pos;
      ImVec2       m_ScreenPos;
      ImVec2       m_StartCursorPos;
      ImVec2       m_CursorPos;
      HBDirection_ m_Direction;
      ImVec2       m_BottomToTopCursorPos;
      ImVec2       m_TopToBottomCursorPos;
      ImVec2       m_LeftToRightCursorPos;
      ImVec2       m_RightToLeftCursorPos;
      bool         m_HasBegun;
      bool         m_IsEnded;
      HBStyleFlags m_StyleFlags;

      ImGuiID m_ParentID = -1;
    };
    class HBWidgetDebugger;
  }// namespace Debuggers


  namespace Fonts {
    class FontLoader;
    struct HBIcon;
  }

  namespace Animation {
    class HBAnimManager;
  }
}// namespace HBUI

namespace HBUI {
  class HBIWidget;
//  namespace Windows {
//    class HBWindow;
//  }// namespace Windows
}// namespace HBUI


//-----------------------------------------------------------------------------
// [SECTION] Style
//-----------------------------------------------------------------------------
//enum HBCol_ {
//  HBCol_Count
//};
struct HBStyle {
//  ImVec2 FramePadding = getFramePadding();

  ImVec2 IconSize        = ImVec2(16, 16);
  ImVec2 TaskBarIconSize = ImVec2(55, 55);
  ImVec2 getFramePadding() {
    //    if (_framePadding != ImVec2(0, 0)) {
    //      return _framePadding;
    //    }
    ImGuiStyle *style = &ImGui::GetStyle();
    return {style->FramePadding.x, style->FramePadding.y};
  }
private:
//  ImVec2 _framePadding = ImVec2(0, 0);

};

//-----------------------------------------------------------------------------
// [SECTION] Context
//-----------------------------------------------------------------------------
namespace HBUI::Windows {
  class HBWindow;
}

struct HBContext {

  friend class HBUI::Windows::HBWindow;

public:
  bool initialized      = false;
  bool firstFrameCalled = false;
  bool startFrameCalled = false;
  bool endFrameCalled   = true;//initialized as true so that the first frame can start

  HBIO   io   = {};
  HBTime time = {};

  HBStyle *style = nullptr;

  HBPlatformWindowData windowData = {};

  HBUI::HBIWidget *currentAppingWindow = nullptr;

  // Draw Data
  HBUI::Animation::HBAnimManager *animManager   = nullptr;
  HBUI::HBWidgetManager          *widgetManager = nullptr;
  HBUI::Fonts::FontLoader        *fontLoader    = nullptr;

  //States
  std::map<ImGuiID, HBButtonState_> buttonStates;

  // clang-format off
  std::string mainWindowTitle;
  HBStyleFlags mainWindowStyleFlags =
        HBStyleFlags_StretchHorizontal
      | HBStyleFlags_StretchVertical
      | HBStyleFlags_NoBorder
      ;
  HBUIWindowFlags mainWindowFlags =
      HBUIWindowFlags_MainWindow
      | HBUIWindowFlags_HasTaskBar
      | HBUIWindowFlags_HasDock
      | HBUIWindowFlags_NoOuterBorders
      ;

  ImGuiWindowFlags imGuiMainWindowFlags =
        ImGuiWindowFlags_NoDocking
      | ImGuiWindowFlags_NoTitleBar
      | ImGuiWindowFlags_NoCollapse
      | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoBringToFrontOnFocus
      | ImGuiWindowFlags_NoNavFocus
      //
      ;
  // clang-format on

private:
  HBUI::Windows::HBWindow *mainWindow = nullptr;//used as fallback window is the same size as the native window worksize

public:
  //Debugger
  HBUI::Debuggers::HBWidgetDebugger *widgetDebugger = nullptr;

  ~HBContext();

  void initialize();
  void afterBackendInitialized();

  void startFrame();
  void endFrame();

  HBButtonState_ getButtonState(ImGuiID id);

  [[nodiscard]] HBUI::Windows::HBWindow &getMainWindow() const;
  [[maybe_unused]] void                  setMainWindowAsCurrent();

  [[maybe_unused]] void startWidget(HBUI::HBIWidget *widget);
  [[maybe_unused]] void endCurrentWidget();

  void update() {}
};

struct HBIcon {
  std::string name;
  std::string clazz;
};