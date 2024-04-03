//
// Created by Kasper de Bruin on 06/02/2024.
//

#pragma once
#include "../../include/HBUI/HBUIEnums.cpp"

#ifndef HBUI_API
#define HBUI_API
#endif

#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>

#include "Fonts/IconsNerdFont.h"

namespace HBUI {
  HBUI_API std::string wchar32ToUtf8(const ImWchar &wchar);

  HBUI_API HBContext *getCurrentContext();
  HBUI_API HBContext *initialize(const std::string &title,
                                 int width, int height,
                                 int HBBackendWindowFlags = 0);

  HBUI_API void afterBackendInitialized();

  //---------------------------------------------------------------------------------
  // [SECTION] Context
  //---------------------------------------------------------------------------------

  //---------------------------------------------------------------------------------
  // [SECTION] IO
  //---------------------------------------------------------------------------------
  HBUI_API HBIO  &getIO();
  HBUI_API ImVec2 getMousePos();
  HBUI_API bool   isMouseButtonDown(HBMouseButtons_ button);

  //---------------------------------------------------------------------------------
  // [SECTION] Main Window
  //---------------------------------------------------------------------------------

  //(imgui fullscreen) Window
  HBUI_API const ImVec2 &getMainWindowSize();
  HBUI_API const ImVec2 &getMainWindowPos();
  HBUI_API const ImVec2 &getMainWindowDpiScaleFactor();
  HBUI_API float         getMainWindowDpiScale();
  HBUI_API std::string   getMonitorName();

  //(backend) Window
  //(backend) Window
  HBUI_API const ImVec2 &getNativeWindowPos();
  HBUI_API const ImVec2 &getNativeWindowSize();
  HBUI_API ImColor      &getNativeWindowClearColor();
  HBUI_API void          setNativeWindowClearColor(const ImColor &color);

  //ImGui Viewports
  HBUI_API ImGuiViewport *getCurrentViewport();
  HBUI_API ImGuiViewport *getMainViewport();

  HBUI_API ImVec2        getViewportPos(ImGuiViewport *viewport);
  HBUI_API ImVec2        getMainViewportPos();
  HBUI_API const ImVec2 &getMainViewportSize();
  HBUI_API ImVec2        getCurrentViewportPos();

  //---------------------------------------------------------------------------------
  // [SECTION] Cursor
  //---------------------------------------------------------------------------------
  HBUI_API ImVec2 getMainWindowCursorPos();
  HBUI_API ImVec2 getMainWindowCursorScreenPos();
  //todo:	HBUI_API ImVec2 getMainWindowCursorScreenPos(ImGuiViewport *viewport);

  HBUI_API ImVec2 getContentRegionMaxMainWindow();
  HBUI_API ImVec2 getContentRegionAvailMainWindow();
  //todo: HBUI_API ImVec2 getContentRegionAvail(ImGuiViewport *viewport);

  HBUI_API HBLayoutType_ getCurrentLayout();
  HBUI_API void          setLayout(HBLayoutType_ layoutType);

  //---------------------------------------------------------------------------------
  // [SECTION] Helpers
  //---------------------------------------------------------------------------------
  HBUI_API HBUIBaseType_ typeToBaseType(HBUIType_ type);
  HBUI_API bool          aabb(const ImVec2 &min1, const ImVec2 &max1, const ImVec2 &min2, const ImVec2 &max2);
  HBUI_API bool          containsPoint(const ImVec2 &min, const ImVec2 &max, const ImVec2 &point);

  //---------------------------------------------------------------------------------
  // [SECTION] Helpers/Positioning
  //---------------------------------------------------------------------------------
  HBUI_API ImVec2 getCenteredPosition(const ImVec2 &size, const ImVec2 &parentSize);

  //---------------------------------------------------------------------------------
  // [SECTION] Debugger
  //---------------------------------------------------------------------------------
  HBUI_API void showWidgetDebugWindow(bool *p_open);
  HBUI_API void addOrUpdateWidgetDebugData(HBIWidget *widget);

  //---------------------------------------------------------------------------------
  // [SECTION] Widgets
  //---------------------------------------------------------------------------------
  HBUI_API bool beginFullScreenWindow(
      std::string        label,
      HBUIWindowFlags    windowFlags = HBUIWindowFlags_None,
      HBStyleFlags       styleFlags  = HBStyleFlags_None,
      Windows::HBWindow *window      = nullptr);
  HBUI_API bool beginWindow(std::string        label,
                            ImGuiID            id,
                            HBUIWindowFlags    flags            = HBUIWindowFlags_None,
                            ImGuiWindowFlags   imguiWindowFlags = ImGuiWindowFlags_None,
                            const ImVec2      &size             = {0, 0},
                            const ImVec2      &localPos         = {0, 0},
                            HBDirection_       direction        = HBDirection_TopToBottom,
                            const ImVec2      &startCursorPos   = {0, 0},
                            HBStyleFlags       styleFlags       = HBStyleFlags_None,
                            Windows::HBWindow *window           = nullptr);

