//
// Created by Kasper de bruin on 07/03/2024.
//

#ifndef IMGUI_HBBUTTON_H
#define IMGUI_HBBUTTON_H
#include <HBUI/HBUI.h>
#include <UIItems/Interfaces/HBIWidget.h>
#include <fonts/FontLoader.h>
#include <fonts/Fonts.h>
#include <magic_enum.hpp>

namespace HBUI::Buttons {

  class HBButton : public HBIWidget {
  public:
    [[maybe_unused]] HBButton(std::string                              label,
                              HBButtonType_                            buttonType,
                              HBMouseButtons_                          interactionButton = HBMouseButtons_Left,
                              const ImVec2                            &localPos          = ImVec2(0, 0),
                              const ImVec2                            &size              = ImVec2(-1, -1),
                              const float                              rounding          = -1,
                              HBStyleFlags                             styleFlags        = HBStyleFlags_CanHover,
                              bool                                     canBeOwnParent    = false,
                              HBDirection_                             direction         = HBDirection_LeftToRight,
                              const ImVec2                            &startCursorPos    = ImVec2(0, 0),
                              HBUI::Properties::WidgetColorProperties *colorProperties   = HBUI::Builder::HBWidgetColorPropertiesBuilder().fromImGuiStyle(HBUIType_Button).build())
        : c_ButtonType(buttonType), backupFont(HBUI::getFont()), m_RoundingInput(rounding),
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
      backupFont = HBUI::getFont();
    }

    [[maybe_unused]] HBButton(std::string                              label,
                              HBMouseButtons_                          interactionButton = HBMouseButtons_Left,
                              const ImVec2                            &localPos          = ImVec2(0, 0),
                              const ImVec2                            &size              = ImVec2(-1, -1),
                              const float                              rounding          = -1,
                              HBStyleFlags                             styleFlags        = HBStyleFlags_CanHover,
                              bool                                     canBeOwnParent    = false,
                              HBDirection_                             direction         = HBDirection_LeftToRight,
                              const ImVec2                            &startCursorPos    = ImVec2(0, 0),
                              HBUI::Properties::WidgetColorProperties *colorProperties   = HBUI::Builder::HBWidgetColorPropertiesBuilder().fromImGuiStyle(HBUIType_Button).build())
        : HBButton(std::move(label),
                   HBButtonType_Text,
                   interactionButton,
                   localPos,
                   size,
                   rounding,
                   styleFlags,
                   canBeOwnParent,
                   direction,
                   startCursorPos,
                   colorProperties) {
    }


    [[maybe_unused]] explicit HBButton([[maybe_unused]] Fonts::HBIcon          *icon,
                                       HBMouseButtons_                          interactionButton = HBMouseButtons_Left,
                                       const ImVec2                            &localPos          = ImVec2(0, 0),
                                       const ImVec2                            &size              = ImVec2(-1, -1),
                                       const float                              rounding          = 12,
                                       HBStyleFlags                             styleFlags        = HBStyleFlags_CanHover,
                                       bool                                     canBeOwnParent    = false,
                                       HBUI::Properties::WidgetColorProperties *colorProperties   = HBUI::Builder::HBWidgetColorPropertiesBuilder().fromImGuiStyle(HBUIType_Button).build())
        : HBButton(std::move(HBUI::wchar32ToUtf8(icon->glyph)),
                   HBButtonType_Icon,
                   HBMouseButtons_Left,
                   localPos,
                   size,
                   rounding,
                   styleFlags,
                   canBeOwnParent,
                   HBDirection_LeftToRight,
                   {0, 0},
                   colorProperties) {
    }

  private:
  protected:
  public:
    [[nodiscard]] ImVec2 calculateSize_impl() override {
      if (m_CalculatedSize != ImVec2(0, 0)) {
        return m_CalculatedSize;
      }

      HBStyle style        = HBUI::getStyle();
      ImVec2  framePadding = style.getFramePadding();//use frame padding for the text

      float width  = 0;
      float height = 0;

      m_CalculatedTextSize = ImGui::CalcTextSize(getLabel().c_str(), nullptr, true);

      if (getButtonType() == HBButtonType_Icon) {
        width  = style.TaskBarIconSize.x;
        height = style.TaskBarIconSize.y;
      } else {
        if (getInputSize().x == 0) {//only has set on the x axis
          width = m_CalculatedTextSize.x;
        } else {
          width = getInputSize().x;
        }
        if (getInputSize().y == 0) {//only has set on the y axis
          height = m_CalculatedTextSize.y;
        } else {
          height = getInputSize().y;
        }
      }

      width += framePadding.x * 2;
      height += framePadding.y * 2;

      m_CalculatedSize = ImVec2(width, height);
      return {width, height};
    }

