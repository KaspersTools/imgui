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
#include <memory>
#include <vector>
#include <map>
#include "Animation/Animation.h"
#include "HBTime.h"

//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations
//-----------------------------------------------------------------------------
typedef int MainWindowFlags; // -> enum HBMainWindowFlags_
typedef int DockspaceFlags;  // -> enum HBDockspaceFlags_
typedef int MainMenuBarFlags;// -> enum HBMainMenuBarFlags_

typedef int HBDrawType;// -> enum HBDrawType_

struct HBContext;

struct HBTime;

struct HBDrawData;
struct HBUIItem;
struct HBCircle;
struct HBRect;
struct HBUpdatable;

//forward function declarationss
namespace HBUI {
  HBUI_API void printVec2(const ImVec2 &vec2, const std::string &name);

  HBUI_API void printVec4(const ImVec4 &vec4, const std::string &name);

  //---------------------------------------------------------------------------------
  // [SECTION] Context
  //---------------------------------------------------------------------------------
  HBUI_API HBContext *getCurrentContext();
}// namespace HBUI

struct HBStyle {
  //Colors
  ImColor menuBarColor             = (163, 255, 0, 255);
  bool    useHBUIStyleMenuBarColor = false;//Use the HBUI style for the mainmenu

  ImVec2 mainMenuBarVerticalFirstItemOffset   = {0, 55}; //Extra offset for the first item in the vertical mainmenubar
  ImVec2 mainMenuBarHorizontalFirstItemOffset = {0, 0};//Extra offset for the first item in the vertical mainmenubar
  ImVec2 mainMenuItemSize                     = {45,
                                                 45};                  //The size of the mainmenu items (width, height) \\ for circles x is used for the radius and y is ignored

  ImColor mainMenuBarItemColor          = (255, 0, 266, 255);//The color/Tint of the mainmenu items
  bool    useHBUIStyleMainMenuItemColor = false;       //Use the HBUI style for the mainmenu items

  ImVec2 mainMenuItemsPadding = {0,
                                 0};//The padding for the mainmenu items (top, right, bottom, left). If not set use imgui frame padding
  ImVec2 mainMenuItemsSpacing = {5,
                                 9};//The spacing for the mainmenu items (top, right, bottom, left). If not set the imgui frame spacing


  void logToTTY() {
    //    ImGui::LogText("HBUI Style" IM_NEWLINE);
    ImGui::LogToTTY();
    HBUI::printVec2(mainMenuBarVerticalFirstItemOffset, "mainMenuBarVerticalFirstItemOffset");
    HBUI::printVec2(mainMenuBarHorizontalFirstItemOffset, "mainMenuBarHorizontalFirstItemOffset");
    HBUI::printVec2(mainMenuItemSize, "mainMenuItemSize");
    HBUI::printVec4(menuBarColor, "menuBarColor");
    HBUI::printVec4(mainMenuBarItemColor, "mainMenuBarItemColor");
    HBUI::printVec2(mainMenuItemsPadding, "mainMenuItemsPadding");
    HBUI::printVec2(mainMenuItemsSpacing, "mainMenuItemsSpacing");
    ImGui::LogFinish();
  }
};

//-----------------------------------------------------------------------------
// [SECTION] Dockspace
//-----------------------------------------------------------------------------
enum HBDockspaceFlags_ {
  HB_DOCKSPACE_FLAG_NONE    = 0,
  HB_DOCKSPACE_FLAG_MENUBAR = 1 << 0,
};

//-----------------------------------------------------------------------------
// [SECTION] MAIN Window
//-----------------------------------------------------------------------------
enum HBMainWindowFlags_ {
  HBUI_MAIN_WINDOW_FLAG_NONE          = 0,
  HBUI_MAIN_WINDOW_FLAG_NO_DECORATION = 1 << 1,
  HBUI_MAIN_WINDOW_FLAG_NO_RESIZE     = 1 << 2,
  HBUI_MAIN_WINDOW_FLAG_NO_MOVE       = 1 << 3,
  HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR   = 1 << 4
};

