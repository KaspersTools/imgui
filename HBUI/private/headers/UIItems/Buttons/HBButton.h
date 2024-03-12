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
    ImVec2 calculateSize() override {
      float width  = 0;
      float height = 0;

      ImGuiStyle &style      = ImGui::GetStyle();

      ImVec2 size = ImGui::CalcTextSize(getLabel().c_str(), NULL, true);
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
      ImVec2 screenPos = getScreenPos();
      ImVec2 size      = calculateSize();

      return true;
    }

    void draw() override {
      IM_ASSERT(getDrawList() != nullptr && "DrawList is nullptr");
      IM_ASSERT(hasBegun() && "Begin has not been called");

      ImVec2 buttonMin = getScreenPos();
      ImVec2 buttonMax = buttonMin + calculateSize();

      ImDrawList *drawList = getDrawList();
      drawList->AddRectFilled(buttonMin, buttonMax, IM_COL32(255, 0, 0, 255));
      auto pad = ImGui::GetStyle().FramePadding;
      drawList->AddText(buttonMin+pad, IM_COL32(255, 255, 255, 255), getLabel().c_str());
    }


    [[nodiscard]] HBButtonType_ getButtonType() const {
      return c_ButtonType;
    }

    HBButtonState_ getButtonState() const {
      auto* ctx = HBUI::getCurrentContext();
      IM_ASSERT(ctx != nullptr && "Context is nullptr");
      if(ctx->isActiveButton(getID())) {
        return HBButtonState_Active;
      }

      ImVec2 mousePos = HBUI::getMousePos();

    }

  private:
    const HBButtonType_ c_ButtonType;
  };

}// namespace HBUI::Buttons


#endif//IMGUI_HBBUTTON_H
