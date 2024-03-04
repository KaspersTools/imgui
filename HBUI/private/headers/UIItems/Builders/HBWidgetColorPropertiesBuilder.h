//
// Created by Kasper de Bruin on 04/03/2024.
//

#ifndef IMGUI_HBWIDGETCOLORPROPERTIESBUILDER_H
#define IMGUI_HBWIDGETCOLORPROPERTIESBUILDER_H

#include "UIItems/Properties/HBWidgetColorProperties.h"

namespace HBUI {

	namespace Builder {

		class HBWidgetColorPropertiesBuilder {

	private:
			HBUI::Properties::WidgetColorProperties *m_ColorProperties = new HBUI::Properties::WidgetColorProperties();

	public:
			HBWidgetColorPropertiesBuilder() = default;

			HBWidgetColorPropertiesBuilder &setBackgroundColor(const ImColor &color) {
				m_ColorProperties->m_BackgroundColor = color;
				return *this;
			}

			HBWidgetColorPropertiesBuilder &setTextColor(const ImColor &color) {
				m_ColorProperties->m_TextColor = color;
				return *this;
			}

			HBWidgetColorPropertiesBuilder &setBorderColor(const ImColor &color) {
				m_ColorProperties->m_BorderColor = color;
				return *this;
			}

			HBWidgetColorPropertiesBuilder &setHoverColor(const ImColor &color) {
				m_ColorProperties->m_HoverColor = color;
				return *this;
			}

			HBWidgetColorPropertiesBuilder &setWithBackground(const bool withBackground) {
				m_ColorProperties->m_WithBackground = withBackground;
				return *this;
			}

			HBWidgetColorPropertiesBuilder &setWithBorder(const bool withBorder) {
				m_ColorProperties->m_WithBorder = withBorder;
				return *this;
			};

			HBWidgetColorPropertiesBuilder &setDrawLocation(const HBDrawFlags_ drawLocation) {
				m_ColorProperties->m_DrawLocation = drawLocation;
				return *this;
			}

			/**
			 * @brief sets the color properties based on the imgui style
			 * @param type 	the type of the widget
			 * @param drawLocation	the draw location of the widget (default is HBDrawLocation_None), set this to some other value to overwrite
			 * the already set draw location.
			 * @param flags 	the flags for the color properties HBColorPropertiesFlags_Default (default is
			 * HBColorPropertiesFlags_Default), set this to some other
			 * value to overwrite the already set flags.
			 * @param backgroundColor 	the background color of the widget. ImColor(-1, -1, -1, -1) is the default value, if this is set the imgui
			 * value for this type will be used.
			 * @param borderColor 	the border color. ImColor(-1, -1, -1, -1) is the default value, if this is set the imgui value for this type
			 * will be used.
			 * @param hoverColor 	the hover color. ImColor(-1, -1, -1, -1) is the default value, if this is set the imgui value for this type
			 * will be used.
			 * @return HBWidgetColorPropertiesBuilder& 	the builder
			 */
			HBWidgetColorPropertiesBuilder &fromImGuiStyle(HBUIType_ type,
			                                                  const HBDrawFlags_ drawLocation    = HBDrawFlags_None,
			                                                  const HBColorPropertiesFlags flags = HBColorPropertiesFlags_Default,
			                                                  const ImColor &backgroundColor     = ImColor(-1, -1, -1, -1),
			                                                  const ImColor &borderColor         = ImColor(-1, -1, -1, -1),
			                                                  const ImColor &hoverColor          = ImColor(-1, -1, -1, -1));

			HBUI::Properties::WidgetColorProperties *build();
		};
	}// namespace Builder
}// namespace HBUI

#endif//IMGUI_HBWIDGETCOLORPROPERTIESBUILDER_H
