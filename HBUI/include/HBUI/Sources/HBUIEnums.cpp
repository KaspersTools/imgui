//
// Created by Kasper de Bruin on 22/02/2024.
//

#include "imgui.h"

//-----------------------------------------------------------------------------
// [SECTION] HBWidgetManager
//-----------------------------------------------------------------------------
enum HBDrawLocation {
  HBDrawFlags_NonDrawable        = 0,
  HBDrawFlags_ForegroundDrawList,
  HBDrawFlags_DrawOnParent,
  HBDrawFlags_CreateOwnImGuiWindow,
  HBDrawFlags_BackgroundDrawList,
  HBDrawFlags_CurrentViewportForegroundDrawList,
  HBDrawFlags_CurrentViewportBackgroundDrawList,
  HBDrawFlags_CurrentWindowDrawList,
};

enum HBWidgetResizeType_ {
  HBWidgetResizeType_ScaleToChildren = 0, //default
  HBWidgetResizeType_ScaleToParent,
  HBWidgetResizeType_Fixed
};

typedef int HBSideBarFlags;
enum HBSideBarFlags_ {
  HBSideBarFlags_None       = 0,
  HBSideBarFlags_Horizontal = 1 << 1,      //default
  HBSideBarFlags_Vertical   = 1 << 2,
  HBSideBarFlags_Animated   = 1 << 3,
  HBSideBarFlags_FullSize   = 1 << 4, //if horizontal then full width, if vertical then full height
};

enum HBLayoutType_ {
  HBLayoutType_None       = 0,
  HBLayoutType_Horizontal = 1,
  HBLayoutType_Vertical   = 2
};

typedef int HBUIType;
enum HBUIType_ {
  HBNONE = 0,
  HBSIDEBAR,
  HBSIDEBARBUTTON,
  HBNEWLINE,
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

  inline static void init() {
    lastTime = ImGui::GetTime();
  }

  inline static void startFrame() {
    float time = ImGui::GetTime();
    deltaTime = time - lastTime;
    lastTime  = time;
  }

  inline static void endFrame() {
    frameTime = ImGui::GetTime() - lastTime;
  }
};
