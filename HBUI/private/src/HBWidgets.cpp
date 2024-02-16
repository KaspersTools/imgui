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
    float  OuterAngle;
  };

  static const ImGuiResizeBorderDef resize_border_def[4] =
                                        {
                                            {ImVec2(+1, 0), ImVec2(0, 1), ImVec2(0, 0), IM_PI * 1.00f},// Left
                                            {ImVec2(-1, 0), ImVec2(1, 0), ImVec2(1, 1), IM_PI *
                                                                                        0.00f},// Right
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

//-----------------------------------------------------------------------------
// [SECTION] Rect
//-----------------------------------------------------------------------------
bool HBRect::draw(ImDrawList *drawList, ImColor color, bool drawFilled) {
  drawList->AddRectFilled(start, end, color);

  //todo:remove this is for debugging
  ImGui::GetForegroundDrawList()->AddRect(start,
                                          end,
                                          ImColor(rand() % 255, rand() % 255, rand() % 255, 255));
}


void HBRect::update(float deltaTime) {
}

//-----------------------------------------------------------------------------
// [SECTION] MenuBar
//-----------------------------------------------------------------------------
bool HBMainMenuBar::draw(ImDrawList *drawList, ImColor color = ImColor(-1, -1, -1, -1), bool drawFilled = false) {
  if (color.Value == ImColor(-1, -1, -1, -1)) {
    color = getColor();//
  }
  bool maximized = false;

  auto                imguiStyle = ImGui::GetStyle();
  auto                ctx        = HBUI::getCurrentContext();
  auto                drawData   = HBUI::getCurrentContext()->drawData;
  auto                viewport   = (ImGuiViewportP *) (void *) ImGui::GetMainViewport();
  const HBMainMenuBar &bar       = *drawData->currentAppendingMenuBar;

  ImGui::SetCurrentViewport(NULL, viewport);
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking |
                                  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                                  ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                  ImGuiWindowFlags_NoBackground;

  //      main menu bars are drawn like this
  //-----------------------------------------------
  //--------first call horizontal------------------
  //-----------------------------------------------
  //|||||||||||||xoxoxoxoxoxoooxooxoxoxoxoooxooxoxo
  //|second |||||xooxoxox    MAIN      xoooxooxoxox
  //|call |||||||xooxoxox    MAIN      xoooxooxoxox
  //|vertical |||xooxoxox   CONTENT    xoooxooxoxox
  //|||||||||||||xoxoxoxoxoooxooxoxoxoxoooxooxoxoxo
  //|||||||||||||xoxoxoxoxoooxooxoxoxoxoooxooxoxoxo
  //|||||||||||||xoxoxoxoxoooxooxoxoxoxoooxooxoxoxo

  //|||||||||||||----------------------------------
  //|||||||||||||-----second call horizontal-------
  //|||||||||||||----------------------------------
  //|first ||||||xooxoxoxoxoooxooxoxoxoxoooxooxoxox
  //|call |||||||xooxoxox    MAIN      xoooxooxoxox
  //|vertical |||xooxoxox   CONTENT    xoooxooxoxox
  //|||||||||||||xooxoxoxoxoooxooxoxoxoxoooxooxoxox
  //|||||||||||||xooxoxoxoxoooxooxoxoxoxoooxooxoxox
  //|||||||||||||xooxoxoxoxoooxooxoxoxoxoooxooxoxox

  //  if (bar.isHorizontal()) {
  //    float endY = HBRect::start.y + height;
  //    float endX = start.x + HBUI::getWindowSize().x;
  //    HBRect::end = ImVec2(endX, endY);
  //
  //    //1 px offset for overlap
  //    HBRect::start.x -= 1;
  //    HBRect::end.x   += 1;
  //    HBRect::start.x += drawData->cursorPos.x;
  //    drawData->cursorPos.y += height;
  //  } else if (bar.isVertical()) {
  //    drawData->cursorPos.x +=  width;
  //  }

  ImVec2 panelStart = {};
  ImVec2 panelEnd   = {};

  ImVec2 vpStart = HBUI::getViewportPos();
  ImVec2 vpSize  = HBUI::getViewportSize();
  ImVec2 vpEnd   = vpStart + vpSize;

  ImVec2 min = vpStart;
  ImVec2 max = {};

  if (isHorizontal()) {
    vpStart.x += drawData->cursorPos.x;
    vpStart.y += drawData->cursorPos.y;

    min = vpStart;
    max = {vpEnd.x, vpStart.y + height};

    drawData->cursorPos.y += height;
  } else if (isVertical()) {
    vpStart.x += drawData->cursorPos.x;
    vpStart.y += drawData->cursorPos.y;

    min = vpStart;
    max = {vpStart.x + width, vpEnd.y};

    drawData->cursorPos.x += width;
  }

  HBRect::start = min;
  HBRect::end   = max;

  ImGui::SetNextItemAllowOverlap();

  ImGui::SetNextWindowPos(HBRect::start);
  ImGui::SetNextWindowSize(HBRect::Size());
  ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);

  ImVec2 windowPadding    = ImVec2(0.0f, 0.0f);
  float  windowRounding   = 0.0f;
  float  WindowBorderSize = 0.0f;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, windowPadding);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, WindowBorderSize);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, windowRounding);

  bool p_Open = false;
  auto genId = ImGui::GetID(id.c_str());
  p_Open = ImGui::Begin(id.c_str(), nullptr, window_flags);

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

  HBRect::draw(drawList, color, true);
  return p_Open;
}

