//
// Created by Kasper de bruin on 08/03/2024.
//

#ifndef IMGUI_HBWINDOW_H
#define IMGUI_HBWINDOW_H

#include <UIItems/Interfaces/HBIWidget.h>

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "NullDereference"
inline static void       renderWindowOuterBorders(ImGuiWindow *window) {
  IM_ASSERT(window != nullptr && "renderWindowOuterBorders() called with nullptr window");

  struct ImGuiResizeBorderDef {
    ImVec2 InnerDir;
    ImVec2 SegmentN1, SegmentN2;
    float  OuterAngle{};
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

  ImGuiContext &g           = *GImGui;
  float         rounding    = window->WindowRounding;
  float         border_size = 1.0f;// window->WindowBorderSize;
  if (border_size > 0.0f && !(window->Flags & ImGuiWindowFlags_NoBackground)) {
    window->DrawList->AddRect(window->Pos, {window->Pos.x + window->Size.x, window->Pos.y + window->Size.y},
                                    ImGui::GetColorU32(ImGuiCol_Border), rounding, 0, border_size);
  }

  int border_held = window->ResizeBorderHeld;
  if (border_held != -1) {
    const ImGuiResizeBorderDef &def      = resize_border_def[border_held];
    ImRect                      border_r = GetResizeBorderRect(window, border_held, rounding, 0.0f);
    ImVec2                      p1       = ImLerp(border_r.Min, border_r.Max, def.SegmentN1);
    const float                 offsetX  = def.InnerDir.x * rounding;
    const float                 offsetY  = def.InnerDir.y * rounding;
    p1.x += 0.5f + offsetX;
    p1.y += 0.5f + offsetY;

    ImVec2 p2 = ImLerp(border_r.Min, border_r.Max, def.SegmentN2);
    p2.x += 0.5f + offsetX;
    p2.y += 0.5f + offsetY;

    window->DrawList->PathArcTo(p1, rounding, def.OuterAngle - IM_PI * 0.25f, def.OuterAngle);
    window->DrawList->PathArcTo(p2, rounding, def.OuterAngle, def.OuterAngle + IM_PI * 0.25f);
    window->DrawList->PathStroke(ImGui::GetColorU32(ImGuiCol_SeparatorActive), 0,
                                       ImMax(2.0f, border_size));// Thicker than usual
  }
  if (g.Style.FrameBorderSize > 0 && !(window->Flags & ImGuiWindowFlags_NoTitleBar) && !window->DockIsActive) {
    float y = window->Pos.y + window->TitleBarHeight() - 1;
    window->DrawList->AddLine(ImVec2(window->Pos.x + border_size, y),
                                    ImVec2(window->Pos.x + window->Size.x - border_size, y),
                                    ImGui::GetColorU32(ImGuiCol_Border), g.Style.FrameBorderSize);
  }
}
#pragma clang diagnostic pop

namespace HBUI::Bars {
  class HBTaskBar;
}
namespace HBUI::Windows {
  class HBWindow : public HBUI::HBIWidget {
  public:
    explicit HBWindow(
        std::string                        label,
        ImGuiID                            id               = -1,
        HBUIWindowFlags                    flags            = HBUIWindowFlags_None,
        ImGuiWindowFlags                   imguiWindowFlags = ImGuiWindowFlags_None,
        const ImVec2                      &size             = {0, 0},
        const ImVec2                      &localPos         = {0, 0},
        HBDirection_                       direction        = HBDirection_TopToBottom,
        const ImVec2                      &startCursorPos   = {0, 0},
        HBStyleFlags                       styleFlags       = HBStyleFlags_None,
        Properties::WidgetColorProperties *colorProperties  = nullptr,
        HBUIType_                          type             = HBUIType_Window)

        : c_DefaultFlags(flags),
          m_Flags(flags),
          c_DefaultImGuiWindowFlags(imguiWindowFlags), m_ImguiWindowFlags(imguiWindowFlags),
          HBIWidget(id,
                    std::move(label),
                    localPos,
                    size,
                    type,
                    styleFlags,
                    true,
                    colorProperties == nullptr ? getColorPropertiesForWindow(flags) : colorProperties,
                    direction,
                    startCursorPos) {


      if(m_Flags & HBUIWindowFlags_MainWindow){
        HBUI::getCurrentContext()->mainWindow = this;
        HBUI::getCurrentContext()->mainWindowFlags = m_Flags;
        HBUI::getCurrentContext()->imGuiMainWindowFlags = imguiWindowFlags;
        HBUI::getCurrentContext()->mainWindowTitle = getLabel();
      }

      if(localPos.x != 0 && localPos.y != 0) {
        m_ImguiWindowFlags |= ImGuiWindowFlags_NoMove;
      }
      if(size.x != 0 && size.y != 0) {
        m_ImguiWindowFlags |= ImGuiWindowFlags_NoResize;
      }
    }

  protected:
    inline static Properties::WidgetColorProperties *getColorPropertiesForWindow(HBUIWindowFlags windowFlags) {
      return HBUI::Builder::HBWidgetColorPropertiesBuilder().fromImGuiStyle(HBUIType_Window).build();
    }

  private:
    void drawDefaultWindow() {
      ImGui::Begin(getLabel().c_str(), nullptr, m_ImguiWindowFlags);
      setDrawList(ImGui::GetWindowDrawList());
    }

    void drawNoMoveMainViewportWindow() {
      ImGuiViewport *viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::SetNextWindowPos(getScreenPos());
      ImVec2 tempSize = calculateSize();
      ImGui::SetNextWindowSize(tempSize);

      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

      const bool isMaximized = false;
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(0.0f, 0.0f));
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

      ImGui::Begin(getLabel().c_str(), nullptr, m_ImguiWindowFlags);
      setDrawList(ImGui::GetWindowDrawList());
      ImGui::PopStyleVar(2);
      ImGui::PopStyleVar(2);

      {
        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
        if (!isMaximized) {
          renderWindowOuterBorders(ImGui::GetCurrentWindow());
        }

        ImGui::PopStyleColor();// ImGuiCol_Border
      }
    }

    void drawBase() {
      if (m_Flags & HBUIWindowFlags_MainWindow) {
        drawNoMoveMainViewportWindow();
      } else {
        drawDefaultWindow();
      }
      setDrawList(ImGui::GetWindowDrawList());
    }

  public:
    void afterTaskBarEnd(Bars::HBTaskBar *taskBar);

  protected:
    bool beforeBegin() override {
      if (m_Flags & HBUIWindowFlags_HasTaskBar) {
        drawBase();
      }
      return true;
    }
    bool beforeDraw() override {
      return true;
    }
    void draw() override {
      m_ScreenPos = ImGui::GetCurrentWindow()->Pos;
      ImGui::End();
    }
    bool afterEnd() override {
      return true;
    }

  public:
    ImVec2 getScreenPos() override;


  public:
    bool windowFlagSet(HBUIWindowFlags flag) const {
      return (m_Flags & flag) == flag;
    }

  private:
    const HBUIWindowFlags c_DefaultFlags;
    HBUIWindowFlags       m_Flags          = 0;
    HBUIWindowFlags       m_FlagsNextFrame = 0;

    const ImGuiWindowFlags c_DefaultImGuiWindowFlags;
    ImGuiWindowFlags       m_ImguiWindowFlags          = 0;
    ImGuiWindowFlags       m_ImguiWindowFlagsNextFrame = 0;

    ImVec2 m_ScreenPos = {0, 0};
  };
}// namespace HBUI::Windows


#endif//IMGUI_HBWINDOW_H
