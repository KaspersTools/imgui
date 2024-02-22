//
// Created by Kasper de Bruin on 21/02/2024.
//

#ifndef IMGUI_HBSIDEBAR_H
#define IMGUI_HBSIDEBAR_H

namespace HBUI {
  class HBSideBar : public RectWidget {
  public:
    HBSideBar(
        const ImGuiID &id, const std::string &label,
        const HBSideBarFlags flags, const ImVec2 &position,
        const ImVec2 &size, const HBDrawLocation drawLocationFlag
    ) : RectWidget(id, label,
                   HBUIType_::HBSIDEBAR, nullptr,
                   position, size,
                   true,
                   drawLocationFlag,
                   HBLayoutType_::Horizontal,
                   HBWidgetResizeType_ScaleToChildren, HBWidgetResizeType_ScaleToChildren
    ) {
      m_flags = flags;
    }

    ~HBSideBar() {
    }

    void
    render() override {
      RectWidget::render();
    }

  private:
    HBSideBarFlags m_flags = 0;
  };
}


#endif //IMGUI_HBSIDEBAR_H
