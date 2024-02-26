//
// Created by Kasper de Bruin on 10/02/2024.
//

#include <UIItems/HBUIItemBase.h>
#include <UIItems/HBNewLine.h>
#include <Panels/Menus/HBSideBar.h>
#include <Panels/Menus/HBSideBarButton.h>

#include <imgui.h>

static bool aabb(const ImVec2 &min1, const ImVec2 &max1, const ImVec2 &min2, const ImVec2 &max2) {
  return (min1.x < max2.x && max1.x > min2.x) && (min1.y < max2.y && max1.y > min2.y);
}

void renderWindowOuterBorders(ImGuiWindow *window) {
  struct ImGuiResizeBorderDef {
    ImVec2 InnerDir;
    ImVec2 SegmentN1, SegmentN2;
    float  OuterAngle;
  };

  static const ImGuiResizeBorderDef resize_border_def[4] =
                                        {
                                            {ImVec2(+1, 0), ImVec2(0, 1), ImVec2(0, 0), IM_PI * 1.00f}, // Left
                                            {ImVec2(-1, 0), ImVec2(1, 0), ImVec2(1, 1), IM_PI * 0.00f}, // Right
                                            {ImVec2(0, +1), ImVec2(0, 0), ImVec2(1, 0), IM_PI * 1.50f}, // Up
                                            {ImVec2(0, -1), ImVec2(1, 1), ImVec2(0, 1), IM_PI * 0.50f}  // Down
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

  ImGuiContext &g          = *GImGui;
  float        rounding    = window->WindowRounding;
  float        border_size = 1.0f; // window->WindowBorderSize;
  if (border_size > 0.0f && !(window->Flags & ImGuiWindowFlags_NoBackground)) {
    window->DrawList->AddRect(window->Pos, {window->Pos.x + window->Size.x, window->Pos.y + window->Size.y},
                              ImGui::GetColorU32(ImGuiCol_Border), rounding, 0, border_size);
  }

  int border_held = window->ResizeBorderHeld;
  if (border_held != -1) {
    const ImGuiResizeBorderDef &def     = resize_border_def[border_held];
    ImRect                     border_r = GetResizeBorderRect(window, border_held, rounding, 0.0f);
    ImVec2                     p1       = ImLerp(border_r.Min, border_r.Max, def.SegmentN1);
    const float                offsetX  = def.InnerDir.x * rounding;
    const float                offsetY  = def.InnerDir.y * rounding;
    p1.x += 0.5f + offsetX;
    p1.y += 0.5f + offsetY;

    ImVec2 p2 = ImLerp(border_r.Min, border_r.Max, def.SegmentN2);
    p2.x += 0.5f + offsetX;
    p2.y += 0.5f + offsetY;

    window->DrawList->PathArcTo(p1, rounding, def.OuterAngle - IM_PI * 0.25f, def.OuterAngle);
    window->DrawList->PathArcTo(p2, rounding, def.OuterAngle, def.OuterAngle + IM_PI * 0.25f);
    window->DrawList->PathStroke(ImGui::GetColorU32(ImGuiCol_SeparatorActive), 0,
                                 ImMax(2.0f, border_size)); // Thicker than usual
  }
  if (g.Style.FrameBorderSize > 0 && !(window->Flags & ImGuiWindowFlags_NoTitleBar) && !window->DockIsActive) {
    float y = window->Pos.y + window->TitleBarHeight() - 1;
    window->DrawList->AddLine(ImVec2(window->Pos.x + border_size, y),
                              ImVec2(window->Pos.x + window->Size.x - border_size, y),
                              ImGui::GetColorU32(ImGuiCol_Border), g.Style.FrameBorderSize);
  }
}

namespace HBUI {
  HBUI_API void
  newLine(float size, HBWidgetResizeType_ resizeType) {
    HBNewLine *nl = new HBNewLine(size, resizeType);
    HBWidgetManager::appendWidget(nl);
  }


  HBUI_API bool
  beginSideBar(
      const ImGuiID &id,
      const HBSideBarFlags flags,
      const ImVec2 &position,
      ImVec2 size,
      const std::string &label,
      const HBDrawLocation drawLocationFlag
  ) {
    bool isHorizontal = flags & HBSideBarFlags_Horizontal;
    bool isVertical   = flags & HBSideBarFlags_Vertical;

    IM_ASSERT((isHorizontal || isVertical) && "A sidebar must be either horizontal or vertical");
    IM_ASSERT(!(isHorizontal && isVertical) && "A sidebar cannot be both horizontal and vertical");
    HBLayoutType_ layoutType = HBLayoutType_::HBLayoutType_Horizontal;

    if (isVertical) {
      layoutType = HBLayoutType_::HBLayoutType_Vertical;
    }
    HBWidgetResizeType_ resizeTypeXAxis = HBWidgetResizeType_::HBWidgetResizeType_ScaleToChildren;
    HBWidgetResizeType_ resizeTypeYAxis = HBWidgetResizeType_::HBWidgetResizeType_ScaleToChildren;

    if (flags & HBSideBarFlags_FullSize) {
      if (isHorizontal) {//todo: add cursor position if not over vp or foreground drawlist
        size.x = HBUI::getViewportSize().x;
        if (size.y != 0) {
          resizeTypeYAxis = HBWidgetResizeType_::HBWidgetResizeType_Fixed;
        }
      } else {
        size.y = HBUI::getViewportSize().y;
        if(size.x != 0) {
          resizeTypeXAxis = HBWidgetResizeType_::HBWidgetResizeType_Fixed;
        }
      }
    }

    HBSideBar *sideBar = new HBSideBar(id, label, flags, position, size, drawLocationFlag, layoutType,
                                       resizeTypeXAxis, resizeTypeYAxis);


    HBWidgetManager::appendWidget(sideBar);
    return true;
  }

  IMGUI_API void
  endSideBar() {
    HBUI::getCurrentContext()->widgetManager->endAppendingWidget(HBUIType_::HBSIDEBAR);
  }

  //menu items
  IMGUI_API bool
  sideBarBarButton(const ImGuiID id, const std::string& label, const ImVec2 &position, const ImVec2 &size) {
    HBSideBarButton *button = new HBSideBarButton(id, label,
                                                  position, size,
                                                  HBDrawLocation::HBDrawFlags_DrawOnParent);

    HBWidgetManager::appendWidget(button);

  }
}// namespace HBUI