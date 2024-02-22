//
// Created by Kasper de Bruin on 22/02/2024.
//

//-----------------------------------------------------------------------------
// [SECTION] HBWidgetManager
//-----------------------------------------------------------------------------
enum HBDrawLocation {
  HBDrawFlags_ForegroundDrawList                = 0, //default
  HBDrawFlags_DrawOnParent                      = 1,
  HBDrawFlags_CreateOwnImGuiWindow              = 2,
  HBDrawFlags_BackgroundDrawList                = 3,
  HBDrawFlags_CurrentViewportForegroundDrawList = 4,
  HBDrawFlags_CurrentViewportBackgroundDrawList = 5,
  HBDrawFlags_CurrentWindowDrawList             = 7,
};

enum HBWidgetResizeType_ {
  HBWidgetResizeType_ScaleToChildren = 0, //default
  HBWidgetResizeType_ScaleToParent,
  HBWidgetResizeType_Fixed
};

typedef int HBSideBarFlags;
enum HBSideBarFlags_ {
  HBSideBarFlags_Horizontal   = 0,      //default
  HBSideBarFlags_Vertical     = 1 << 1,
  HBSideBarFlags_Animated     = 1 << 3,
  HBSideBarFlags_OverviewPort = 1 << 4
};

typedef int HBLayoutType;
enum HBLayoutType_ {
  Horizontal = 0,
  Vertical   = 1
};

typedef int HBUIType;
enum HBUIType_ {
  HBNONE    = 0,
  HBSIDEBAR = 1,
  HBSIDEBARBUTTON,
  HBWINDOW  = 2,
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