//-----------------------------------------------------------------------------
// [SECTION] HBIO
//-----------------------------------------------------------------------------
struct HBIO {
  std::string title = "ImVK";

  int width  = 1280;
  int height = 720;

  MainWindowFlags mainWindowFlags = HBUI_MAIN_WINDOW_FLAG_NONE;
};

//-----------------------------------------------------------------------------
// [SECTION] Drawables
//-----------------------------------------------------------------------------
enum HB_Primitive_DrawType_ {
  HB_Draw_Type_Square = 0,
  HB_Draw_Type_Circle = 1,
};

struct HBUpdatable {
public:
  HBUpdatable() {};

public:
  virtual void update(float deltaTime) = 0;
};

typedef int HBItemFlags;
enum HBItemFlags_ {
  HBItemFlags_None       = 0,
  HBItemFlags_Animatable = 1 << 0,  //if animatable it gets saved in the update map in context
};

struct HBUIItem : HBUpdatable {
public:
  HBUIItem(const std::string &id, HBItemFlags flags) {

    ImGuiWindowFlags_NoTitleBar;
    this->idString = id;
    this->id       = ImGui::GetID(id.c_str());
  };

  ~HBUIItem() {};

  //ID
  ImGuiID     id       = 0;
  std::string idString = "";
  HBItemFlags flags    = HBItemFlags_None;

public:
  //call right after the creation, for example in beginMainMenuBar(). Even before the actual draw function is called.
  void beforeDraw() {
    ImGui::PushID(id);
  }

  //call after the draw function is called and the window is closed with ImGui::End()
  void afterDraw() {
    ImGui::PopID();
  }

public:
  virtual bool draw(ImDrawList *drawList, ImColor color, bool drawFilled) = 0;

  void update(float deltaTime) override {

  }

  //gets calculated based on the draw type
  ImVec2 getSpacing() {
    return ImVec2(12, 12);
  }
};

struct HBRect : HBUIItem {
  HBRect(const std::string &id, HBItemFlags itemFlags = HBItemFlags_None) : HBUIItem(id, itemFlags) {
  }

  ~HBRect() {
  }

  //Position
  ImVec2 start = {};
  ImVec2 end   = {};

  bool draw(ImDrawList *drawList, ImColor color, bool drawFilled) override;

  void update(float deltaTime) override;

  ImVec2 StartPos() const {
    return start;
  }

  ImVec2 EndPos() const {
    return end;
  }

  ImVec2 Size() const {
    return ImVec2(end.x - start.x, end.y - start.y);
  }
};


//-----------------------------------------------------------------------------
// [SECTION] Main menubar
//-----------------------------------------------------------------------------
enum HBMainMenuBarFlags_ {
  HB_MAIN_MENU_BAR_FLAG_NONE           = 0,
  HB_MAIN_MENU_BAR_FLAG_HORIZONTAL     = 1 << 0,
  HB_MAIN_MENU_BAR_FLAG_VERTICAL       = 1 << 1,
  HB_MAIN_MENU_BAR_FLAG_USE_HBUI_STYLE = 1 << 2,
  HB_Main_Menu_Bar_Flag_Animated       = 1 << 3,
};

struct HBMenuButton : HBUIItem {
public:
  HBMenuButton(const std::string &label, const HBDrawType drawType,
               const ImVec2 size, HBItemFlags itemFlags = HBItemFlags_None)
      : HBUIItem(label, itemFlags) {
    this->label    = label;
    this->size     = size;
    this->drawType = drawType;
  };

  ~HBMenuButton() {

  };

public:
  std::string label    = "";
  ImVec2      size     = {};
  ImVec2      pos      = {}; //relative to the mainmenu, gets calculated if set to 0,0
  HBDrawType  drawType = HB_Draw_Type_Square;
  ImVec2      spacing  = {-1, -1};

//  ImVec2 getSpacing() const;

public:
  bool draw(ImDrawList *drawList, ImColor color, bool drawFilled) override;

  void update(float deltaTime) override;
};

