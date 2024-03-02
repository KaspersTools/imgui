//
// Created by Kasper de Bruin on 29/02/2024.
//

#ifndef IMGUI_HBDOCKPANEL_H
#define IMGUI_HBDOCKPANEL_H

namespace HBUI {
	class HBDockPanel : public RectWidget {
public:
		HBDockPanel(
		    const std::string &label,
		    const ImVec2 size,
		    const WidgetDrawData &drawData,
		    const WidgetSizingData &sizingData,
		    const WidgetLayoutData &layoutData,
		    const HBItemFlags itemFlags) : RectWidget(label,
		                                              HBUIType_DockPanel,
		                                              drawData,
		                                              sizingData,
		                                              layoutData,
		                                              itemFlags) {
		}

		~HBDockPanel() {
		}

		void render() override {
			bool isVisible = getColorProperties().m_IsVisible;
			ImGui::SetNextWindowSize({
			    getXSize(),
			    getYSize()
			});
			ImGui::SetNextWindowPos(getScreenPosMin());

			ImGui::Begin(m_Label.c_str(), &isVisible,
			             ImGuiWindowFlags_NoCollapse |
			                 ImGuiWindowFlags_NoResize |
			                 ImGuiWindowFlags_NoMove |
			                 ImGuiWindowFlags_NoTitleBar |
			                 ImGuiWindowFlags_NoBringToFrontOnFocus |
			                 ImGuiWindowFlags_NoNavFocus |
			                 ImGuiWindowFlags_NoNav);

			ImGui::DockSpace(ImGui::GetID(m_Label.c_str()),
			                 {RectWidget::getXSize(), RectWidget::getYSize()},
			                 ImGuiDockNodeFlags_PassthruCentralNode);

			ImGui::End();
		}
	};
}// namespace HBUI
#endif//IMGUI_HBDOCKSPACE_H
