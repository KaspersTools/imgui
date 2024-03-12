//
// Created by Kasper de Bruin on 04/03/2024.
//

//clang-format off
#include "HBWidgetColorPropertiesBuilder.h"

#include <HBUI/HBUI.h>
#include <memory>

//clang-format on

namespace HBUI::Builder {
  HBWidgetColorPropertiesBuilder &HBWidgetColorPropertiesBuilder::setBackgroundFromImGuiStyle(HBUIType_ type) {
    IM_ASSERT(type != HBUIType_None && "HBUIType_None is not a valid type");
    bool setColorToImGui = false;
    if (m_ColorProperties->m_BackgroundColor.Value <= ImVec4(0, 0, 0, 0)) {
      setColorToImGui = true;
    }

    if (m_ColorProperties->m_BackgroundColor.Value == ImVec4(-1, -1, -1, -1)) {
      setColorToImGui = true;
    }

    this->setUseImGui(setColorToImGui);

    switch (type) {
      case HBUIType_TaskBar: {
        this->setBackgroundImGuiSource(ImGuiCol_MenuBarBg);
        break;
      }
      case HBUIType_SideBarButton:
      case HBUIType_Button: {
        this->setBackgroundImGuiSource(ImGuiCol_Button);
        break;
      }
      case HBUIType_Window:
        this->setBackgroundImGuiSource(ImGuiCol_WindowBg);
        break;
      case HBUIType_None://will never reach this because of the assert
        break;
    }
    return *this;
  }

  HBWidgetColorPropertiesBuilder &HBWidgetColorPropertiesBuilder::fromImGuiStyle(HBUIType_      type,
                                                                                 const ImColor &backgroundColor,
                                                                                 const ImColor &textColor,
                                                                                 const ImColor &borderColor,
                                                                                 const ImColor &hoverColor) {
    IM_ASSERT(type != HBUIType_None && "HBUIType_None is not a valid type");

    this->setTextColor(textColor);
    this->setBackgroundColor(backgroundColor);
    this->setBorderColor(borderColor);
    this->setHoverColor(hoverColor);

    //set color based on imgui style
    setBackgroundFromImGuiStyle(type);

    if (textColor.Value > ImVec4(-1, -1, -1, -1)) {
      this->setTextColor(ImGui::GetStyleColorVec4(ImGuiCol_Text));
    }

    if (hoverColor.Value <= ImVec4(0, 0, 0, 0)) {
      this->setHoverColor(ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
    }
    if (borderColor.Value == ImVec4(-1, -1, -1, -1)) {
      this->setBorderColor(ImGui::GetStyleColorVec4(ImGuiCol_Border));
    }


    return *this;
  }

  HBUI::Properties::WidgetColorProperties *HBWidgetColorPropertiesBuilder::build() {
    HBUI::Properties::WidgetColorProperties *properties = m_ColorProperties;

    m_ColorProperties = nullptr;
    return properties;
  }
}// namespace HBUI::Builder
