
#include <KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h>

#include "Extension.h"

#include <optional>
#include <string>

namespace HBUI {
  /*
   * @brief drawDebugRectWithLabel draws a debug rect with a label.
   * @param label The label of the debug rect.
   * @param isActive If the debug rect should be drawn.
   * @param start The start position of the debug rect.
   * @param end The end position of the debug rect.
   * @param rectColor The color of the debug rect.
   * @param textColor The color of the debug text.
   * @param textBGColor The color of the background of the debug text.
   * @param backGroundBehindText Should the background be drawn behind the debug text?
   * @param textDir The direction of the debug text.
   * @param drawList The draw list to draw the debug rect on.
   * @return void
   * @note This function is used to draw debug rects with a label.
   */
  void drawDebugRectWithLabel(
          const std::string &label,
          const bool isActive,

          const ImVec2 &start, const ImVec2 &end,

          const bool drawFilled,
          const ImColor &rectColor,
          const ImColor &rectFillColor,

          const ImColor &textColor,
          const ImColor &textBGColor,
          const bool backGroundBehindText,

          const ImGuiDir_ textDir,
          ImDrawList *drawList = ImGui::GetForegroundDrawList()) {
    if (!isActive) return;
    if (drawFilled) {
      drawList->AddRectFilled(start, end, rectFillColor);
    }

    drawList->AddRect(start, end, rectColor);

    ImVec2 posStart = {start.x + 5, start.y + 5};
    ImVec2 posEnd = {end.x - 5, end.y - 5};

    ImVec2 textSize = ImGui::CalcTextSize(label.c_str());

    switch (textDir) {
      case ImGuiDir_Left:
        posStart = {start.x - textSize.x, start.y};
        posEnd = {start.x, start.y + textSize.y};
        break;
      case ImGuiDir_Right:
        posStart = {end.x, start.y};
        posEnd = {end.x + textSize.x, start.y + textSize.y};
        break;
      case ImGuiDir_Up:
        posStart = {start.x, start.y - textSize.y};
        posEnd = {start.x + textSize.x, start.y};
        break;
      case ImGuiDir_Down:
        posStart = {start.x, end.y};
        posEnd = {start.x + textSize.x, end.y + textSize.y};
        break;
      case ImGuiDir_None:
        posStart = {start.x, start.y};
        posEnd = {start.x + textSize.x, start.y + textSize.y};
        break;
    }
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 windowLocation = ImGui::GetWindowPos();

    if (posEnd.x > windowSize.x + windowLocation.x) {
      posStart.x = (windowSize.x - textSize.x) + windowLocation.x;
      posEnd.x = windowSize.x + windowLocation.x;
    }
    if (posEnd.y > windowSize.y + windowLocation.y) {
      posStart.y = (windowSize.y - textSize.y) + windowLocation.y;
      posEnd.y = windowSize.y + windowLocation.y;
    }
    if (posStart.x < windowLocation.x) {
      posStart.x = windowLocation.x;
      posEnd.x = windowLocation.x + textSize.x;
    }
    if (posStart.y < windowLocation.y) {
      posStart.y = windowLocation.y;
      posEnd.y = windowLocation.y + textSize.y;
    }
    //check if pos is out of screen
    if (backGroundBehindText) {
      drawList->AddRectFilled(posStart, posEnd, textBGColor);
    }

    drawList->AddText(posStart, textColor, label.c_str());
  }

  void drawDebugRectWithLabel(ApplicationTitleBarDebugInfo &debugInfo,
                              ImDrawList *drawList = ImGui::GetForegroundDrawList()) {
    drawDebugRectWithLabel(debugInfo.label, debugInfo.isActive, debugInfo.start, debugInfo.end, debugInfo.drawFilled,
                           debugInfo.rectColor, debugInfo.rectFilledColor,
                           debugInfo.textColor, debugInfo.textBGColor,
                           debugInfo.backGroundBehindText, debugInfo.textDir, drawList);
  }


