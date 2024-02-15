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


//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations
//-----------------------------------------------------------------------------
typedef int MainWindowFlags;  // -> enum HBMainWindowFlags_
typedef int DockspaceFlags;   // -> enum HBDockspaceFlags_
typedef int MainMenuBarFlags; // -> enum HBMainMenuBarFlags_

struct HBContext;

struct HBMainWindowSettings;

struct HBTime;

struct HBDrawData;
struct HBUIItem;
struct HBCircle;
struct HBRect;

struct HBUpdatable;


//-----------------------------------------------------------------------------
// [SECTION] Drawables
//-----------------------------------------------------------------------------
enum HBButtonDrawType {
  Normal,
  Circle,
  Square
};

struct HBUpdatable {
  public:
  HBUpdatable() = default;

  public:
  virtual void update(float deltaTime) = 0;
};

struct HBUIItem : HBUpdatable {
  public:
  HBUIItem(){};

  public:
  //ID
  std::string ID;

  //Color
  ImColor Color = {};

  //Scrolling
  float MinMouseScroll;
  float MaxMouseScroll;
  float CurrentMouseScroll;

  //Children
  std::vector<std::shared_ptr<HBUIItem>> Children = {};

  std::shared_ptr<HBUIItem> getChild(const std::string &id);

  std::shared_ptr<HBUIItem> getLastChild();

  std::shared_ptr<HBUIItem> getLastYPos();

  public:
  void startDraw(ImDrawList *drawList);

  virtual ImVec2 StartPos() const = 0;

  virtual ImVec2 EndPos() const = 0;

  private:
  virtual void draw(ImDrawList *drawList) = 0;

  void drawChildren(ImDrawList *drawList);
};

struct HBCircle : HBUIItem {
  ImVec2 Center;
  float Radius;

  void draw(ImDrawList *drawList) override;

  ImVec2 StartPos() const override;

  ImVec2 EndPos() const override;
};

struct HBRect : HBUIItem {
  //Position
  ImVec2 Start;
  ImVec2 End;

  void draw(ImDrawList *drawList) override;

  ImVec2 StartPos() const override;

  ImVec2 EndPos() const override;
};

struct HBDrawData {
  MainMenuBarFlags mainMenuBarFlags = {};
};

//-----------------------------------------------------------------------------
// [SECTION] Time
//-----------------------------------------------------------------------------
struct HBTime {
  float deltaTime = 0.0f;
  float lastTime  = 0.0f;
  float frameTime = 0.0f;
};

//-----------------------------------------------------------------------------
// [SECTION] Flags & Enumerations
//-----------------------------------------------------------------------------
struct HBPadding{
  float top     =   0;
  float right   =   0;
  float bottom  =   0;
  float left    =   0;

  HBPadding(float top, float right, float bottom, float left) : top(top), right(right), bottom(bottom), left(left) {}
  HBPadding(float topBottom, float rightLeft) : top(topBottom), right(rightLeft), bottom(topBottom), left(rightLeft) {}
  HBPadding(float all) : top(all), right(all), bottom(all), left(all) {}
  HBPadding() = default;
};

struct HBStyle{
  //main window
  HBPadding firstItemPadding          = HBPadding(0, 0, 0, 0);//padding for the first item
  HBPadding firstItemPaddingScreenMax = HBPadding(0, 0, 0, 0);//if the screen is maximized use this padding

  //menuitems
  ImVec2    menuItemSizeButton        = ImVec2(32, 32);
};

//-----------------------------------------------------------------------------
// [SECTION] Main menubar
//-----------------------------------------------------------------------------
enum HBMainMenuBarFlags_ {
  HB_MAIN_MENU_BAR_FLAG_NONE       = 0,
  HB_MAIN_MENU_BAR_FLAG_HORIZONTAL = 1 << 0,
  HB_MAIN_MENU_BAR_FLAG_VERTICAL   = 1 << 1,
};

