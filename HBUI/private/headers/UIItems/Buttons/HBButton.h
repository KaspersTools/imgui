//
// Created by Kasper de bruin on 07/03/2024.
//

#ifndef IMGUI_HBBUTTON_H
#define IMGUI_HBBUTTON_H
#include <HBUI/HBUI.h>
#include <UIItems/Interfaces/HBIWidget.h>

namespace HBUI::Buttons {

  class HBButton : public HBIWidget {
  public:
    HBButton(std::string                              label,
             HBButtonType_                            buttonType,
             HBMouseButtons_                          interactionButton,
             const ImVec2                            &localPos        = ImVec2(0, 0),
             const ImVec2                            &size            = ImVec2(-1, -1),
             HBStyleFlags                             styleFlags      = HBStyleFlags_CanHover,
             bool                                     canBeOwnParent  = false,
             HBDirection_                             direction       = HBDirection_LeftToRight,
             const ImVec2                            &startCursorPos  = ImVec2(0, 0),
             HBUI::Properties::WidgetColorProperties *colorProperties = HBUI::Builder::HBWidgetColorPropertiesBuilder().fromImGuiStyle(HBUIType_Button).build())
        : c_ButtonType(buttonType),
          HBIWidget(ImGui::GetID(label.c_str()),
                    std::move(label),
                    localPos, size,
                    HBUIType_Button,
                    styleFlags,
                    canBeOwnParent,
                    colorProperties,
                    direction,
                    startCursorPos,
                    nullptr) {
    }

  private:
  protected:
  public:
    [[nodiscard]] ImVec2 calculateSize() const override {
      float width  = 0;
      float height = 0;

      ImGuiStyle &style = ImGui::GetStyle();

      ImVec2 size = ImGui::CalcTextSize(getLabel().c_str(), nullptr, true);
      if (getInputSize().x == 0) {//only has set on the x axis
        switch (getButtonType()) {
          case HBButtonType_Text:
            width = size.x + style.FramePadding.x * 2;
            break;
          case HBButtonType_Icon:
            width = 16;
            break;
        }
      } else {
        width = getInputSize().x;
      }

      if (getInputSize().y == 0) {//only has set on the y axis
        switch (getButtonType()) {
          case HBButtonType_Text:
            height = size.y + style.FramePadding.y * 2;
            break;
          case HBButtonType_Icon:
            height = 16;
            break;
        }
      } else {
        height = getInputSize().y;
      }

      return {width, height};
    }

  private:
  protected:
    bool beforeBegin() override {
      if (!hasSetPos()) {
      }

      HBButtonState_ currentState = getCurrentState();
      HBUI::setButtonState(getID(), currentState);

      return true;
    }

    void draw() override {
      IM_ASSERT(getDrawList() != nullptr && "DrawList is nullptr");
      IM_ASSERT(hasBegun() && "Begin has not been called");

      ImVec2 buttonMin = getScreenPos();
      ImVec2 buttonMax = buttonMin + calculateSize();

      ImDrawList *drawList = getDrawList();
      ImColor     color    = ImGui::GetStyle().Colors[ImGuiCol_Button];
      HBButtonState_ m_ButtonState = HBUI::getButtonState(getID());

      if (m_ButtonState == HBButtonState_Hovered) {
        color = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
      } else if (m_ButtonState == HBButtonState_Clicked) {
        color = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
      }
      drawList->AddRectFilled(buttonMin, buttonMax, color);
      auto pad = ImGui::GetStyle().FramePadding;
      drawList->AddText(buttonMin + pad, IM_COL32(255, 255, 255, 255), getLabel().c_str());
    }


    [[nodiscard]] HBButtonType_ getButtonType() const {
      return c_ButtonType;
    }

  private:
    [[nodiscard]] HBButtonState_ getCurrentState() const {
      auto *ctx = HBUI::getCurrentContext();

      IM_ASSERT(ctx != nullptr && "Context is nullptr");
      ImVec2 mousePos  = HBUI::getMousePos();
      ImVec2 screenPos = getScreenPos();
      ImVec2 size      = calculateSize();

      ImVec2 max = screenPos + size;

      bool isHovered = HBUI::containsPoint(screenPos, max, mousePos);
      bool mouseDown = HBUI::isMouseButtonDown(HBMouseButtons_Left);
      if (isHovered) {
        if (mouseDown) {
          return HBButtonState_Clicked;
        }
        return HBButtonState_Hovered;
      }
      return HBButtonState_None;
    }

  private:
    const HBButtonType_ c_ButtonType;
  };

}// namespace HBUI::Buttons


#endif//IMGUI_HBBUTTON_H