  void beginTitleBar(const bool drawTitleCentered,
                     const std::string &appTitle,
                     const bool hasLogo,
                     const VkDescriptorSet logoDescriptorSet,
                     const ImVec2 &logoSize,
                     const ImVec2 &startMaximized,
                     const ImVec2 &startWindowed,
                     const float inHeight,
                     const float menuBarExtraOffsetY,
                     const bool isMaximized,
                     std::shared_ptr<ApplicationTitleBarDebugInfo> debugInfo) {

    ImGuiContext &g = *GImGui;
    ImGuiWindow *window = g.CurrentWindow;
    ImDrawList *drawList = window->DrawList;

    Extension::getTempData()->TitleBarHeight = inHeight;
    //draw area

    //Background of full titlebar
    {
      //@brief drawDebugRectWithLabel draws a debug rect with a label.
      //* @param label The label of the debug rect.
      //* @param isActive If the debug rect should be drawn.
      //* @param start The start position of the debug rect.
      //* @param end The end position of the debug rect.
      //* @param rectColor The color of the debug rect.
      //* @param textColor The color of the debug text.
      //* @param textBGColor The color of the background of the debug text.
      //* @param backGroundBehindText Should the background be drawn behind the debug text?
      //* @param textDir The direction of the debug text.
      //* @param drawList The draw list to draw the debug rect on.
      ImVec2 leftTopTitleBarScreen = {window->Pos.x, window->Pos.y};
      ImVec2 rightBottomTitleBarScreen = {window->Pos.x + window->Size.x, window->Pos.y + inHeight};

      ImColor menuBarBGColor = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg);
      ImGui::GetBackgroundDrawList()->AddRectFilled(leftTopTitleBarScreen, rightBottomTitleBarScreen, ImColor(0, 0, 0, 0));
      debugInfo->label = "TitleBar";
      debugInfo->start = leftTopTitleBarScreen;
      debugInfo->end = rightBottomTitleBarScreen;
      drawDebugRectWithLabel(*debugInfo);


      drawList->AddRectFilled(leftTopTitleBarScreen, rightBottomTitleBarScreen,
                              menuBarBGColor);
    }


