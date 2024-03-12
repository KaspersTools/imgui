//
// Created by Kasper de Bruin on 01/03/2024.
//

#ifndef IMGUI_HBWIDGETCOLORPROPERTIES_H
#define IMGUI_HBWIDGETCOLORPROPERTIES_H

#include <HBUI/HBUI.h>

namespace HBUI {
  namespace Builder {
    class HBWidgetColorPropertiesBuilder;
  };// namespace Builder

  namespace Properties {
    struct [[maybe_unused]] WidgetColorProperties {
      friend class HBUI::HBIWidget;
      friend class HBUI::Builder::HBWidgetColorPropertiesBuilder;

    private:
      [[maybe_unused]] WidgetColorProperties() = default;
      [[maybe_unused]] WidgetColorProperties(const ImColor &backgroundColor,
                                             ImGuiCol_      backgroundColumn,
                                             const ImColor &hoverColor,
                                             const ImColor &textColor,
                                             const ImColor &borderColor);

    private:
      [[maybe_unused]] void pushColorProperties();
      [[maybe_unused]] void popColorProperties() const;

    private:
      int m_PushCount = 0;
      bool m_UseImGui = true;

      ImColor m_TextColor       = ImColor(0, 255, 0, 255);
      ImColor m_BorderColor     = ImColor(0, 0, 255, 255);
      ImColor m_HoverColor      = ImColor(140, 140, 140, 255);
      ImColor m_BackgroundColor = ImColor(255, 0, 0, 255);

      ImGuiCol_ m_BackgroundColorTarget;
      ImGuiCol_ m_BackgroundColumnSource;

    public:



    };
  };  // namespace Properties
};    // namespace HBUI
#endif//IMGUI_HBWIDGETCOLORPROPERTIES_H