struct HBMainMenuBar : HBUIItem {
  HBMainMenuBar(const std::string &id,
                HBItemFlags itemFlags = 0,
                MainMenuBarFlags flags = 0,
                ImVec2 windowPos = {},
                ImVec2 windowSize = {},
                HBAnimProps<ImVec2> animProps = {}
  ) : HBUIItem(id, itemFlags),
      flags(flags),
      windowPos(windowPos),
      windowSize(windowSize),
      animProps(animProps) {
  }

  ~HBMainMenuBar() {};

  bool draw(ImDrawList *drawList, ImColor color, bool drawFilled) override;

  void update(float deltaTime) override;

  void append(std::shared_ptr<HBMenuButton> item);

  bool isHorizontal() const;

  bool isVertical() const;

  bool useCustomStyle() const;

  ImColor getColor() const;

  //flags
  MainMenuBarFlags flags = 0;

  //properties //fixme: make height and width a vec2
  ImVec2 windowPos  = {0, 0};//The start position of the MenuBar in window space, if 0 it will be on the cursorPos
  ImVec2 windowSize = {0, 0};//The size of the MenuBar, if 0 it will be calculated based on the items

  bool enabled = true;

  //primitive
  HBRect rect = HBRect(idString + "mainMenuBar", HBItemFlags_None);

  //items
  std::vector<std::shared_ptr<HBMenuButton>> items       = {};
  ImVec2                                     nextItemPos = {};

  //animation
//  HBPanelAnimType_ animType = HB_PANEL_ANIM_TYPE_NONE;
  HBAnimProps<ImVec2> animProps = {};
};

//-----------------------------------------------------------------------------
// [SECTION] Draw Data
//-----------------------------------------------------------------------------
//gets reset every frame
struct HBDrawData {
  DockspaceFlags dockspaceFlags = 0;          //the flags for the dockspace
  ImVec2         dockspaceSize  = {};         //the size of the dockspace
  ImVec2         dockspacePos   = {};         //the position of the dockspace

  std::shared_ptr<HBMainMenuBar>              currentAppendingMenuBar = NULL;//the current appending main menu bar
  std::vector<std::shared_ptr<HBMainMenuBar>> mainMenuBars            = {};//the main menu bars/ Think im goind to just rename it to panels lol

  ImVec2 cursorPos = ImVec2(0, 0);     //used to determine where for example the dockspace should startr
};


//-----------------------------------------------------------------------------
// [SECTION] HBUIContext
//-----------------------------------------------------------------------------
struct HBContext {
  bool    initialized = false;
  HBIO    io          = {};
  HBStyle style       = {};
  HBTime  time        = {};

  //Draw Data
  std::shared_ptr<HBDrawData> drawData = std::make_shared<HBDrawData>();

  //AnimManager
  void update() {
    animManager->update();
  }

  bool hasAnimation(const ImGuiID &id) {
    return animManager->hasAnimation(id);
  }

  bool hasAnimation(const std::string& id){
    return animManager->hasAnimation(ImGui::GetID(id.c_str()));
  }


  ImVec2 addAnimation(const std::string &id, HBAnimProps<ImVec2> props) {
    animManager->addAnimation(id, props);
  }

  std::shared_ptr<HBAnimManager> animManager = std::make_shared<HBAnimManager>();
};

//-----------------------------------------------------------------------------
// [SECTION] HBUI
//-----------------------------------------------------------------------------
namespace HBUI {
  //---------------------------------------------------------------------------
  HBUI_API HBContext *
  initialize(const std::string &title, int width, int height, MainWindowFlags flags);

  HBUI_API void
  setCurrentContext(HBContext *ctx);

  HBUI_API void
  setCurrentImGuiContext(ImGuiContext *ctx);

  HBUI_API void
  clearContext();

  HBUI_API HBStyle &
  getStyle();

  HBUI_API HBIO &
  getIO();

  HBUI_API HBDrawData &
  getDrawData();

  //---------------------------------------------------------------------------------
// [SECTION] Context
//---------------------------------------------------------------------------------
  HBUI_API ImVec2
  getCursorViewportPos();

  HBUI_API ImVec2
  getContentRegionAvail();

