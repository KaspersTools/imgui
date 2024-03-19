//
// Created by Kasper de Bruin on 10/02/2024.
//

// clang-format off
#include <HBUI/HBUI.h>
#include <UIItems/Interfaces/HBIWidget.h>
#include <UIItems/Windows/HBWindow.h>
#include <UIItems/Bars/HBTaskBar.h>
#include <UIItems/Buttons/HBButton.h>
#include <UIItems/Debugger/HBWidgetDebugger.h>


// clang-format on

namespace HBUI {
  //-------------------------------------------------------------------------
  // [SECTION] Private
  //-------------------------------------------------------------------------
  void setCurrentAppendingWidget(HBIWidget *widget) {
    auto *ctx = getCurrentContext();
    IM_ASSERT(ctx != nullptr && "Context is nullptr");
    ctx->startWidget(widget);
  }

  //-------------------------------------------------------------------------
  // [SECTION] Helpers
  //-------------------------------------------------------------------------
  HBUIBaseType_ typeToBaseType(HBUIType_ type) {
    IM_ASSERT(false && "Not implemented");
    return HBUIBaseType_Unknown;
  }

  //-------------------------------------------------------------------------
  // [SECTION] Debugging
  //-------------------------------------------------------------------------
  void showWidgetDebugWindow(bool *p_open) {
    auto *ctx = HBUI::getCurrentContext();
    IM_ASSERT(ctx != nullptr && "Context is nullptr");
    IM_ASSERT(ctx->widgetDebugger != nullptr && "Widget debugger is nullptr");

    ctx->widgetDebugger->draw(p_open);
  }
  void addOrUpdateWidgetDebugData(HBIWidget *widget) {
    auto *ctx = HBUI::getCurrentContext();
    IM_ASSERT(ctx != nullptr && "Context is nullptr");
    IM_ASSERT(ctx->widgetDebugger != nullptr && "Widget debugger is nullptr");

    ctx->widgetDebugger->addWidget(widget);
  }

  //-------------------------------------------------------------------------
  // [SECTION] Windows and Widgets
  //-------------------------------------------------------------------------
  void beginTaskBar(const std::string &label, const ImVec2 &size) {
    auto ctx = getCurrentContext();
    IM_ASSERT(ctx != nullptr && "Context is nullptr");

    auto *taskBar = new HBUI::Bars::HBTaskBar(label,
                                              nullptr,
                                              ImVec2(0, 0),
                                              size);
    ;
    setCurrentAppendingWidget(taskBar);
  }
  void endTaskBar() {
    getCurrentContext()->endCurrentWidget();
  }

  HBButtonState_ textButton(std::string label, const ImVec2 &size, const ImVec2 &localPos, HBMouseButtons_ interactButton) {
    auto *button  = new HBUI::Buttons::HBButton(std::move(label), HBButtonType_Text, interactButton,
                                                localPos, size);

    setCurrentAppendingWidget(button);
    getCurrentContext()->endCurrentWidget();

    return HBButtonState_None;
  }

  HBButtonState_ iconButton(Fonts::HBIcon* icon, const ImVec2 &size, const ImVec2 &localPos, HBMouseButtons_ interactButton) {
    auto *button  = new HBUI::Buttons::HBButton(icon, interactButton,
                                                localPos, size);
    setCurrentAppendingWidget(button);
    getCurrentContext()->endCurrentWidget();

    return HBButtonState_None;
  }

  bool beginFullScreenWindow(std::string              label,
                             HBUIWindowFlags          windowFlags,
                             HBStyleFlags             styleFlags,
                             HBUI::Windows::HBWindow *window) {
    HBStyleFlags finalStyleFlags = styleFlags | HBStyleFlags_StretchVertical | HBStyleFlags_StretchHorizontal;

    beginWindow(std::move(label),
                ImVec2(0, 0),
                windowFlags,
                finalStyleFlags,
                //fixme: make this customizable
                ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoFocusOnAppearing |
                    ImGuiWindowFlags_NoNav,
                HBDirection_TopToBottom,
                ImVec2(0, 0),
                ImVec2(0, 0));

    return true;
  }
  bool beginWindow(std::string        label,
                   ImGuiID            id,
                   HBUIWindowFlags    flags,
                   ImGuiWindowFlags   imguiWindowFlags,
                   const ImVec2      &size,
                   const ImVec2      &localPos,
                   HBDirection_       direction,
                   const ImVec2      &startCursorPos,
                   HBStyleFlags       styleFlags,
                   Windows::HBWindow *window) {

    auto *colorBuilder = new Builder::HBWidgetColorPropertiesBuilder();
    colorBuilder->fromImGuiStyle(HBUIType_Window);
    colorBuilder->setBackgroundColor(ImVec4(255, 0, 0, 255));
    auto *colorProperties = colorBuilder->build();
    window                = new HBUI::Windows::HBWindow(std::move(label),
                                         id == -1 ? ImGui::GetID(label.c_str()) : id,
                                                        flags,
                                                        imguiWindowFlags,
                                                        size,
                                                        localPos,
                                                        direction,
                                                        startCursorPos,
                                                        styleFlags,
                                                        colorProperties,
                                                        HBUIType_Window);

    setCurrentAppendingWidget(window);
    return true;
  }

  bool beginWindow(std::string              label,
                   const ImVec2            &position,
                   HBUIWindowFlags          windowFlags,
                   HBStyleFlags             styleFlags,
                   ImGuiWindowFlags         imguiWindowFlags,
                   HBDirection_             cursorDirection,
                   const ImVec2            &size,
                   const ImVec2            &cursorPos,
                   ImGuiID                  id,
                   HBUI::Windows::HBWindow *window) {
    IM_ASSERT(!label.empty() && "ID cannot be empty");

    auto *colorBuilder = new Builder::HBWidgetColorPropertiesBuilder();
    colorBuilder->fromImGuiStyle(HBUIType_Window);
    colorBuilder->setBackgroundColor(ImVec4(255, 0, 0, 255));
    auto *colorProperties = colorBuilder->build();
    window = new HBUI::Windows::HBWindow(std::move(label),
                                         id == -1 ? ImGui::GetID(label.c_str()) : id,
                                         windowFlags,
                                         imguiWindowFlags,
                                         size,
                                         position,
                                         cursorDirection,
                                         cursorPos,
                                         styleFlags,
                                         colorProperties,
                                         HBUIType_Window);

    setCurrentAppendingWidget(window);
    return true;
  }
}// namespace HBUI