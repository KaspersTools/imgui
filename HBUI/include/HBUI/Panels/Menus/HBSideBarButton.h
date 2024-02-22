//
// Created by Kasper de Bruin on 22/02/2024.
//

#ifndef IMGUI_SIDEBARBUTTON_H
#define IMGUI_SIDEBARBUTTON_H

namespace HBUI {
  class HBSideBarButton : public RectWidget {
  public:
    HBSideBarButton(
        const ImGuiID &id, const std::string &label,
        const ImVec2 &localPositionOffset, const ImVec2 &size,
        const HBDrawLocation drawLocationFlag
    ) : RectWidget(id, label,
                   HBUIType_::HBSIDEBARBUTTON, nullptr,
                   localPositionOffset, size, false,
                   drawLocationFlag,
                   HBLayoutType_::Horizontal,
                   HBWidgetResizeType_ScaleToChildren, HBWidgetResizeType_ScaleToChildren
    )
    {
    }

    ~HBSideBarButton() {
    }

    void
    render() override {
      RectWidget::render();
    }
  };
}
#endif //IMGUI_SIDEBARBUTTON_H