    float titlebarHorizontalOffset = isMaximized ? startMaximized.x : startWindowed.x;
    float titlebarVerticalOffset = isMaximized ? startMaximized.y : startWindowed.y;

    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));

    //set cursor to the start of the titlebar
    ImGui::SetCursorPos(ImVec2(titlebarHorizontalOffset, titlebarVerticalOffset));
    ImVec2 titleBarStartScreen = {
            ImGui::GetCursorScreenPos().x,
            ImGui::GetCursorScreenPos().y};
    ImVec2 defaultSpacing = ImGui::GetStyle().ItemSpacing;
    ImVec2 currentTitleBarPosScreen = titleBarStartScreen;

    //draw logo
    if (hasLogo) {
      ImVec2 logoStartScreen = {
              titleBarStartScreen.x + defaultSpacing.x,
              titleBarStartScreen.y + defaultSpacing.y};
      ImVec2 logoEndScreen = {
              logoStartScreen.x + logoSize.x,
              logoStartScreen.y + logoSize.y};

      drawList->AddImage((ImTextureID) logoDescriptorSet, logoStartScreen, logoEndScreen);
      {
        debugInfo->label = "Logo";
        debugInfo->start = logoStartScreen;
        debugInfo->end = logoEndScreen;
        drawDebugRectWithLabel(*debugInfo);
      }
      currentTitleBarPosScreen = logoEndScreen;
    }

    //draw centered text
    ImVec2 titleStartScreen = {};

    if (drawTitleCentered && !appTitle.empty()) {
      const ImVec2 titleSize = ImGui::CalcTextSize(appTitle.c_str());
      const float windowCenter = ImGui::GetWindowWidth() / 2.0f;
      const float windowPos = ImGui::GetWindowPos().x;

      const float titleCenter = windowCenter - (titleSize.x / 2.0f);
      const float titleStart = windowPos + titleCenter;

      titleStartScreen = {
              titleStart,
              titleBarStartScreen.y + defaultSpacing.y};

      Extension::getTempData()->CenteredTitleStartWindow = titleStartScreen;
      Extension::getTempData()->CenteredTitleEndWindow = {titleStartScreen.x + titleSize.x,
                                                          titleStartScreen.y + titleSize.y};
      const ImColor textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
      drawList->AddText(titleStartScreen, textColor, appTitle.c_str());
      {
        debugInfo->label = "Title";
        debugInfo->start = titleStartScreen;
        debugInfo->end = {titleStartScreen.x + titleSize.x, titleStartScreen.y + titleSize.y};
        drawDebugRectWithLabel(*debugInfo);
      }
    }
    //draw menu bar
    {
      ImVec2 menuBarStartWindow;
      ImVec2 menuBarStartScreen = {};
      //draw menu bar between centered text and logo
      {
        const float height = ImGui::GetFrameHeightWithSpacing();
        menuBarStartScreen = ImVec2(
                currentTitleBarPosScreen.x + defaultSpacing.x,
                titleBarStartScreen.y + defaultSpacing.y + menuBarExtraOffsetY);

        menuBarStartWindow = screenToWindowSpace(menuBarStartScreen);
        Extension::getTempData()->MenuBarStartWindow = menuBarStartWindow;
        Extension::getTempData()->MenuBarSize = {
                ImGui::GetWindowWidth() - menuBarStartWindow.x,
                height};
      }
    }
  }

  static ImRect MenuBarRect(ImVec2 windowScreenPos, float menuBarWidth,
                            float menuBarHeight) {
    float y1 = windowScreenPos.y;
    return ImRect(windowScreenPos.x,
                  y1,
                  windowScreenPos.x + menuBarWidth,
                  y1 + ImGui::GetFrameHeight());
  }

  void beginMenuBar(ImVec2 screenPos, float menuBarWidth, float menuBarHeight) {

    ImGuiWindow *window = ImGui::GetCurrentWindow();

    IM_ASSERT(!window->DC.MenuBarAppending);
    ImGui::BeginGroup();// Backup position on layer 0 // FIXME: Misleading to use a group for that backup/restore
    ImGui::PushID("##menubar");

    // We don't clip with current window clipping rectangle as it is already set to the area below. However we clip with window full rect.
    // We remove 1 worth of rounding to Max.x to that text in long menus and small windows don't tend to display over the lower-right rounded area, which looks particularly glitchy.
    ImRect menuBarRect = MenuBarRect(screenPos, menuBarWidth, menuBarHeight);

    ImRect clip_rect(
            IM_ROUND(menuBarRect.Min.x + window->WindowBorderSize),
            IM_ROUND(menuBarRect.Min.y + window->WindowBorderSize),
            IM_ROUND(ImMax(menuBarRect.Min.x, menuBarRect.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize))),
            IM_ROUND(menuBarRect.Max.y));

    clip_rect.ClipWith(window->OuterRectClipped);
    ImGui::PushClipRect(clip_rect.Min, clip_rect.Max, false);

    // We overwrite CursorMaxPos because BeginGroup sets it to CursorPos (essentially the .EmitItem hack in EndMenuBar() would need something analogous here, maybe a BeginGroupEx() with flags).
    window->DC.CursorPos = window->DC.CursorMaxPos =
            ImVec2(menuBarRect.Min.x + window->DC.MenuBarOffset.x, menuBarRect.Min.y + window->DC.MenuBarOffset.y);

    window->DC.LayoutType = ImGuiLayoutType_Horizontal;
    window->DC.NavLayerCurrent = ImGuiNavLayer_Menu;
    window->DC.MenuBarAppending = true;
    ImGui::AlignTextToFramePadding();
  }

  void endMenuBar() {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
      return;
    ImGuiContext &g = *GImGui;

    if (ImGui::NavMoveRequestButNoResultYet() &&
        (g.NavMoveDir == ImGuiDir_Left || g.NavMoveDir == ImGuiDir_Right) &&
        (g.NavWindow->Flags & ImGuiWindowFlags_ChildMenu)) {
      ImGuiWindow *nav_earliest_child = g.NavWindow;
      while (nav_earliest_child->ParentWindow &&
             (nav_earliest_child->ParentWindow->Flags & ImGuiWindowFlags_ChildMenu))
        nav_earliest_child = nav_earliest_child->ParentWindow;
      if (nav_earliest_child->ParentWindow == window &&
          nav_earliest_child->DC.ParentLayoutType == ImGuiLayoutType_Horizontal &&
          (g.NavMoveFlags & ImGuiNavMoveFlags_Forwarded) == 0) {
        const ImGuiNavLayer layer = ImGuiNavLayer_Menu;
        IM_ASSERT(window->DC.NavLayersActiveMaskNext & (1 << layer));// Sanity check
        ImGui::FocusWindow(window);
        ImGui::SetNavID(window->NavLastIds[layer], layer, 0, window->NavRectRel[layer]);

        g.NavDisableHighlight = true;// Hide highlight for the current frame so we don't see the intermediary selection.
        g.NavDisableMouseHover = g.NavMousePosDirty = true;

        ImGui::NavMoveRequestForward(g.NavMoveDir, g.NavMoveClipDir, g.NavMoveFlags,
                                     g.NavMoveScrollFlags);// Repeat
      }
    }

    IM_ASSERT(window->DC.MenuBarAppending);
    ImGui::PopClipRect();
    ImGui::PopID();
    window->DC.MenuBarOffset.x = window->DC.CursorPos.x -
                                 window->Pos.x;// Save horizontal position so next append can reuse it. This is kinda equivalent to a per-layer CursorPos.
    g.GroupStack.back().EmitItem = false;
    ImGui::EndGroup();// Restore position on layer 0
    window->DC.LayoutType = ImGuiLayoutType_Vertical;
    window->DC.NavLayerCurrent = ImGuiNavLayer_Main;
    window->DC.MenuBarAppending = false;
  }

  void beginMainMenuBar() {
    Extension::getTempData()->CurrentResetWindowPos = ImGui::GetCursorPos();

    const ImVec2 startWindow = Extension::getTempData()->MenuBarStartWindow;
    ImGui::SetCursorPos(startWindow);

    const ImVec2 startScreenPos = windowToScreenSpace(startWindow);
    ImGui::BeginGroup();
    beginMenuBar(startScreenPos,
                 Extension::getTempData()->MenuBarSize.x,
                 Extension::getTempData()->MenuBarSize.y);
  }

  void endMainMenuBar() {
    endMenuBar();
    ImGui::EndGroup();

    ImGui::SetCursorPos(Extension::getTempData()->CurrentResetWindowPos);
  }


  void renderWindowOuterBorders(ImGuiWindow *window) {
    struct ImGuiResizeBorderDef {
      ImVec2 InnerDir;
      ImVec2 SegmentN1, SegmentN2;
      float OuterAngle;
    };

    static const ImGuiResizeBorderDef resize_border_def[4] =
            {
                    {ImVec2(+1, 0), ImVec2(0, 1), ImVec2(0, 0), IM_PI * 1.00f},// Left
                    {ImVec2(-1, 0), ImVec2(1, 0), ImVec2(1, 1), IM_PI * 0.00f},// Right
                    {ImVec2(0, +1), ImVec2(0, 0), ImVec2(1, 0), IM_PI * 1.50f},// Up
                    {ImVec2(0, -1), ImVec2(1, 1), ImVec2(0, 1), IM_PI * 0.50f} // Down
            };

    auto GetResizeBorderRect = [](ImGuiWindow *window, int border_n, float perp_padding, float thickness) {
      ImRect rect = window->Rect();
      if (thickness == 0.0f) {
        rect.Max.x -= 1;
        rect.Max.y -= 1;
      }
      if (border_n == ImGuiDir_Left) {
        return ImRect(rect.Min.x - thickness, rect.Min.y + perp_padding, rect.Min.x + thickness,
                      rect.Max.y - perp_padding);
      }
      if (border_n == ImGuiDir_Right) {
        return ImRect(rect.Max.x - thickness, rect.Min.y + perp_padding, rect.Max.x + thickness,
                      rect.Max.y - perp_padding);
      }
      if (border_n == ImGuiDir_Up) {
        return ImRect(rect.Min.x + perp_padding, rect.Min.y - thickness, rect.Max.x - perp_padding,
                      rect.Min.y + thickness);
      }
      if (border_n == ImGuiDir_Down) {
        return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness, rect.Max.x - perp_padding,
                      rect.Max.y + thickness);
      }
      IM_ASSERT(0);
      return ImRect();
    };
  }

  ImVec2
  calculateCenteredWindowPos() {
    return ImGui::GetContentRegionAvail().HalfSize();
  }


  void textCentered(const std::string &text, const ImVec4 &color){
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str()).HalfSize();
    ImVec2 windowCenter = calculateCenteredWindowPos();
    ImVec2 textStart = windowCenter - textSize;

    ImGui::SetCursorPos(textStart);
    ImGui::TextColored(color, text.c_str());
  }
  void
  textCenteredMultiline(const std::string &text, const ImVec4 &color){
    float win_width = ImGui::GetWindowSize().x;
    float text_width = ImGui::CalcTextSize(text.c_str()).x;

    // calculate the indentation that centers the text on one line, relative
    // to window left, regardless of the `ImGuiStyleVar_WindowPadding` value
    float text_indentation = (win_width - text_width) * 0.5f;

    // if text is too long to be drawn on one line, `text_indentation` can
    // become too small or even negative, so we check a minimum indentation
    float min_indentation = 20.0f;
    if (text_indentation <= min_indentation) {
      text_indentation = min_indentation;
    }

    ImGui::SameLine(text_indentation);
    ImGui::PushTextWrapPos(win_width - text_indentation);
    ImGui::TextWrapped(text.c_str());
    ImGui::PopTextWrapPos();
  }

  ImVec2
  screenToWindowSpace(const ImVec2 &screenPos) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImVec2 windowPos = window->Pos;
    ImVec2 scroll = window->Scroll;

    ImVec2 windowSpacePos = {screenPos.x - windowPos.x + scroll.x, screenPos.y - windowPos.y + scroll.y};
    return windowSpacePos;
  }

  ImVec2
  windowToScreenSpace(const ImVec2 &screenPos) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImVec2 windowPos = window->Pos;
    ImVec2 windowMin = windowPos;
    ImVec2 windowSpacePos = {screenPos.x + windowMin.x, screenPos.y + windowMin.y};

    return windowSpacePos;
  }

  void endTitleBar() {
    //reset cursor pos
    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
    ImGui::SetCursorPosY(Extension::getTempData()->TitleBarHeight);
    //Todo: maybe in end frame?
    Extension::getTempData()->reset();
  }


}// namespace HBUI
