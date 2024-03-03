//
// Created by Kasper de Bruin on 01/03/2024.
//

#ifndef IMGUI_HBWIDGETCOLORPROPERTIES_H
#define IMGUI_HBWIDGETCOLORPROPERTIES_H

namespace HBUI {
	struct WidgetColorProperties {

		WidgetColorProperties(HBUIType_ type,
		                      const bool withBackground,
		                      HBDrawLocation_ drawLocation = HBDrawFlags_DrawOnParent,
		                      const bool withBorder = false,
		                      const bool isVisible  = true)
		    :m_WithBackground(withBackground),
		      m_WithBorder(withBorder),
		      m_IsVisible(isVisible),
					c_DrawLocation(drawLocation)
		{
			setColorsBasedOnUIType(type);
		}

		WidgetColorProperties(const ImColor &backgroundColor,
		                      const ImColor &hoverColor,
		                      const ImColor &textColor,
		                      const ImColor &borderColor,

		                      const bool withBackground = true,
		                      const HBDrawLocation_ drawLocation = HBDrawFlags_DrawOnParent,
		                      const bool withBorder = false,
		                      const bool isVisible = true)
		    : m_BackgroundColor(backgroundColor),
		      m_TextColor(textColor),
		      m_BorderColor(borderColor),
		      m_HoverColor(hoverColor),
		      m_WithBackground(withBackground),
		      m_WithBorder(withBorder),
		      m_IsVisible(isVisible),
		      c_DrawLocation(drawLocation){
		}

		ImColor m_BackgroundColor = ImColor(255, 0, 0, 255);
		ImColor m_TextColor       = ImColor(0, 255, 0, 255);
		ImColor m_BorderColor     = ImColor(0, 0, 255, 255);
		ImColor m_HoverColor      = ImColor(140, 140, 140, 255);

		bool m_WithBackground = true;
		bool m_WithBorder     = true;
		bool m_IsVisible      = true;

		const HBDrawLocation_ c_DrawLocation;
		void setColorBasedOnBaseUIType(HBUIType_ type) {}
private:
		void setColorsBasedOnUIType(HBUIType_ type) {
			switch (type) {
				case HBUIType_None:
					break;
				case HBUIType_SideBar:
					break;
				case HBUIType_DockPanel:
					break;
				case HBUIType_SideBarButton:
				case HBUIType_Button:
					break;
			}
		}
		HBDrawLocation_ getDrawLocation() const {
			return m_DrawLocation;
		}


private:
		HBDrawLocation_ m_DrawLocation;
	};
}// namespace HBUI
#endif//IMGUI_HBWIDGETCOLORPROPERTIES_H
