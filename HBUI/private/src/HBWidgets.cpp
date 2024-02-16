//
// Created by Kasper de Bruin on 10/02/2024.
//
#include <HBUI/HBUI.h>


//-----------------------------------------------------------------------------
// [SECTION] Helper functions
//-----------------------------------------------------------------------------
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

/**
 *
 * ******************
 * HBUIItem
 * ******************
 *
 */
//-----------------------------------------------------------------------------
// [SECTION] HBUIItem
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// [SECTION] Rect
//-----------------------------------------------------------------------------
bool HBRect::draw(ImDrawList *drawList) {
  drawList->AddRectFilled(start, end, Color);
}
void HBRect::update(float deltaTime) {
}

//-----------------------------------------------------------------------------
// [SECTION] MenuItem
//-----------------------------------------------------------------------------
bool HBMenuItem::draw(ImDrawList *drawList) {
}
void HBMenuItem::update(float deltaTime) {
}


//-----------------------------------------------------------------------------
// [SECTION] MenuBar
//-----------------------------------------------------------------------------
bool HBMainMenuBar::draw(ImDrawList *drawList) {
  bool maximized = false;

  auto imguiStyle = ImGui::GetStyle();

  auto ctx      = HBUI::getCurrentContext();
  auto hbStyle  = HBUI::getCurrentContext()->style;
  auto drawData = HBUI::getCurrentContext()->drawData;
  auto viewport = (ImGuiViewportP *) (void *) ImGui::GetMainViewport();

  bool horizontal = (drawData->currentAppendingMenuBar == drawData->mainMenuBarHorizontal);
  bool vertical = (drawData->currentAppendingMenuBar == drawData->mainMenuBarVertical);

  ctx->drawData->savedScreenPos = ImGui::GetStyle().WindowMinSize;
  imguiStyle.WindowMinSize = {0, 0};

  ImGui::SetCurrentViewport(NULL, viewport);
  ImGuiWindowFlags window_flags =   ImGuiWindowFlags_NoResize
                                  | ImGuiWindowFlags_NoMove
                                  | ImGuiWindowFlags_NoDocking
                                  | ImGuiWindowFlags_NoTitleBar
                                  | ImGuiWindowFlags_NoCollapse
                                  | ImGuiWindowFlags_NoScrollbar
                                  | ImGuiWindowFlags_NoScrollWithMouse
                                  | ImGuiWindowFlags_NoBringToFrontOnFocus
                                  | ImGuiWindowFlags_NoNavFocus
                                  | ImGuiWindowFlags_NoBackground
                                  ;
  HBRect::start = HBUI::getViewportPos();

  if (horizontal) {
    float endY = HBRect::start.y + hbStyle.mainMenuHorizontalHeight;
    float endX = start.x         + HBUI::getWindowSize().x;

    HBRect::end = ImVec2(endX, endY);

    if(drawData->mainMenuBarVertical != nullptr){
      HBRect::start.x += hbStyle.mainMenuVerticalWidth;
    }

    //create a small overlap on the left side of the horizontal menu-bar
    HBRect::start.x -= 1;
    HBRect::end.x   += 1;
  }
  else if (vertical) {
    HBRect::end = ImVec2(viewport->Pos.x + hbStyle.mainMenuVerticalWidth,
                         viewport->Pos.y + viewport->Size.y);

    if(drawData->mainMenuBarHorizontal != nullptr){
      HBRect::start.y += hbStyle.mainMenuHorizontalHeight;
    }
  }
  ImGui::SetNextItemAllowOverlap();
  ImGui::SetNextWindowPos     (HBRect::start ,    ImGuiCond_Always);
  ImGui::SetNextWindowSize    (HBRect::Size(),    ImGuiCond_Always);
  ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);


  ImVec2 windowPadding       = ImVec2(0.0f, 0.0f);
  float windowRounding       = 0.0f;
  float WindowBorderSize     = 0.0f;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,      windowPadding    );
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,   WindowBorderSize );
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,     windowRounding   );

  bool p_Open = false;
  if(horizontal) {
    p_Open = ImGui::Begin("###HBUIMainMenuBar_HORIZONTAL", nullptr, window_flags);
  }else if(vertical){
    p_Open = ImGui::Begin("###HBUIMainMenuBar_VERTICAL",   nullptr, window_flags);
  }
  ImGui::PopStyleVar(3);

  if (!p_Open) {
    ImGui::End();
    return false;
  }
  if (!maximized) {
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
    renderWindowOuterBorders(ImGui::GetCurrentWindow());
    ImGui::PopStyleColor();// ImGuiCol_Border
  }
  if (drawList == nullptr) {
    drawList = ImGui::GetWindowDrawList();
  }
  HBRect::draw(drawList);

  if(horizontal) {
    HBUI::appendToCursor(ImVec2(0, hbStyle.mainMenuHorizontalHeight), false);
  }else if(vertical){
    HBUI::appendToCursor(ImVec2(hbStyle.mainMenuVerticalWidth, 0), false);
  }

  return p_Open;
}
void HBMainMenuBar::update(float deltaTime) {
}


