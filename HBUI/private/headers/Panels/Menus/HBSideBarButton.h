//
// Created by Kasper de Bruin on 22/02/2024.
//

#ifndef IMGUI_SIDEBARBUTTON_H
#define IMGUI_SIDEBARBUTTON_H

namespace HBUI {
	class HBSideBarButton : public HBButton {
public:
		HBSideBarButton(
		    const ImGuiID &id, const std::string &label,
		    const ImVec2 &localPositionOffset,
		    const ImVec2 &size,
		    const ImVec2 &cursorPos,
		    const ImVec4 &padding,
		    const HBDrawLocation drawLocationFlag) : HBButton(id, label,
		                                                      localPositionOffset,
		                                                      size,
		                                                      cursorPos,
		                                                      padding,
		                                                      drawLocationFlag) {
		}

		~HBSideBarButton() {
		}

		void
		    render() override {
			RectWidget::render();
		}
	};
}// namespace HBUI
#endif//IMGUI_SIDEBARBUTTON_H
