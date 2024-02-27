//
// Created by Kasper de Bruin on 26/02/2024.
//

#ifndef IMGUI_HBBUTTON_H
#define IMGUI_HBBUTTON_H
namespace HBUI {
	class HBButton : public RectWidget {
public:
		HBButton(
		    const ImGuiID &id, const std::string &label,
		    const ImVec2 &localPositionOffset,
		    const ImVec2 &size,
		    const ImVec2& cursorPos,
		    const ImVec4 &padding,
		    const HBDrawLocation drawLocationFlag) : RectWidget(id, label,
		                                                        HBUIType_SideBarButton,    //background
		                                                        localPositionOffset,
		                                                        size,
		                                                        cursorPos,
		                                                        padding,
		                                                        true,
		                                                        drawLocationFlag,
		                                                        HBLayoutType_::HBLayoutType_Horizontal,
		                                                        HBWidgetResizeType_ScaleToChildren, HBWidgetResizeType_ScaleToChildren) {
		}

		ImColor getBackgroundColor(const bool useHBUIColor = false) const override {
			IM_ASSERT(!useHBUIColor && "Not implemented");
			if (isActive()) {
				return ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
			}
			if (isHovered()) {
				return ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
			}

			return ImGui::GetStyleColorVec4(ImGuiCol_Button);
		}

		ImColor getTextColor(const bool useHBUIColor = false) const override {
			IM_ASSERT(!useHBUIColor && "Not implemented");
			return ImGui::GetStyleColorVec4(ImGuiCol_Text);
		}

		ImColor getBorderColor(const bool useHBUIColor = false) const override {
			IM_ASSERT(!useHBUIColor && "Not implemented");
			return ImGui::GetStyleColorVec4(ImGuiCol_Border);
		}

public:
		bool isActive() const {
			return m_isActive;
		}


private:
		bool m_isActive = false;
	};

}// namespace HBUI
#endif//IMGUI_HBBUTTON_H
