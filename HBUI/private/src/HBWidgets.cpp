//
// Created by Kasper de Bruin on 10/02/2024.
//
#include <HBUI/HBUI.h>

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
 *
 * ******************
 * DOCKSPACES
 * ******************
 *
 */
  HBUI_API void
  beginFullScreenDockspace(const bool isMaximized){
    auto viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowMinSize.x = 370.0f;

    ImGui::SetNextWindowPos({viewport->WorkPos.x, viewport->WorkPos.y});
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImVec2 WindowPaddingNormal = ImVec2(1.0f, 1.0f);
    ImVec2 WindowPaddingMaximized = ImVec2(6.0f, 6.0f);

    float WindowBorderSize = 0.0f;
    ImVec4 MenuBarBG = {0.0f, 0.0f, 0.0f, 0.0f};

    if (isMaximized) {
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, WindowPaddingMaximized);
    } else {
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, WindowPaddingNormal);
    }

    ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar(2);
    ImGui::PopStyleVar(1);

    ImGui::DockSpace(ImGui::GetID("MyDockspace"));
  }

  HBUI_API void
  endFullScreenDockspace() {
    ImGui::End();
  }
}// namespace HBUI