struct HBMainMenuItem{
  HBUIItem *item;
  std::string name;
  //todo: add image
};

//-----------------------------------------------------------------------------
// [SECTION] Dockspace
//-----------------------------------------------------------------------------
enum HBDockspaceFlags_ {
  HB_DOCKSPACE_FLAG_None      = 0,
  HB_DOCKSPACE_FLAG_MENUBAR   = 1 << 0,
};

//-----------------------------------------------------------------------------
// [SECTION] MainWindowSettings
//-----------------------------------------------------------------------------
enum HBMainWindowFlags_ {
  HBUI_MAIN_WINDOW_FLAG_NONE                =   0,
  HBUI_MAIN_WINDOW_FLAG_NO_DECORATION       =   1 << 1,
  HBUI_MAIN_WINDOW_FLAG_NO_RESIZE           =   1 << 2,
  HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR         =   1 << 4
};

struct HBMainWindowSettings {
};

//-----------------------------------------------------------------------------
// [SECTION] HBIO
//-----------------------------------------------------------------------------
struct HBIO{
  std::string           title               = "ImVK";
  int                   width               = 1280;
  int                   height              = 720;

  HBMainWindowSettings* mainWindowSettings  = {};
  MainWindowFlags       mainWindowFlags     = HBUI_MAIN_WINDOW_FLAG_NONE;
};

//-----------------------------------------------------------------------------
// [SECTION] HBUIContext
//-----------------------------------------------------------------------------
struct HBContext {
  bool    initialized   = false;
  HBIO    io            = {};
  HBStyle style         = {};

  HBTime  time          = {};

  //Draw Data
  std::shared_ptr<HBDrawData> drawData = std::make_shared<HBDrawData>();
};

//-----------------------------------------------------------------------------
// [SECTION] HBUI
//-----------------------------------------------------------------------------
namespace HBUI {
  HBUI_API HBContext *
  initialize(const std::string &title, int width, int height, MainWindowFlags flags);

  HBUI_API void
  setCurrentContext(HBContext *ctx);

  HBUI_API void
  setCurrentImGuiContext(ImGuiContext *ctx);

  HBUI_API HBContext *
  getCurrentContext();

  HBUI_API void
  clearContext();

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
  HBUI_API void
  beginFullScreenDockspace(DockspaceFlags flags = HB_DOCKSPACE_FLAG_None);

  HBUI_API void
  beginFullScreenDockspace(const bool isMaximized,
                           const bool mainWindowNoTitlebar,
                           const bool hasMenuBar);

  HBUI_API void endFullScreenDockspace();

  //---------------------------------------------------------------------------------
  // [SECTION] Menu Bars
  //---------------------------------------------------------------------------------
  HBUI_API bool
  beginMainMenuBar(MainMenuBarFlags flags);                           // create and append to a full screen menu-bar.

  IMGUI_API void
  endMainMenuBar();                                                   // only call EndMainMenuBar() if BeginMainMenuBar() returns true!

  IMGUI_API bool
  beginMainMenuItem(const std::string &name);

  //---------------------------------------------------------------------------------
  // [SECTION] Sample/Debug Windows
  //---------------------------------------------------------------------------------
  HBUI_API void
  showDebugWindow(bool *p_open = NULL);

  //---------------------------------------------------------------------------------
  // [SECTION] Updating
  //---------------------------------------------------------------------------------
  HBUI_API float getDeltaTime();
  HBUI_API float getFrameTime();
  HBUI_API float getTime();

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
  //---------------------------------------------------------------------------------
  HBUI_API bool
  mouseOverRect(const ImVec2 &start, const ImVec2 &end);

  HBUI_API bool
  mouseOverCircle(const ImVec2 &center, float radius);

  HBUI_API bool
  isFlagSet(int *flags, int flag);

  HBUI_API void
  toggleFlag(int flag);


}// namespace HBUI
