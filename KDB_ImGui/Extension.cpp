#include "Extension.h"
#include <string>

namespace KDB_ImGui {

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
                     const bool drawDebug,
                     const ImColor debugColor) {

    Extension::getTempData()->TitleBarHeight = inHeight;

    float titlebarHorizontalOffset = isMaximized ? startMaximized.x : startWindowed.x;
    float titlebarVerticalOffset = isMaximized ? startMaximized.y : startWindowed.y;

    const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;
    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));

    const ImVec2 titlebarMin =
            {ImGui::GetCursorScreenPos().x + titlebarHorizontalOffset,
             ImGui::GetCursorScreenPos().y - titlebarVerticalOffset};

    const ImVec2 titlebarMax = {
            ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - titlebarVerticalOffset,
            ImGui::GetCursorScreenPos().y + Extension::getTempData()->TitleBarHeight - titlebarVerticalOffset};

    auto *bgDrawList = ImGui::GetBackgroundDrawList();
    auto *fgDrawList = ImGui::GetForegroundDrawList();

    bgDrawList->AddRectFilled(titlebarMin, titlebarMax, ImColor(0, 0, 0, 255));

    if (drawDebug) { fgDrawList->AddRect(titlebarMin, titlebarMax, ImColor(255, 0, 0, 255)); }

    //set cursor to the start of the titlebar
    ImGui::SetCursorPos(ImVec2(titlebarHorizontalOffset, titlebarVerticalOffset));

    ImVec2 titleBarStartScreen = {
            ImGui::GetCursorScreenPos().x,
            ImGui::GetCursorScreenPos().y};

    ImVec2 titleBarStartWindow = screenToWindowSpace(titleBarStartScreen);
    ImVec2 defaultSpacing = ImGui::GetStyle().ItemSpacing;

    ImVec2 currentTitleBarPosScreen = titleBarStartScreen;

    //draw logo
    if (hasLogo) {
      ImVec2 logoStartScreen = {
              titleBarStartScreen.x + defaultSpacing.x,
              titleBarStartScreen.y + defaultSpacing.y};
      ImVec2 logoStartWindow = screenToWindowSpace(logoStartScreen);

      ImVec2 logoEndScreen = {
              logoStartScreen.x + logoSize.x,
              logoStartScreen.y + logoSize.y};
      ImVec2 logoEndWindow = screenToWindowSpace(logoEndScreen);
      //Zoom in to image
      fgDrawList->AddImage((ImTextureID) logoDescriptorSet, logoStartScreen, logoEndScreen);
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
      Extension::getTempData()->CenteredTitleEndWindow = {titleStartScreen.x + titleSize.x, titleStartScreen.y + titleSize.y};

      fgDrawList->AddText(titleStartScreen, ImColor(255, 255, 255, 255), appTitle.c_str());
    }
    //draw menu bar
    {
      ImVec2 menuBarStartWindow;
      ImVec2 menuBarEndScreen;
      ImVec2 menuBarEndWindow;

      float width = ImGui::GetWindowWidth();
      if (drawTitleCentered) {
        width = (titleStartScreen.x - currentTitleBarPosScreen.x) - defaultSpacing.x;
      }

      ImVec2 menuBarStartScreen = {};
      //draw menu bar between centered text and logo
      {
        const float height = ImGui::GetFrameHeightWithSpacing();
        menuBarStartScreen = ImVec2(
                currentTitleBarPosScreen.x + defaultSpacing.x,
                titleBarStartScreen.y + defaultSpacing.y + menuBarExtraOffsetY);

        menuBarStartWindow = screenToWindowSpace(menuBarStartScreen);
        menuBarEndScreen = {menuBarStartScreen.x + width, menuBarStartScreen.y + height};
        menuBarEndWindow = screenToWindowSpace(menuBarEndScreen);

        Extension::getTempData()->MenuBarStartWindow = menuBarStartWindow;
        Extension::getTempData()->MenuBarSize = {ImGui::GetWindowWidth(), height};
        Extension::getTempData()->DrawDebug = drawDebug;
        Extension::getTempData()->DebugColor = debugColor;
      }
    }
  }

  static ImRect MenuBarRect(ImVec2 windowScreenPos, float menuBarWidth, float menuBarHeight) {
    float y1 = windowScreenPos.y;
    return ImRect(windowScreenPos.x,
                  y1,
                  windowScreenPos.x + menuBarWidth,
                  y1 + ImGui::GetFrameHeight());
  }

  void beginMenuBar(ImVec2 screenPos, float menuBarWidth, float menuBarHeight, bool drawDebug, ImColor debugColor) {

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

    if (drawDebug) {
      ImGui::GetForegroundDrawList()->AddRect(
              menuBarRect.Min,
              menuBarRect.Max,
              debugColor);
    }
  }

  void endMenuBar() {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
      return;
    ImGuiContext &g = *GImGui;

    // Nav: When a move request within one of our child menu failed, capture the request to navigate among our siblings.
    if (ImGui::NavMoveRequestButNoResultYet() && (g.NavMoveDir == ImGuiDir_Left || g.NavMoveDir == ImGuiDir_Right) && (g.NavWindow->Flags & ImGuiWindowFlags_ChildMenu)) {
      // Try to find out if the request is for one of our child menu
      ImGuiWindow *nav_earliest_child = g.NavWindow;
      while (nav_earliest_child->ParentWindow && (nav_earliest_child->ParentWindow->Flags & ImGuiWindowFlags_ChildMenu))
        nav_earliest_child = nav_earliest_child->ParentWindow;
      if (nav_earliest_child->ParentWindow == window && nav_earliest_child->DC.ParentLayoutType == ImGuiLayoutType_Horizontal && (g.NavMoveFlags & ImGuiNavMoveFlags_Forwarded) == 0) {
        // To do so we claim focus back, restore NavId and then process the movement request for yet another frame.
        // This involve a one-frame delay which isn't very problematic in this situation. We could remove it by scoring in advance for multiple window (probably not worth bothering)
        const ImGuiNavLayer layer = ImGuiNavLayer_Menu;
        IM_ASSERT(window->DC.NavLayersActiveMaskNext & (1 << layer));// Sanity check
        ImGui::FocusWindow(window);
        ImGui::SetNavID(window->NavLastIds[layer], layer, 0, window->NavRectRel[layer]);

        g.NavDisableHighlight = true;// Hide highlight for the current frame so we don't see the intermediary selection.
        g.NavDisableMouseHover = g.NavMousePosDirty = true;

        ImGui::NavMoveRequestForward(g.NavMoveDir, g.NavMoveClipDir, g.NavMoveFlags, g.NavMoveScrollFlags);// Repeat
      }
    }

    IM_ASSERT(window->DC.MenuBarAppending);
    ImGui::PopClipRect();
    ImGui::PopID();
    window->DC.MenuBarOffset.x = window->DC.CursorPos.x - window->Pos.x;// Save horizontal position so next append can reuse it. This is kinda equivalent to a per-layer CursorPos.
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
    beginMenuBar(startScreenPos, Extension::getTempData()->MenuBarSize.x, Extension::getTempData()->MenuBarSize.y,
                 Extension::getTempData()->DrawDebug, Extension::getTempData()->DebugColor);
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
      if (border_n == ImGuiDir_Left) { return ImRect(rect.Min.x - thickness, rect.Min.y + perp_padding, rect.Min.x + thickness, rect.Max.y - perp_padding); }
      if (border_n == ImGuiDir_Right) { return ImRect(rect.Max.x - thickness, rect.Min.y + perp_padding, rect.Max.x + thickness, rect.Max.y - perp_padding); }
      if (border_n == ImGuiDir_Up) { return ImRect(rect.Min.x + perp_padding, rect.Min.y - thickness, rect.Max.x - perp_padding, rect.Min.y + thickness); }
      if (border_n == ImGuiDir_Down) { return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness, rect.Max.x - perp_padding, rect.Max.y + thickness); }
      IM_ASSERT(0);
      return ImRect();
    };
  }

  ImVec2
  screenToWindowSpace(const ImVec2 &screenPos) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImVec2 windowPos = window->Pos;
    ImVec2 windowSize = window->Size;
    ImVec2 scroll = window->Scroll;
    ImVec2 windowSpacePos = {screenPos.x - windowPos.x + scroll.x, screenPos.y - windowPos.y + scroll.y};
    return windowSpacePos;
  }

  ImVec2 windowToScreenSpace(const ImVec2 &screenPos) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImVec2 windowPos = window->Pos;
    ImVec2 windowSize = window->Size;

    ImVec2 windowMin = windowPos;

    ImVec2 windowSpacePos = {screenPos.x + windowMin.x, screenPos.y + windowMin.y};

    return windowSpacePos;
  }

  void endTitleBar() {
    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
    ImGui::SetCursorPosY(Extension::getTempData()->TitleBarHeight);

    Extension::getTempData()->reset();
  }


}// namespace KDB::IMGUI
