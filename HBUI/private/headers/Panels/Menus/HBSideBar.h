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
		    const HBSideBarFlags flags,
		    const ImVec2 &position,
		    const ImVec2 &size,
		    const ImVec2 &cursorPos,
		    const ImVec4 &padding,
		    const HBDrawLocation drawLocationFlag,
		    const HBLayoutType_ layoutType,
		    const HBWidgetResizeType_ widthResizeType,
		    const HBWidgetResizeType_ heightResizeType) : RectWidget(id, label,
		                                                             HBUIType_SideBar,
		                                                             position,
		                                                             size,
		                                                             cursorPos,
		                                                             padding,
		                                                             true,
		                                                             drawLocationFlag,
		                                                             layoutType,
		                                                             widthResizeType, heightResizeType),
		                                                  m_flags(flags) {
		}

		~HBSideBar() {
		}

		void render() override {
			RectWidget::render();
		}

		ImColor getBackgroundColor(const bool useHBUIColor = false) const override {
			IM_ASSERT(!useHBUIColor && "Not implemented");

			return ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg);
		}

private:
		HBSideBarFlags m_flags = 0;
	};
}// namespace HBUI


#endif//IMGUI_HBSIDEBAR_H
