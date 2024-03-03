//
// Created by Kasper de Bruin on 21/02/2024.
//

#ifndef IMGUI_HBUIITEMBASE_H
#define IMGUI_HBUIITEMBASE_H


#include <vector>

// clang-format off

#include <HBUI/HBUI.h>
#include <Utils/HBIUpdatable.h>
#include "Helpers/HBUIHelpers.h"
// clang-format on

namespace HBUI {
	class HBItemBase;

	//todo: maybe move this just to HBContext
	class HBWidgetManager{
public:
		HBWidgetManager(const ImVec2 &defaultCursorPosStart = {0, 0},
		                HBLayoutType_ defaultLayoutType     = HBLayoutType_::HBLayoutType_Horizontal) {
			s_defaultCursorPos  = defaultCursorPosStart;
			s_defaultLayoutType = defaultLayoutType;
			reset();
		}

		void startFrame()  {
			reset();
		}
		void endFrame()  {
			IM_ASSERT(sp_AppendingWidget == nullptr && "A widget was not ended properly");
		}

		void appendWidget(HBItemBase *widget);
		void endAppendingWidget(const HBUIType_ type);

		void reset() {
			s_cursorPos  = s_defaultCursorPos;
			s_layoutType = s_defaultLayoutType;
		}

		//-------------------------------------------------------------------------
		// [Getters & Setters]
		//-------------------------------------------------------------------------
		HBItemBase *getAppendingWidget() {
			return sp_AppendingWidget;
		}
		ImVec2 getCursorPos() {
			return s_cursorPos;
		}

		HBLayoutType_ getLayoutType() {
			return s_layoutType;
		}
		void setLayoutType(HBLayoutType_ newLayoutType) {
			s_layoutType = newLayoutType;
		}

private:
		HBItemBase *sp_AppendingWidget = nullptr;// The widget that is currently being appended to the window

		ImVec2 s_cursorPos        = ImVec2(0, 0);// Position relative to native window
		ImVec2 s_defaultCursorPos = ImVec2(0, 0);// Position relative to native window

		HBLayoutType_ s_layoutType        = HBLayoutType_::HBLayoutType_Horizontal;// The current layout type
		HBLayoutType_ s_defaultLayoutType = HBLayoutType_::HBLayoutType_Horizontal;// The layout type which is used each frame to reset back to
	};

	class HBItemBase {
public:
		HBItemBase(const std::string &label,
		           const HBUIType_ type,
		           const WidgetLayoutProperties &layoutProperties,
		           HBItemBase *parent = nullptr)
		    : HBItemBase(ImGui::GetID(label.c_str()), type, label, {0, 0}, {0, 0}, layoutProperties, WidgetColorProperties(type, true), parent) {
		}

		HBItemBase(const ImGuiID id,
		           const HBUIType_ type,
		           const WidgetLayoutProperties &layoutProperties,
		           HBItemBase *parent = nullptr)
		    : HBItemBase(id, type, "", {0, 0}, {0, 0}, layoutProperties, WidgetColorProperties(type, true), parent) {
		}

		HBItemBase(const ImGuiID id,
		           const HBUIType_ type,
		           const ImVec2 &screenPosition,
		           const WidgetLayoutProperties &layoutProperties,
		           const WidgetColorProperties &colorProperties,
		           HBItemBase *parent = nullptr)
		    : HBItemBase(id, type, "", screenPosition, {0, 0}, layoutProperties, colorProperties, parent) {
		}

		HBItemBase(const ImGuiID id,
		           const HBUIType_ type,
		           const ImVec2 &screenPosition,
		           const ImVec2 &size,
		           const WidgetLayoutProperties &layoutProperties,
		           HBItemBase *parent = nullptr)
		    : HBItemBase(id, type, "", screenPosition, size, layoutProperties, WidgetColorProperties(type, true), parent) {
		}


		HBItemBase(const ImGuiID id,
		           const HBUIType_ type,
		           const std::string &label,
		           const ImVec2 &screenPosition,
		           const ImVec2 &size,
		           const WidgetLayoutProperties &layoutProperties,
		           const WidgetColorProperties &colorProperties,
		           HBItemBase *parent)
		    : c_ID(id), m_Label(label),
		      m_ScreenPosition(screenPosition),
		      m_Size(size),
		      c_Type(type),
		      m_ColorProperties(colorProperties),
		      m_LayoutProperties(layoutProperties),
		      cp_Parent(parent) {
		}

public:
		HBUIType_ getType() const {
			return c_Type;
		}

		virtual void addChild(HBItemBase *child);
		virtual void draw() = 0;

protected:
		ImDrawList *getDrawList() const {
			switch (m_ColorProperties.c_DrawLocation) {
				case HBDrawFlags_DrawOnParent:
					IM_ASSERT(cp_Parent != nullptr && "Parent is nullptr");
					return cp_Parent->getDrawList();
				case HBDrawFlags_MainImGuiWindowDrawList:
					ImGuiWindow *window = HBUI::getMainImGuiWindow();
					IM_ASSERT(window != nullptr && "Main ImGui window is nullptr");
					return window->DrawList;
			}
		}


		virtual ImVec2 getNextChildPosition() const;

		virtual ImVec2 getCursorPos() const;
		virtual void setCursorPos(const ImVec2 &newPos);
		virtual void getCursorScreenPos() const;

		virtual ImVec2 getContentRegionAvail(const bool includePadding = false) const;
		virtual ImVec2 getContentRegionMax(const bool includePadding = true) const;


		const ImGuiID c_ID     = -1;
		std::string m_Label;
		ImVec2 m_ScreenPosition;
		ImVec2 m_Size;
		const HBUIType_ c_Type = HBUIType_::HBUIType_None;

		HBUI::WidgetColorProperties m_ColorProperties;
		HBUI::WidgetLayoutProperties m_LayoutProperties;

		const HBItemBase *cp_Parent;
		std::vector<HBItemBase *> pv_Children;
	};
}// namespace HBUI
#endif//IMGUI_HBUIITEMBASE_H