  HBUI_API bool beginWindow(std::string        label,
                            const ImVec2      &position         = ImVec2(0, 0),
                            HBUIWindowFlags    windowFlags      = HBUIWindowFlags_None,
                            HBStyleFlags       styleFlags       = HBStyleFlags_None,
                            ImGuiWindowFlags   imguiWindowFlags = ImGuiWindowFlags_None,
                            HBDirection_       cursorDirection  = HBDirection_TopToBottom,
                            const ImVec2      &size             = ImVec2(0, 0),
                            const ImVec2      &cursorPos        = ImVec2(0, 0),
                            ImGuiID            id               = -1,
                            Windows::HBWindow *window           = nullptr);

  HBUI_API void endWindow();

  HBUI_API bool beginFullScreenDockSpaceWindow(const std::string &id,
                                               const ImVec2      &position  = {0, 0},
                                               const ImVec2      &cursorPos = {0, 0});
  HBUI_API void endFullScreenDockSpaceWindow();

  //---------------------------------------------------------------------------------
  // [SECTION] Windows/Panels
  //---------------------------------------------------------------------------------
  HBUI_API bool beginFlexPanel(const std::string &id,
                               const ImVec2      &position  = {0, 0},
                               const ImVec2      &size      = {0, 0},
                               const ImVec2      &cursorPos = {0, 0});
  HBUI_API void endFlexPanel();

  //---------------------------------------------------------------------------------
  // [SECTION] Side And Menu Bars
  //---------------------------------------------------------------------------------
  HBUI_API void beginTaskBar(const std::string &label, const ImVec2 &size = ImVec2(0, 0));
  HBUI_API void endTaskBar();

  //---------------------------------------------------------------------------------
  // [SECTION] Buttons
  //---------------------------------------------------------------------------------
  HBUI_API HBButtonState_ textButton(std::string     label,
                                     const ImVec2   &size           = {0, 0},
                                     const ImVec2   &localPos       = {0, 0},
                                     HBMouseButtons_ interactButton = HBMouseButtons_Left);

  HBUI_API HBButtonState_ iconButton(Fonts::HBIcon  *icon,
                                     const ImVec2   &size           = {0, 0},//this uses the HBStyle.IconSize as default
                                     const ImVec2   &localPos       = {0, 0},
                                     HBMouseButtons_ interactButton = HBMouseButtons_Left);

  HBUI_API void           setButtonState(ImGuiID id, HBButtonState_ state);
  HBUI_API HBButtonState_ getButtonState(ImGuiID id);
  //---------------------------------------------------------------------------------
  // [SECTION] Sample/Debug Windows
  //---------------------------------------------------------------------------------
  HBUI_API void showDebugWindow(bool *p_open);

  //---------------------------------------------------------------------------------
  // [SECTION] Updating
  //---------------------------------------------------------------------------------
  HBUI_API void update(float deltatime);

  //---------------------------------------------------------------------------------
  // [SECTION] Rendering
  //---------------------------------------------------------------------------------
  HBUI_API void startFrame();
  HBUI_API void endFrame();
  HBUI_API bool wantToClose();
  HBUI_API void shutdown();

  //---------------------------------------------------------------------------------
  // [SECTION] Input
  //---------------------------------------------------------------------------------


  //---------------------------------------------------------------------------------
  // [SECTION] Styles
  //---------------------------------------------------------------------------------
  HBUI_API HBStyle &getStyle();

  //---------------------------------------------------------------------------------
  // [SECTION] Flags
  //---------------------------------------------------------------------------------
  HBUI_API bool isFlagSet(int *flags, int flag);

  //---------------------------------------------------------------------------------
  // [SECTION] Fonts
  //---------------------------------------------------------------------------------
  HBUI_API float  getWindowSizeDpiScaleFactor();
  HBUI_API float  getFontSizeIncreaseFactor();
  HBUI_API ImVec2 getWindowScaleFactor();

  namespace Fonts {
    struct HBFont;
    struct HBIcon;
  }// namespace Fonts

  HBUI_API Fonts::HBIcon *addDefaultIcon(const std::string &name, ImWchar glyph);
  HBUI_API Fonts::HBFont *getFont();
  HBUI_API Fonts::HBFont *getFont(float fontSize, HBLoadFontFlags flags);
  HBUI_API Fonts::HBFont *getDefaultFont();
  HBUI_API Fonts::HBFont *getBigFont();

  HBUI_API Fonts::HBIcon *getIcon(const std::string &iconName);

  HBUI_API void activateFontSize(float fontSize);
  HBUI_API void activateFont(Fonts::HBFont *font);

  //---------------------------------------------------------------------------------
  // [SECTION] Windows
  //---------------------------------------------------------------------------------
  HBUI_API void showLogWindow(bool *p_open);

  //---------------------------------------------------------------------------------
  // [SECTION] Logging
  //---------------------------------------------------------------------------------
  HBUI_API void log(const std::string &message);
  HBUI_API void warn(const std::string &message);
  HBUI_API void debug(const std::string &message);
  HBUI_API void error(const std::string &message);


}// namespace HBUI