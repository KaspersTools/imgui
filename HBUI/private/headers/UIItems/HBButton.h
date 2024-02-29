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
		    const ImVec2 &cursorPos,
		    const ImVec4 &padding,
		    const ImVec4 &margin,
		    const HBDrawLocation drawLocationFlag,
		    ImFont *font,
		    const HBItemFlags itemFlags) : RectWidget(id, label,
		                                              HBUIType_SideBarButton,//background
		                                              localPositionOffset,
		                                              calculateSize(padding, label, font),
		                                              cursorPos,
		                                              padding,
		                                              margin,
		                                              true,
		                                              drawLocationFlag,
		                                              HBLayoutType_::HBLayoutType_Horizontal,
		                                              HBWidgetResizeOptions_ScaleToChildren,
		                                              HBWidgetResizeOptions_ScaleToChildren,
		                                              itemFlags),
		                                   p_Font(font) {
		}

		ImVec2 getTextSize(const std::string &label, ImFont *font) const {
			IM_ASSERT(font != NULL && "Font is nullptr");

			const bool pushFont = ImGui::GetFont() != p_Font;

			if (pushFont) { ImGui::PushFont(p_Font); }
			const ImVec2 label_size = ImGui::CalcTextSize(label.c_str(), NULL, true);
			if (pushFont) { ImGui::PopFont(); }

			return label_size;
		}

		ImVec2 calculateSize(ImVec4 padding = ImVec4(0, 0, 0, 0), const std::string &label = "", ImFont *font = nullptr) {
			if (font == nullptr)
				p_Font = ImGui::GetFont();

			p_Font = font;
			IM_ASSERT(p_Font != NULL && "Font not found/loaded???");


			ImVec2 label_size = getTextSize(label, p_Font);
			ImVec2 size       = ImGui::CalcItemSize({0, 0},
			                                        label_size.x + padding.x + padding.z,
			                                        label_size.y + padding.y + padding.w);

			return size;
		}

		void render() override {
			ImDrawList *drawList = getDrawlist();
			IM_ASSERT(drawList != nullptr && "Drawlist not found");

			RectWidget::render();

			if (getLabel() != "") {
				IM_ASSERT(p_Font != NULL && "Font not found/loaded???");
				const bool pushFont = ImGui::GetFont() != p_Font;
				if (pushFont) { ImGui::PushFont(p_Font); }

				const ImRect bb(getScreenPosMin(), getScreenPosMax());
				ImVec2* textSize = new ImVec2(getTextSize(getLabel(), p_Font));
				ImGui::RenderTextClippedEx(
				    							drawList,
													{bb.Min.x + getPadding().x,
				                   bb.Min.y + getPadding().y},
				                  {bb.Max.x - getPadding().z,
				                   bb.Max.y - getPadding().w},
				                  getLabel().c_str(), NULL, textSize,
				                  {.5,.5}, &bb);

				delete textSize;

				if (pushFont) { ImGui::PopFont(); }
			}
		}

public:
		bool isActive() const {
			return m_isActive;
		}


private:
		bool m_isActive = false;

		ImFont *p_Font = nullptr;
	};

}// namespace HBUI
#endif//IMGUI_HBBUTTON_H
