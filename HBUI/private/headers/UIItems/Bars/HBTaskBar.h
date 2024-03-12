//
// Created by Kasper de bruin on 07/03/2024.
//

#ifndef IMGUI_HBTASKBAR_H
#define IMGUI_HBTASKBAR_H


#include <HBUI/HBUI.h>
#include <UIItems/Interfaces/HBIWidget.h>

namespace HBUI::Bars {
  class HBTaskBar : public HBIWidget {
  public:
    HBTaskBar(std::string                              label,
              HBIWidget                               *parentWidget,
              const ImVec2                            &localPos,
              const ImVec2                            &size            = {0, 0},
              HBDirection_                             direction       = HBDirection_LeftToRight,
              const ImVec2                            &startCursorPos  = {0, 0},
              HBStyleFlags                             styleFlags      = HBStyleFlags_StretchHorizontal,
              HBUI::Properties::WidgetColorProperties *colorProperties = nullptr)
        : HBIWidget(std::move(label),
                    localPos,
                    size,
                    HBUIType_TaskBar,
                    styleFlags,
                    false,
                    (colorProperties == nullptr ? HBUI::Builder::HBWidgetColorPropertiesBuilder().fromImGuiStyle(HBUIType_TaskBar, styleFlags).build() : colorProperties),
                    direction,
                    startCursorPos,
                    parentWidget) {
    }


  protected:
    bool beforeBegin() override;

    void draw() override {
      ImVec2 size   = calculateSize();
      float  width  = size.x;
      float  height = size.y;
    }

    bool afterEnd() override;
  };

}// namespace HBUI::Bars


#endif//IMGUI_HBTASKBAR_H
