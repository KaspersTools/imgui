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
class IWidgetBase;

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

  HBUI_API bool isMaximized();



  /**
   * @brief Jump to the next line
   * @details Jumps to the next line of the current appending widget.
   * @example This is an example of how to use the newLine function.s
   * @code
   *  if (HBUI::beginSideBar(ImGui::GetID("SideBar"), HBSideBarFlags_Horizontal, {510, 510}, {150, 150}, )) {
   *     HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0, 0}, {32, 32});
   *     HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0, 0}, {32, 32});
   *     HBUI::newLine();
   *     HBUI::sideBarBarButton(ImGui::GetID("BTN1"), {0, 0}, {32, 32});
   *     HBUI::endSideBar();
   *  }
   * @endcode
   * @param size       The height of the current line, can be 0
   * @param resizeType The resize type of the current line. If set to HBWidgetResizeType_ScaleToChildren the height/width gets calculated based on the biggest child of the current line.
   *                        If set to HBWidgetResizeType_Fixed the height/width is the size parameter.
   */
  HBUI_API void newLine(float size = 0,
                        HBWidgetResizeType_ resizeType = HBWidgetResizeType_ScaleToChildren);

  //---------------------------------------------------------------------------------
  // [SECTION] Panels/Bars
  //---------------------------------------------------------------------------------
  HBUI_API bool
  beginSideBar(
      const ImGuiID &id,
      const HBSideBarFlags flags = 0, //default horizontal
      const ImVec2 &position = {0, 0},
      ImVec2 size = ImVec2(0,0),
      const std::string &label = "",
      const HBDrawLocation itemFlags = HBDrawFlags_ForegroundDrawList
  );

  IMGUI_API void
  endSideBar();

  IMGUI_API bool
  sideBarBarButton(const ImGuiID id, const std::string& label,
                   const ImVec2 &position = {0, 0}, const ImVec2 &size = {0, 0});

  //---------------------------------------------------------------------------------
  // [SECTION] Sample/Debug Windows
  //---------------------------------------------------------------------------------
  HBUI_API void
  showDebugWindow(bool *p_open);

  HBUI_API void addDebugWidget(const std::string &name, IWidgetBase *widget);

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
  // [SECTION] Fonts
  //---------------------------------------------------------------------------------
  HBUI_API bool
  loadFont(const std::string &fontPath, float fontSize, const std::string &fontName);

  HBUI_API bool
  loadFont(const std::string &fontPath, float fontSize, const std::string &fontName, const std::string &fontGlyphRanges);

  //---------------------------------------------------------------------------------
  // [SECTION] IDS
  //---------------------------------------------------------------------------------
  HBUI_API ImGuiID
  getID(const char *str, const char *str_end);

}// namespace HBUI

//#include "fonts/FontLoader.h"

#include "UIItems/HBUIItemBase.h"
#include "UIItems/HBDefaultItems.h"
#include "Panels/HBPanels.h"