namespace HBUI {
  /**
 *
 * ******************
 * DOCKSPACES
 * ******************
 *
 */
  HBUI_API bool
  beginFullScreenDockspace(HBDockspaceFlags_ flags) {
    const bool maximized              = isMaximized();
    const bool mainWindowNoTitleBar   = (HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR & getCurrentContext()->io.mainWindowFlags);
    const bool hasMenuBar             = (HB_DOCKSPACE_FLAG_MENUBAR & flags);
    return beginFullScreenDockspace(maximized, mainWindowNoTitleBar, hasMenuBar);
  }
  HBUI_API bool
  beginFullScreenDockspace(const bool isMaximized, const bool mainWindowNoTitleBar, const bool hasMenuBar) {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    ImGuiViewport *viewport       = ImGui::GetMainViewport();

    ImGuiStyle &style     = ImGui::GetStyle();
    float minWinSizeX     = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    auto drawData         = HBUI::getCurrentContext()->drawData;

    ImGui::SetNextWindowPos     (getCursorViewportPos());
    ImGui::SetNextWindowSize    (getContentRegionAvail());
    ImGui::SetNextWindowViewport(viewport->ID);

    window_flags |= ImGuiWindowFlags_NoTitleBar            | ImGuiWindowFlags_NoCollapse             | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove                | ImGuiWindowFlags_NoBringToFrontOnFocus  | ImGuiWindowFlags_NoNavFocus |
                    ImGuiWindowFlags_NoScrollbar           | ImGuiWindowFlags_NoScrollWithMouse;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;

    ImVec2 WindowPaddingNormal    = ImVec2(0.0f, 0.0f);
    ImVec2 WindowPaddingMaximized = ImVec2(6.0f, 6.0f);

    float windowRounding   = 0;
    float WindowBorderSize = 0.0f;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,    isMaximized ? WindowPaddingMaximized : WindowPaddingNormal);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, WindowBorderSize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,   windowRounding);

    //todo: menubar bg when custom?
    ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    if (!isMaximized) {
      ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
      renderWindowOuterBorders(ImGui::GetCurrentWindow());
      ImGui::PopStyleColor();
    }

    ImGui::DockSpace(ImGui::GetID("MyDockspace"), ImVec2(0.0f, 0.0f));
    ImGui::End();

    style.WindowMinSize.x = minWinSizeX;
  }
  /**
 *
 * ******************
 * MenuBar
 * ******************
 *
 */
  HBUI_API bool
  beginMainMenuBar(MainMenuBarFlags flags) {

    HBContext *ctx = HBUI::getCurrentContext();

    const bool horizontal = (HB_MAIN_MENU_BAR_FLAG_HORIZONTAL & flags) ||
                            (HB_MAIN_MENU_BAR_FLAG_NONE       & flags);

    const bool vertical   = (HB_MAIN_MENU_BAR_FLAG_VERTICAL   & flags);

    IM_ASSERT(!(vertical && horizontal) && "Cannot create a menu-bar with both horizontal and vertical flags! Create two separate menu-bars instead.");

    ImColor col = ctx->style.menuBarColor;
    if (col.Value == ImColor(-1, -1, -1, 255))
      col = ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg];

    if (horizontal) {
      ctx->drawData->mainMenuBarHorizontal    = std::make_shared<HBMainMenuBar>(flags, col);
      ctx->drawData->currentAppendingMenuBar  = ctx->drawData->mainMenuBarHorizontal;
    }
    if (vertical) {
      ctx->drawData->mainMenuBarVertical      = std::make_shared<HBMainMenuBar>(flags, col);
      ctx->drawData->currentAppendingMenuBar  = ctx->drawData->mainMenuBarVertical;
    }
    return true;

  }// create and append to a full screen menu-bar.

  IMGUI_API void
  endMainMenuBar() {
    HBContext *ctx = HBUI::getCurrentContext();
    ctx->drawData->currentAppendingMenuBar->draw(nullptr);
    ctx->drawData->currentAppendingMenuBar = nullptr;

    ImGui::GetStyle().WindowMinSize = ctx->drawData->savedScreenPos;

    ImGui::End();
  }// only call EndMainMenuBar() if BeginMainMenuBar() returns true!

  //menu items
  IMGUI_API bool
  beginMainMenuItem(const std::string &id, HBDrawType type, ImVec2 size) {
    if (type == Normal) { type = Square; }
    if (size == ImVec2(0, 0)) {}

    IM_ASSERT(id != "" && "Menu item id cannot be empty!");

    return true;
  }


  IMGUI_API void
  EndMainMenuItem() {
  }

}// namespace HBUI