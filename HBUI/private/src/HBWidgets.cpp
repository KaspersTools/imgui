//
// Created by Kasper de Bruin on 10/02/2024.
//
#include <HBUI/HBUI.h>


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
void HBRect::draw(ImDrawList *drawList) {
  drawList->AddRectFilled(start, end, Color);
}
void HBRect::update(float deltaTime) {
}
ImVec2 HBRect::StartPos() const {
  return start;
}
ImVec2 HBRect::EndPos() const {
  return end;
}


//-----------------------------------------------------------------------------
// [SECTION] MenuItem
//-----------------------------------------------------------------------------
void HBMenuItem::draw(ImDrawList *drawList) {
  itemToDraw->draw(drawList);
}
void HBMenuItem::update(float deltaTime) {
}


//-----------------------------------------------------------------------------
// [SECTION] MenuBar
//-----------------------------------------------------------------------------
void HBMainMenuBar::draw(ImDrawList *drawList) {
  //  ImDrawList *drawList = ImGui::GetWindowDrawList();
  ImGui::SetCursorPos(ImVec2(0, 0));

  //todo: calculate all items size
  float height = 50;
  float width = ImGui::GetMainViewport()->Size.x;

  HBRect::start = ImGui::GetCursorScreenPos();
  HBRect::end = ImVec2(HBRect::start.x + width,
                       HBRect::start.y + height);

  if (HBUIItem::Color.Value == ImColor(-1, -1, -1, -1))
    HBUIItem::Color = ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg];

  HBRect::draw(drawList);

  auto drawData = HBUI::getCurrentContext()->drawData;
  ImGui::SetCursorScreenPos({HBRect::start.x, HBRect::end.y});
}
void HBMainMenuBar::update(float deltaTime) {
}


namespace HBUI {

  /**
   * ******************
   * Updatables
   * ******************
   */
  //todo: add to updateables

  /**
 *
 * ******************
 * DOCKSPACES
 * ******************
 *
 */

  HBUI_API void
  beginFullScreenDockspace(DockspaceFlags flags) {
    const bool maximized = isMaximized();
    const bool mainWindowNoTitleBar = (HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR & getCurrentContext()->io.mainWindowFlags);
    const bool hasMenuBar = (HB_DOCKSPACE_FLAG_MENUBAR & flags);

    beginFullScreenDockspace(maximized, mainWindowNoTitleBar, hasMenuBar);
  }

  HBUI_API void
  beginFullScreenDockspace(const bool isMaximized,
                           const bool mainWindowNoTitlebar,
                           const bool hasMenuBar) {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImGuiStyle &style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    auto p = viewport->Pos;

    auto drawData = HBUI::getCurrentContext()->drawData;
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;

    ImVec2 WindowPaddingNormal = ImVec2(1.0f, 1.0f);
    ImVec2 WindowPaddingMaximized = ImVec2(6.0f, 6.0f);

    float windowRounding = 0;
    float WindowBorderSize = 0.0f;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? WindowPaddingMaximized : WindowPaddingNormal);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, WindowBorderSize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, windowRounding);

    //todo: menubar bg when custom?
    ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    if (!isMaximized) {
      ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
      // Draw window border if the window is not maximized
      renderWindowOuterBorders(ImGui::GetCurrentWindow());
      ImGui::PopStyleColor();// ImGuiCol_Border
    }
  }

  HBUI_API
  void endFullScreenDockspace() {
    ImGui::DockSpace(ImGui::GetID("MyDockspace"), ImVec2(0.0f, 0.0f));
    ImGui::End();
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

    ImColor col = ctx->style.menuBarColor;

    if (col.Value == ImColor(-1, -1, -1, 255))
      col = ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg];

    ctx->drawData->mainMenuBar = std::make_shared<HBMainMenuBar>(flags, col);

    return true;
  }// create and append to a full screen menu-bar.

  IMGUI_API void
  endMainMenuBar() {
    HBContext *ctx = HBUI::getCurrentContext();

    ctx->drawData->mainMenuBar->draw(ImGui::GetForegroundDrawList());

    ctx->drawData->firstItemStart = {
            ctx->drawData->mainMenuBar->start.x,
            ctx->drawData->mainMenuBar->end.y};
  }// only call EndMainMenuBar() if BeginMainMenuBar() returns true!

  void drawHorizontalMenuBar() {
  }

  //menu items
  IMGUI_API bool
  beginMainMenuItem(const std::string &name, HBDrawType type, ImVec2 size) {
    if (name == "") {
      "Menu item name cannot be empty!";
      return false;
    }
    if (type == Normal) { type = Square; }
    if (size == ImVec2(0, 0)) { size = HBUI::getCurrentContext()->style.menuItemSizeButton; }

    HBContext *ctx = HBUI::getCurrentContext();

    const bool horizontal = (HB_MAIN_MENU_BAR_FLAG_HORIZONTAL & ctx->drawData->mainMenuBar->flags) ||
                            (HB_MAIN_MENU_BAR_FLAG_NONE & ctx->drawData->mainMenuBar->flags);
    const bool vertical = (HB_MAIN_MENU_BAR_FLAG_VERTICAL & ctx->drawData->mainMenuBar->flags);

    //Default horizontal menu
    if (horizontal) {
      ImColor col = ImColor(255, 0, 0, 255);
      std::shared_ptr<HBMenuItem> menuItem = std::make_shared<HBMenuItem>(name, type, col);
      ctx->drawData->mainMenuBar->items.push_back(menuItem);
      return true;
    } else if (vertical) {
    }
    return false;
  }

  IMGUI_API void
  EndMainMenuItem() {
    HBContext *ctx = HBUI::getCurrentContext();
    ctx->drawData->mainMenuBar->items.back()->draw(ImGui::GetForegroundDrawList());
  }

}// namespace HBUI