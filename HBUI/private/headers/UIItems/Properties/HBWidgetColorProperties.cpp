//
// Created by Kasper de Bruin on 04/03/2024.
//

// clang-format off

#include <UIItems/Builders/HBWidgetColorPropertiesBuilder.h>

#include "HBWidgetColorProperties.h"

// clang-format on

namespace HBUI {
  namespace Properties {
    [[maybe_unused]] WidgetColorProperties::WidgetColorProperties(
        const ImColor &backgroundColor,
        ImGuiCol_      backgroundColumn,
        const ImColor &hoverColor,
        const ImColor &textColor,
        const ImColor &borderColor)
        : m_BackgroundColor(backgroundColor),
          m_BackgroundColorTarget(backgroundColumn),
          m_TextColor(textColor),
          m_BorderColor(borderColor),
          m_HoverColor(hoverColor){
    }


    void WidgetColorProperties::pushColorProperties() {
      return;
      if (m_UseImGui) {
        ImGui::PushStyleColor(m_BackgroundColorTarget, ImGui::GetStyleColorVec4(m_BackgroundColumnSource));
        m_PushCount+=1;
        return;
      }

      ImGui::PushStyleColor(ImGuiCol_Text, m_TextColor.Value);
      m_PushCount+=1;

      ImGui::PushStyleColor(ImGuiCol_Border, m_BorderColor.Value);
      m_PushCount+=1;

      ImGui::PushStyleColor(m_BackgroundColorTarget, m_BackgroundColor.Value);
      m_PushCount+=1;

      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_HoverColor.Value);
      m_PushCount+=1;
    }
    void WidgetColorProperties::popColorProperties() const {
      return;
      ImGui::PopStyleColor(m_PushCount);
    }

  };// namespace Properties
};  // namespace HBUI