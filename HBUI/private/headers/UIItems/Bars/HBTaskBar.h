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

    void draw(const ImVec2 &size, const ImVec2 &screenPos) override {
//      getDrawList()->AddRectFilled(screenPos, ImVec2(screenPos.x + size.x, screenPos.y + size.y), ImColor(255,0,0,255));
    }

    bool afterEnd() override;
  };

}// namespace HBUI::Bars


#endif//IMGUI_HBTASKBAR_H
