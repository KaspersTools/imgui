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
		    const ImColor &backgroundColor                = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg),
		    const HBItemFlags itemFlags                   = HBItemFlags_DrawChildrenCentered,
		    const ImVec2 &position                        = ImVec2(0, 0),
		    const ImVec2 &size                            = ImVec2(0, 0),
		    const ImVec2 &cursorPos                       = ImVec2(0, 0),
		    const ImVec4 &padding                         = ImVec4(0, 0, 0, 0),
		    const ImVec4 &margin                          = ImVec4(0, 0, 0, 0),
		    const HBDrawLocation drawLocationFlag         = HBDrawFlags_CurrentWindowDrawList,
		    const HBLayoutType_ layoutType                = HBLayoutType_Vertical,
		    const HBWidgetResizeOptions_ widthResizeType  = HBWidgetResizeOptions_ScaleToChildren,
		    const HBWidgetResizeOptions_ heightResizeType = HBWidgetResizeOptions_ScaleToChildren
		    ) : RectWidget(id, label,
		                   HBUIType_SideBar,
		                   position,
		                   size,
		                   cursorPos,
		                   padding,
		                   margin,
		                   true,
		                   drawLocationFlag,
		                   layoutType,
		                   widthResizeType, heightResizeType,
		                   itemFlags),
		        m_flags(flags){
		}

		~HBSideBar() {
		}

		void render() override {
			RectWidget::render();
		}

private:
		HBSideBarFlags m_flags    = 0;
	};
}// namespace HBUI


#endif//IMGUI_HBSIDEBAR_H