  private:
    ImVec2 getButtonCenter() {
      ImVec2 size = calculateSizeWithPadding();
      return size * 0.5f;
    }

    ImVec2 getTextPos() {
      ImVec2 btnCenter = getButtonCenter();
      ImVec2 textSize  = ImGui::CalcTextSize(getLabel().c_str(), nullptr, true);
      return btnCenter - textSize * 0.5f;
    }

    float getRounding() {
      if (m_RoundingInput != -1) {
        return m_RoundingInput;
      }

      return ImGui::GetStyle().FrameRounding;
    }

  protected:
    bool beforeBegin() override {
      if (!hasSetPos()) {
      }

      if (getButtonType() == HBButtonType_Icon) {
        backupFont = HBUI::getFont();
        IM_ASSERT(backupFont != nullptr && "Icon is nullptr");

        ImVec2 fontSize = HBUI::getStyle().TaskBarIconSize;
        HBUI::activateFontSize(fontSize.x);
        iconFont = HBUI::getFont();
        HBUI::activateFont(backupFont);
        ImGui::PushFont(iconFont->font);
      }

      HBButtonState_ currentState = getCurrentState();
      HBUI::setButtonState(getID(), currentState);

      return true;
    }

    void draw(const ImVec2 &size, const ImVec2 &screenPos) override {
      IM_ASSERT(getDrawList() != nullptr && "DrawList is nullptr");
      IM_ASSERT(hasBegun() && "Begin has not been called");
      ImDrawList *drawList = getDrawList();

      HBButtonState_ m_ButtonState = HBUI::getButtonState(getID());

      ImColor color = ImGui::GetStyle().Colors[ImGuiCol_Button];
      switch (m_ButtonState) {
        case HBButtonState_Hovered:
          //          color = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
          color = ImColor(43, 45, 48, 255);
          break;
        case HBButtonState_Clicked:
          //          color = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
          break;
        case HBButtonState_Active:
          color = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
          break;
        case HBButtonState_None:
          break;
      }

      ImVec2 sizeWithPadding = calculateSizeWithPadding();
      if (getButtonType() == HBButtonType_Icon) {
      }
      ImVec2 buttonMin = getScreenPos();

      ImVec2 buttonMax    = buttonMin + sizeWithPadding;
      ImVec2 framePadding = HBUI::getStyle().getFramePadding();

      float rounding = getRounding();

      drawList->AddRectFilled(buttonMin, buttonMax, color, rounding, ImDrawFlags_RoundCornersAll);
      drawList->AddText(buttonMin + getTextPos(), IM_COL32(255, 255, 255, 255), getLabel().c_str());

      if (getButtonType() == HBButtonType_Icon) {
        ImGui::PopFont();
      }
    }


    [[nodiscard]] HBButtonType_ getButtonType() const {
      return c_ButtonType;
    }

  public:
    [[nodiscard]] HBButtonState_ getCurrentState() {
      auto *ctx = HBUI::getCurrentContext();

      IM_ASSERT(ctx != nullptr && "Context is nullptr");
      ImVec2 mousePos  = HBUI::getMousePos();
      ImVec2 screenPos = getScreenPos();

      ImVec2 max = screenPos + calculateSize_impl();

      bool isHovered = HBUI::containsPoint(screenPos, max, mousePos);
      bool mouseDown = HBUI::isMouseButtonDown(HBMouseButtons_Left);
      if (isHovered) {
        if (mouseDown) {
//          std::cout << "Clicked" << std::endl;
          return HBButtonState_Clicked;
        }
//        std::cout << "Hover" << std::endl;
        return HBButtonState_Hovered;
      }
//      std::cout << "None" << std::endl;
      return HBButtonState_None;
    }

  private:
    const HBButtonType_ c_ButtonType;
    Fonts::HBIcon      *icon       = nullptr;
    Fonts::HBFont      *iconFont   = nullptr;
    Fonts::HBFont      *backupFont = nullptr;

    ImVec2 m_CalculatedSize     = ImVec2(0, 0);
    ImVec2 m_CalculatedTextSize = ImVec2(0, 0);

    float m_RoundingInput = -1;
  };

}// namespace HBUI::Buttons


#endif//IMGUI_HBBUTTON_H
