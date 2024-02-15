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
std::shared_ptr<HBUIItem> HBUIItem::getChild(const std::string &id) {
  if (Children.empty()) {
    return nullptr;
  }

  for (auto &child: Children) {
    if (child->ID == id) {
      return child;
    }
  }
  return Children[0];
}
std::shared_ptr<HBUIItem> HBUIItem::getLastChild() {
  return Children[Children.size() - 1];
}
std::shared_ptr<HBUIItem> HBUIItem::getLastYPos() {
  std::shared_ptr<HBUIItem> lowestItem = Children[0];
  for (auto child: Children) {
    if (child->EndPos().y > lowestItem->EndPos().y) {
      lowestItem = child;
    }
  }
  return lowestItem;
}

void HBUIItem::startDraw(ImDrawList *drawList) {
  draw(drawList);
  drawChildren(drawList);
}
void HBUIItem::drawChildren(ImDrawList *drawList) {
  for (auto child: Children) {
    child->startDraw(drawList);
  }
};

void HBCircle::draw(ImDrawList *drawList) {
  drawList->AddCircle(Center, Radius, Color);
}
ImVec2 HBCircle::StartPos() const {
  return ImVec2(Center.x - Radius, Center.y - Radius);
}
ImVec2 HBCircle::EndPos() const {
  return ImVec2(Center.x + Radius, Center.y + Radius);
}

void HBRect::draw(ImDrawList *drawList) {
  drawList->AddRect(Start, End, Color);
}
ImVec2 HBRect::StartPos() const {
  return Start;
}
ImVec2 HBRect::EndPos() const {
  return End;
}

namespace HBUI {

  /**
   * ******************
   * Updatables
   * ******************
   */


  /**
 *
 * ******************
 * DOCKSPACES
 * ******************
 *
 */

  HBUI_API void
  beginFullScreenDockspace() {
    const bool maximized = isMaximized();
    const bool isCustomTitleBar = (HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR & getCurrentContext()->windowF());
    const bool mainWindowNoTitleBar = (HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR & getCurrentContext()->windowF());

    beginFullScreenDockspace(maximized, isCustomTitleBar, mainWindowNoTitleBar);
  }


  HBUI_API void
  beginFullScreenDockspace(const bool isMaximized,
                           const bool hasCustomTitlebar,
                           const bool mainWindowNoTitlebar) {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImGuiStyle &style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;

    if (mainWindowNoTitlebar) {
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
    } else {
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
    }

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
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));

    // Draw window border if the window is not maximized
    renderWindowOuterBorders(ImGui::GetCurrentWindow());

    ImGui::PopStyleColor();// ImGuiCol_Border
    //TODO: Start Title/Side bar here
    // {}

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

  HBUI_API void
  beginMainMenuBar() {
    ImGui::BeginMainMenuBar();
  }

  HBUI_API void
  endMainMenuBar() {
    ImGui::EndMainMenuBar();
    //    ImGui::SetCursorPos(g_HBUICTX->drawData.nextWindowPos);
  }
}// namespace HBUI