void HBMainMenuBar::update(float deltaTime) {
}

void HBMainMenuBar::append(std::shared_ptr<HBMenuButton> item) {
  const ImVec2  spacing      = item->getSpacing();
  const HBStyle &style       = HBUI::getStyle();
  const bool    checkForSize = (HB_MAIN_MENU_BAR_FLAG_NO_CALCULATE_SIZE & flags) == 0;

  if (isHorizontal()) {
    ImVec2 start = nextItemPos;
    start.x += spacing.x;//spacing.left;
    start.y      = spacing.y; //spacing.top;

    item->pos = start + style.mainMenuBarHorizontalFirstItemOffset;

    nextItemPos.x = start.x + item->size.x;// + spacing.right;
    nextItemPos.y = start.y;

    ImVec2 totalItemSize = ImVec2(item->pos.x + item->size.x + spacing.x, // spacing.left + spacing.right,
                                  item->pos.y + item->size.y + spacing.y);// spacing.top  + spacing.bottom);

    if (totalItemSize.y > height && checkForSize) {
      {
        height = totalItemSize.y;
      }
    }
  } else if (isVertical()) {
    ImVec2 start = nextItemPos;
    start.x = spacing.x; //spacing.left;
    start.y += spacing.y;//spacing.top;

    item->pos = start + style.mainMenuBarVerticalFirstItemOffset;

    nextItemPos.x = start.x;
    nextItemPos.y = start.y + item->size.y;// + spacing.bottom;

    ImVec2 totalItemSize = ImVec2(item->pos.x + item->size.x + spacing.x, // spacing.left + spacing.right,
                                  item->pos.y + item->size.y + spacing.y);// spacing.top  + spacing.bottom);

    if (totalItemSize.x > width && checkForSize) {
      width = totalItemSize.x;
    }
  }

  items.push_back(item);
}

bool HBMainMenuBar::isHorizontal() const {
  return flags & HB_MAIN_MENU_BAR_FLAG_HORIZONTAL;
};

bool HBMainMenuBar::isVertical() const {
  return flags & HB_MAIN_MENU_BAR_FLAG_VERTICAL;
}

bool HBMainMenuBar::useCustomStyle() const {
  return flags & HB_MAIN_MENU_BAR_FLAG_USE_HBUI_STYLE;
}

ImColor HBMainMenuBar::getColor() const {
  if (useCustomStyle() || HBUI::getStyle().useHBUIStyleMenuBarColor) {
    return HBUI::getStyle().menuBarColor;
  }

  return ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg];
}


//-----------------------------------------------------------------------------
// [SECTION] MenuItem
//-----------------------------------------------------------------------------
bool HBMenuButton::draw(ImDrawList *drawList, ImColor color, bool drawFilled) {
  if (color.Value == ImColor(-1, -1, -1, -1)) {
    ImColor(255, 0, 0, 255);
  }
  if (drawList == nullptr) {
    drawList = ImGui::GetWindowDrawList();
  }

  if (drawFilled) {
    drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), color);
  } else {
    drawList->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), color);
  }
  return true;
}

void HBMenuButton::update(float deltaTime) {
}

