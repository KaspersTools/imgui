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

#include "types/HBFloat.h"

#include "Utils/HBTime.h"
#include "Utils/HBMath.h"

#include "Primitives/Primitive.h"

#include "Utils/HBIUpdatable.h"
#include "../../private/src/HBUIEnums.cpp"

#include "Animation/Animations.h"
#include "Animation/Animation.h"

class HBWidgetManager;

struct HBDrawData : public HBIUpdateable {

  ~HBDrawData() {
  }

  void startFrame() override {

  }

  void endFrame() override {

  }
};

struct HBStyle {
  bool useHBUIStyleMenuBarColor = false;  //Use the HBUI style for the menus

  ImVec2 menuItemSide = {45,
                         45};   //The size of the menuitem items (width, height)

  ImVec2 menuItemSpacing = {5,
                            9};    //The spacing for the menu items (top, right, bottom, left). If not set the imgui frame spacing

  ImColor sideBarColor     = (163, 255, 0, 255);
  ImColor sideBarItemColor = (255, 0, 266, 255);     //The color/Tint of the menu items
};
struct HBIO {
  std::string title = "ImVK";

  int width  = 1280;
  int height = 720;

  MainWindowFlags mainWindowFlags = HBMainWindowFlags_None;
};

struct HBContext {
  bool    initialized = false;
  HBIO    io          = {};
  HBStyle style       = {};
  HBTime  time        = {};

  //Draw Data
  std::shared_ptr<HBDrawData>                     drawData      = std::make_shared<HBDrawData>();
  std::shared_ptr<HBUI::Animation::HBAnimManager> animManager   = std::make_shared<HBUI::Animation::HBAnimManager>();
  std::unique_ptr<HBWidgetManager>                widgetManager = std::make_unique<HBWidgetManager>();

  void startFrame();

  void endFrame();

  void update() {
  }

  bool hasAnimation(const ImGuiID &id) {
    return animManager->hasAnimation(id);
  }

  bool hasAnimation(const std::string &id) {
    return animManager->hasAnimation(ImGui::GetID(id.c_str()));
  }

  ImVec2 addAnimation(const std::string &id, HBAnimProps<ImVec2> props) {
    animManager->addAnimation(id, props);
  }
};

namespace HBUI {
  HBUI_API void printVec2(const ImVec2 &vec2, const std::string &name);

  HBUI_API void printVec4(const ImVec4 &vec4, const std::string &name);

  HBUI_API HBContext *getCurrentContext();

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

  HBUI_API HBDrawData *
  getDrawData();

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
  // [SECTION] Panels/Bars
  //---------------------------------------------------------------------------------
  HBUI_API bool
  beginSideBar(
      const ImGuiID &id,
      const HBSideBarFlags flags = 0, //default horizontal
      const ImVec2 &position = {0, 0},
      const ImVec2 &size = {1000, 1000},
      const std::string &label = "",
      const HBDrawLocation itemFlags = HBDrawFlags_ForegroundDrawList
  );

  IMGUI_API void
  endSideBar();

  IMGUI_API bool
  sideBarBarButton(const ImGuiID id,
                   const ImVec2 &position = {0, 0}, const ImVec2 &size = {0, 0});

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
  isFlagSet(int *flags, int flag);

  HBUI_API void
  toggleFlag(int flag);

  //---------------------------------------------------------------------------------
  // [SECTION] IDS
  //---------------------------------------------------------------------------------
  HBUI_API ImGuiID
  getID(const char *str, const char *str_end);

}// namespace HBUI


#include "Utils/HBUIItemBase.h"
#include "Panels/HBPanels.h"
