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
typedef int MainWindowFlags;// -> enum ImGuiWindowFlags_     // Flags: for Begin(), BeginChild()
typedef int TitleBarFlags;  // -> enum ImGuiWindowFlags_     // Flags: for Begin(), BeginChild()

struct HBUIContext;

struct MainWindowSettings;
struct TitleBarSettings;

struct Time;

struct HBUIDrawData;
struct HBUIItem;
struct HBCircle;
struct HBRect;

struct Updatable;

//-----------------------------------------------------------------------------
// [SECTION] Flags & Enumerations
//-----------------------------------------------------------------------------
//draw types
enum HBButtonDrawType {
  Normal,
  Circle,
  Square
};

enum MainWindowFlags_ {
  HBUI_MAIN_WINDOW_FLAG_NONE = 0,
  HBUI_MAIN_WINDOW_FLAG_NO_DECORATION = 1 << 1,
  HBUI_MAIN_WINDOW_FLAG_NO_RESIZE = 1 << 2,
  HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR = 1 << 4,
  HBUI_MAIN_WINDOW_FLAG_CUSTOM_TITLEBAR = 1 << 5,
  HBUI_MAIN_WINDOW_FLAG_DEFAULT_DOCKSPACE = 1 << 3,
};

//titlebar
enum TitleBarFlags_ {
  TitleBarFlag_None = 0,
  TitleBarFlag_Horizontal = 1 << 0,
  TitleBarFlag_Vertical = 1 << 1,
};

struct TitleBarSettings {
  TitleBarFlags flags = 0;
  ImVec2 size = ImVec2(0, 0);
  ImVec2 pos = ImVec2(0, 0);
  ImColor color = ImColor(0, 0, 0, 0);
};

//Main window
struct MainWindowSettings {
  std::string title = "ImVK";
  int width = 1280;
  int height = 720;

  //Flags
  MainWindowFlags flags = 0;

  //TitleBar
  TitleBarFlags titleBarFlags = 0;
  TitleBarSettings titleBarSettings = {};

  //default constructor
  MainWindowSettings(const std::string &title, int width, int height, MainWindowFlags flags = 0,
                     TitleBarFlags titleBarFlags = 0, const TitleBarSettings titlebarSettings = {}) : width(width), height(height), flags(flags), titleBarFlags(titleBarFlags) {
  }

  //constructor
  MainWindowSettings(const std::string &title, int width, int height) : title(title), width(width), height(height) {}
};

//-----------------------------------------------------------------------------
// [SECTION] Time
//-----------------------------------------------------------------------------
struct Time {
  float deltaTime = 0.0f;
  float lastTime = 0.0f;
  float frameTime = 0.0f;
};

//-----------------------------------------------------------------------------
// [SECTION] HBUIContext
//-----------------------------------------------------------------------------
struct HBUIDrawData {
  ImVec2 nextItemPos = ImVec2(0, 0);// Next item position
};

struct HBUIContext {
  public:
  //=============================================================================
  // [SECTION] Getters
  //=============================================================================
  MainWindowFlags &
  windowF() const {
    return mainWindowSettings->flags;
  }

  TitleBarSettings &
  titleBarS() const {
    return mainWindowSettings->titleBarSettings;
  }

  TitleBarFlags &
  titleBarF() const {
    return mainWindowSettings->titleBarFlags;
  }

  public:
  //=============================================================================
  // [SECTION] Constructors
  //=============================================================================
  //Constructor
  HBUIContext() = default;

  public:
  //=============================================================================
  // [SECTION] Settings
  //=============================================================================
  //MainWindow
  MainWindowSettings *mainWindowSettings = nullptr;

  //=============================================================================
  // [SECTION] Drawing and backend
  //=============================================================================
  //Implementation Data
  void *implementationData = nullptr;

  //ImGui Stuff
  ImGuiContext *imguiContext = nullptr;

  //Draw Data
  HBUIDrawData drawData = {};

  public:
  //=============================================================================
  // [SECTION] Update
  //=============================================================================
  //updatables
  std::vector<std::shared_ptr<Updatable>> updatables = {};

  //Time
  Time time = {};
};

//-----------------------------------------------------------------------------
// [SECTION] HBUIItem
//-----------------------------------------------------------------------------
struct Updatable {
  public:
  Updatable() = default;

  public:
  virtual void update(float deltaTime) = 0;
};

struct HBUIItem : Updatable {
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

//-----------------------------------------------------------------------------
// [SECTION] HBUI
//-----------------------------------------------------------------------------

namespace HBUI {
  HBUI_API HBUIContext *
  initialize(const std::string &title, int width, int height, MainWindowFlags flags);

  HBUI_API void
  setCurrentContext(HBUIContext *ctx);

  HBUI_API void
  setCurrentImGuiContext(ImGuiContext *ctx);

  HBUI_API HBUIContext *
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
  addUpdatable(std::shared_ptr<Updatable> updatable);

  //---------------------------------------------------------------------------------
  // [SECTION] Dockspaces
  //---------------------------------------------------------------------------------
  HBUI_API void
  beginFullScreenDockspace();

  HBUI_API void
  beginFullScreenDockspace(const bool isMaximized,
                           const bool hasCustomTitlebar,
                           const bool mainWindowNoTitlebar);
          //---------------------------------------------------------------------------------
  // [SECTION] Menu Bars
  //---------------------------------------------------------------------------------
  HBUI_API void
  beginMainMenuBar();

  HBUI_API void
  endMainMenuBar();

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

  HBUI_API bool isFlagSet(int *flags, int flag);

  HBUI_API void toggleFlag(int flag);


}// namespace HBUI
