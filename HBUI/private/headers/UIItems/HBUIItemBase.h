//
// Created by Kasper de Bruin on 21/02/2024.
//

// clang-format off

#ifndef IMGUI_HBUIITEMBASE_H
#define IMGUI_HBUIITEMBASE_H

#include <HBUI/HBUI.h>
#include <UIItems/Properties/HBWidgetColorProperties.h>
#include <UIItems/Properties/HBWidgetLayoutProperties.h>

#include <vector>

// clang-format on

namespace HBUI {
	class HBItemBase;

	//todo: maybe move this just to HBContext
	class HBWidgetManager {
public:
		HBWidgetManager(const ImVec2 &defaultCursorPosStart = {0, 0},
		                HBLayoutType_ defaultLayoutType     = HBLayoutType_::HBLayoutType_Horizontal) {
			s_defaultCursorPos  = defaultCursorPosStart;
			s_defaultLayoutType = defaultLayoutType;
			reset();
		}

		void startFrame() {
			reset();
		}
		void endFrame() {
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
		HBItemBase() = default;
		HBItemBase(const ImGuiID id, const HBUIType_ type)
		    : m_ID(id),
		      m_Type(type) {
		}

		HBItemBase(const ImGuiID id, const std::string &label, const HBUIType_ type)
		    : m_ID(id),
		      m_Type(type),
		      m_Label(label) {
		}

		HBItemBase(const std::string &label,
		           const HBUIType_ type)
		    : m_ID(ImGui::GetID(label.c_str())),
		      m_Type(type),
		      m_Label(label) {
		}

		HBItemBase(
		    const ImGuiID Id,
		    const std::string &Label,
		    const HBUIType_ Type,
		    const HBUI::Properties::WidgetColorProperties *ColorProperties,
		    const HBUI::Properties::WidgetLayoutProperties *LayoutProperties,
		    HBItemBase *Parent,
		    const std::vector<HBItemBase *> Children);


public:
		virtual void addChild(HBItemBase *child);
		virtual void draw(){};

public:
		ImDrawList *getDrawList() const {
			switch (m_ColorProperties->getDrawLocation()) {
				case HBDrawFlags_None:
					IM_ASSERT(false && "Invalid draw location");
					return nullptr;
				case HBDrawFlags_OnParent:
					IM_ASSERT(cp_Parent != nullptr && "Parent is nullptr");
					return cp_Parent->getDrawList();
				case HBDrawFlags_MainImGuiWindow:
					IM_ASSERT(getMainImGuiWindow() != nullptr && "Main ImGui window is nullptr");
					IM_ASSERT(getMainImGuiWindow()->DrawList != nullptr && "Main ImGui window is nullptr");
					return getMainImGuiWindow()->DrawList;
				default:
					IM_ASSERT(false && "Invalid draw location");
					return nullptr;
			}
		}

		virtual ImVec2 getCursorPos() const;

private:
		ImGuiID m_ID     = -1;
		HBUIType_ m_Type = HBUIType_None;

public:
		ImGuiID getId() const;
		HBUIType_ getType() const;
		const std::string &getLabel() const;
		Properties::WidgetColorProperties *getColorProperties() const;
		Properties::WidgetLayoutProperties *getLayoutProperties() const;
		HBItemBase *getParent() const;
		const std::vector<HBItemBase *> &getChildren() const;

private:
		std::string m_Label = "unnamed";

		HBUI::Properties::WidgetColorProperties *m_ColorProperties   = nullptr;
		HBUI::Properties::WidgetLayoutProperties *m_LayoutProperties = nullptr;

		HBItemBase *cp_Parent = nullptr;

		std::vector<HBItemBase *> pv_Children = {};
	};

}// namespace HBUI
#endif//IMGUI_HBUIITEMBASE_H