  HBUI_API ImVec2
  getWindowSize();

  HBUI_API ImVec2
  getViewportPos();

  HBUI_API ImVec2
  getViewportSize();

  //---------------------------------------------------------------------------------
  // [SECTION] Main Window
  //---------------------------------------------------------------------------------
  HBUI_API void
  setMainWindowFlags(MainWindowFlags flags);

  HBUI_API bool
  isMaximized();

  //---------------------------------------------------------------------------------
  // [SECTION] Input
  //---------------------------------------------------------------------------------
  HBUI_API bool
  mouseOverRect(const ImVec2 &start, const ImVec2 &end);

  HBUI_API bool
  mouseOverCircle(const ImVec2 &center, float radius);

  //---------------------------------------------------------------------------------
  // [SECTION] Widgets & Drawing
  //---------------------------------------------------------------------------------
  HBUI_API HBCircle &
  drawCircle(const HBCircle &circle, ImDrawList *drawList, const ImColor &color);

  HBUI_API HBRect &
  drawRect(const HBRect &rect, ImDrawList *drawList, const ImColor &color, bool drawFilled);

  HBUI_API bool
  drawCircleButton(const HBCircle &circle, ImDrawList *drawList, const ImColor &color);

  HBUI_API bool
  drawRectButton(const HBRect &rect, ImDrawList *drawList, const ImColor &color);

  HBUI_API bool
  drawCircleImageButton(const HBCircle &circle, const ImTextureID &texture, const ImColor &color,
                        ImDrawList *drawList);

  HBUI_API void
  addUpdatable(std::shared_ptr<HBUpdatable> updatable);

  //---------------------------------------------------------------------------------
  // [SECTION] Dockspaces
  //---------------------------------------------------------------------------------
  HBUI_API bool
  beginFullScreenDockspace(HBDockspaceFlags_ flags = HB_DOCKSPACE_FLAG_NONE);

  HBUI_API bool
  beginFullScreenDockspace(const bool isMaximized,
                           const bool mainWindowNoTitlebar,
                           const bool hasMenuBar);

  //---------------------------------------------------------------------------------
  // [SECTION] Menu Bars
  //---------------------------------------------------------------------------------
  HBUI_API bool
  beginMainMenuBar(const std::string &id, ImVec2 windowPos = {}, ImVec2 windowSize = {},
                   HBItemFlags itemFlags = HBItemFlags_None, MainMenuBarFlags flags = 0);

  HBUI_API bool
  beginMainMenuBar(const std::string &id, HBAnimProps<ImVec2> animProp,
                   HBItemFlags itemFlags, MainMenuBarFlags flags
  );

  IMGUI_API void
  endMainMenuBar();//

  IMGUI_API bool
  menuBarButton(const std::string &id, ImVec2 size = HBUI::getStyle().mainMenuItemSize);

  //---------------------------------------------------------------------------------
  // [SECTION] Sample/Debug Windows
  //---------------------------------------------------------------------------------
  HBUI_API void
  showDebugWindow(bool *p_open);

  //---------------------------------------------------------------------------------
  // [SECTION] Updating
  //---------------------------------------------------------------------------------
  HBUI_API void update(float deltatime);

  //---------------------------------------------------------------------------------
  // [SECTION] Animations
  //---------------------------------------------------------------------------------

  //---------------------------------------------------------------------------------
  // [SECTION] Rendering
  //---------------------------------------------------------------------------------
  HBUI_API void
  startFrame();

  HBUI_API void
  endFrame();

  HBUI_API bool
  wantToClose();

  HBUI_API void
  shutdown();

  //---------------------------------------------------------------------------------
  // [SECTION] Input
  //---------------------------------------------------------------------------------s
  HBUI_API bool
  mouseOverRect(const ImVec2 &start, const ImVec2 &end);

  HBUI_API bool
  mouseOverCircle(const ImVec2 &center, float radius);

  HBUI_API bool
  isFlagSet(int *flags, int flag);

  HBUI_API void
  toggleFlag(int flag);

}// namespace HBUI