ImVec2 HBMenuButton::getSpacing() const {
  auto   style   = HBUI::getStyle();
  ImVec2 spacing = {12, 12};
  return spacing;

  //	if (this->spacing.x == -1 && this->spacing.y == -1) {
  //		auto curMenuBar = HBUI::getCurrentContext()->drawData->currentAppendingMenuBar;
  //		if (curMenuBar != nullptr && curMenuBar->useCustomStyle()) {
  //			spacing = style.mainMenuItemsSpacing;
  //		} else {
  //			spacing = imguiStyle.ItemSpacing;
  //		}
  //		return spacing;
  //	} else {
  //		return this->spacing;
  //	}
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
    const bool maximized            = isMaximized();
    const bool mainWindowNoTitleBar = (HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR & getCurrentContext()->io.mainWindowFlags);
    const bool hasMenuBar           = (HB_DOCKSPACE_FLAG_MENUBAR & flags);
    return beginFullScreenDockspace(maximized, mainWindowNoTitleBar, hasMenuBar);
  }

  HBUI_API bool
  beginFullScreenDockspace(const bool isMaximized, const bool mainWindowNoTitleBar, const bool hasMenuBar) {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    ImGuiViewport    *viewport    = ImGui::GetMainViewport();

    ImGuiStyle &style   = ImGui::GetStyle();
    auto       drawData = HBUI::getCurrentContext()->drawData;

    ImGui::SetNextWindowPos(getCursorViewportPos());
    ImGui::SetNextWindowSize(getContentRegionAvail());
    ImGui::SetNextWindowViewport(viewport->ID);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;

    ImVec2 WindowPaddingNormal    = ImVec2(0.0f, 0.0f);
    ImVec2 WindowPaddingMaximized = ImVec2(6.0f, 6.0f);

    float windowRounding   = 0;
    float WindowBorderSize = 0.0f;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? WindowPaddingMaximized : WindowPaddingNormal);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, WindowBorderSize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, windowRounding);

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
  }

  /**
   *
   * ******************
   * MenuBar
   * ******************
   *
   */
  HBUI_API bool
  beginMainMenuBar(const std::string& id, MainMenuBarFlags flags, ImVec2 size) {
    HBContext *ctx = HBUI::getCurrentContext();
    IM_ASSERT(ctx->drawData->currentAppendingMenuBar == nullptr &&
              "A menu-bar is already active! Please call EndMainMenuBar() before starting a new one.");

    const bool horizontal = (HB_MAIN_MENU_BAR_FLAG_HORIZONTAL & flags) ||
                            (HB_MAIN_MENU_BAR_FLAG_NONE & flags);
    const bool vertical   = (HB_MAIN_MENU_BAR_FLAG_VERTICAL & flags);
    IM_ASSERT(!(vertical && horizontal) &&
              "Cannot create a menu-bar with both horizontal and vertical flags! Create two separate menu-bars instead.");

    ctx->drawData->currentAppendingMenuBar = std::make_shared<HBMainMenuBar>(flags, size);
    ctx->drawData->currentAppendingMenuBar->id = id;

    ctx->drawData->mainMenuBars.push_back(ctx->drawData->currentAppendingMenuBar);

    return true;

  }// create and append to a full screen menu-bar

  IMGUI_API void
  endMainMenuBar() {
    HBContext *ctx           = HBUI::getCurrentContext();
    auto      currentMenuBar = ctx->drawData->currentAppendingMenuBar;
    IM_ASSERT(currentMenuBar != nullptr &&
              "No menu-bar is currently active, did you forgot to call beginMainMenuBar() before calling endMainMenuBar().");

    //draw the back ground rect
    currentMenuBar->draw(nullptr);

    const HBMainMenuBar &bar   = *ctx->drawData->currentAppendingMenuBar;
    const HBStyle       &style = HBUI::getStyle();

    //draw the menu items
    for (auto &child: ctx->drawData->currentAppendingMenuBar->items) {
      if (bar.isHorizontal()) {
        child->pos.x += bar.start.x;
        child->pos.y += bar.start.y;
      } else if (bar.isVertical()) {
        child->pos.x += bar.start.x;
        child->pos.y += bar.start.y;
      }
      child->draw(nullptr, ImGui::GetStyleColorVec4(ImGuiCol_Button), true);
    }

    ctx->drawData->currentAppendingMenuBar = nullptr;
    //    ImGui::GetStyle().WindowMinSize = ctx->drawData->savedScreenPos; //fixme: disabled for now, because it breaks the main menu bar
    ImGui::End();
  }

  //menu items
  IMGUI_API bool
  menuBarButton(const std::string &id, ImVec2 size) {
    IM_ASSERT(id != "" && "Menu item id cannot be empty!");
    auto currentMenuBar = HBUI::getDrawData().currentAppendingMenuBar;
    IM_ASSERT(currentMenuBar != nullptr && "No menu-bar is currently active!");

    if (size == ImVec2(0, 0)) {
      size = HBUI::getStyle().mainMenuItemSize;
    }

    std::shared_ptr<HBMenuButton> menuItem = std::make_shared<HBMenuButton>(id, Square, size);
    currentMenuBar->append(menuItem);

    return true;
  }

  IMGUI_API bool
  mainMenuBarItem(const std::string &id, float radius) {
    IM_ASSERT(id != "" && "Menu item id cannot be empty!");

    return true;
  }
}// namespace HBUI