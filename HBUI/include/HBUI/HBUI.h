//
// Created by Kasper de Bruin on 06/02/2024.
//
#pragma once
#ifndef HBUI_API
#define HBUI_API
#endif

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <iostream>
#include <memory>
#include <vector>

typedef int MainWindowFlags; // -> enum ImGuiWindowFlags_     // Flags: for Begin(), BeginChild()
typedef int HBMenuBarFlags;  // -> enum HBMenuBarFlags_  // Flags for MenuBar
typedef int HBDockSpaceFlags;// -> enum HBDockSpaceFlags_  // Flags for DockSpace

//-----------------------------------------------------------------------------
// [SECTION] Flags & Enumerations
//-----------------------------------------------------------------------------
enum HBButtonDrawType {
  Normal,
  Circle,
  Square
};

enum MainWindowFlags_ {
  HBUI_MAIN_WINDOW_FLAG_NONE = 0,
  HBUI_MAIN_WINDOW_FLAG_RESIZABLE = 1 << 0,
  HBUI_MAIN_WINDOW_FLAG_DECORATED = 1 << 1,
  HBUI_MAIN_WINDOW_FLAG_TITLEBAR = 1 << 2,//fixme: implement but not with glfwWindowHint, only with glfwSetWindowAttrib
  HBUI_MAIN_WINDOW_FLAG_TRANSPARENT = 1 << 3,
  HBUI_MAIN_WINDOW_FLAG_CENTER_WINDOW = 1 << 4,
  //  HBUI_MAIN_WINDOW_FLAGS_VSYNC = 1 << 5, //todo: implement VSYNC
};

struct MainWindowSettings {
  std::string title = "ImVK";

  int width = 1280;
  int height = 720;

  MainWindowFlags flags =
          HBUI_MAIN_WINDOW_FLAG_RESIZABLE |
          HBUI_MAIN_WINDOW_FLAG_DECORATED |
          HBUI_MAIN_WINDOW_FLAG_TITLEBAR |
          HBUI_MAIN_WINDOW_FLAG_TRANSPARENT |
          HBUI_MAIN_WINDOW_FLAG_CENTER_WINDOW;

  //default constructor
  MainWindowSettings() = default;

  //copy constructor
  MainWindowSettings(const MainWindowSettings &other) = default;

  //constructor
  MainWindowSettings(const std::string &title, int width, int height) : title(title), width(width), height(height) {}
};

struct HBUIContext {
  public:
  //MainWindow
  MainWindowSettings *mainWindowSettings = nullptr;
  //Implementation Data
  void *implementationData = nullptr;

  //ImGui Stuff
  ImGuiContext *imguiContext = nullptr;

  //Constructor
  HBUIContext() = default;
};


//-----------------------------------------------------------------------------
// [SECTION] HBUIItem
//-----------------------------------------------------------------------------
struct HBUIItem {
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
  initialize();

  HBUI_API ImVec2
  getNativeWindowSize();

  HBUI_API ImVec2
  getNativeWindowPos();

  HBUI_API void
  setCurrentContext(HBUIContext *ctx);

  HBUI_API void
  setCurrentImGuiContext(ImGuiContext *ctx);

  HBUI_API HBUIContext *
  getCurrentContext();

  HBUI_API void
  clearContext();

  //---------------------------------------------------------------------------------
  // [SECTION] Input
  //---------------------------------------------------------------------------------
  HBUI_API bool
  mouseOverRect(const ImVec2 &start, const ImVec2 &end);

  HBUI_API bool
  mouseOverCircle(const ImVec2 &center, float radius);

  //---------------------------------------------------------------------------------
  // [SECTION] Widgets Drawing
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
  beginDockspaceWindow(ImGuiViewport *viewport, ImGuiStyle &style, const bool isMaximized);

  HBUI_API void
  endDockspaceWindow();

  HBUI_API void
  endVerticalMenuBar();

  HBUI_API void
  startFrame();

  HBUI_API void
  endFrame();

  HBUI_API bool
  wantToClose();

  HBUI_API void
  shutdown();
}// namespace HBUI
