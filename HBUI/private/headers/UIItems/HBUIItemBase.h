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
	namespace Builder{
		class HBItemBuilder;
	}
	class HBItemBase;

	//todo: maybe move this just to HBContext
	class HBWidgetManager {
public:
		explicit HBWidgetManager(const ImVec2 &defaultCursorPosStart = {0, 0},
		                HBLayoutType_ defaultLayoutType     = HBLayoutType_::HBLayoutType_Horizontal) {
			s_defaultCursorPos  = defaultCursorPosStart;
			s_defaultLayoutType = defaultLayoutType;
			reset();
		}

		[[maybe_unused]]void startFrame() {
			reset();
		}
		[[maybe_unused]]void endFrame() {
			IM_ASSERT(sp_AppendingWidget == nullptr && "A widget was not ended properly");
		}

		[[maybe_unused]]void appendWidget(HBItemBase *widget);
		[[maybe_unused]]void endAppendingWidget(HBUIType_ type);

		[[maybe_unused]]void reset() {
			s_cursorPos  = s_defaultCursorPos;
			s_layoutType = s_defaultLayoutType;
		}

		//-------------------------------------------------------------------------
		// [Getters & Setters]
		//-------------------------------------------------------------------------
		[[maybe_unused]]HBItemBase *getAppendingWidget() {
			return sp_AppendingWidget;
		}
		[[maybe_unused]]ImVec2 getCursorPos() {
			return s_cursorPos;
		}

		[[maybe_unused]]HBLayoutType_ getLayoutType() {
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
		friend class Builder::HBItemBuilder;
public:
		HBItemBase() = default;
		HBItemBase(const ImGuiID id, const HBUIType_ type)
		    : m_ID(id),
		      m_Type(type) {
		}

		HBItemBase(const std::string &label,
		           const HBUIType_ type)
		    : m_ID(ImGui::GetID(label.c_str())),
		      m_Type(type),
		      m_Label(label) {
		}

		HBItemBase(const ImGuiID id, const std::string &label, const HBUIType_ type)
		    : m_ID(id),
		      m_Type(type),
		      m_Label(label) {
		}

		HBItemBase(
		    const ImGuiID Id,
		    const std::string &Label,
		    const HBUIType_ Type,
		    HBUI::Properties::WidgetColorProperties *ColorProperties,
		    HBUI::Properties::WidgetLayoutProperties *LayoutProperties,
		    HBItemBase *Parent,
		    const std::vector<HBItemBase *>& Children);


public:
		virtual void addChild(HBItemBase *child);
		virtual void draw(){};

public:
		ImDrawList *getDrawList() const {
			switch (m_ColorProperties->getDrawLocation()) {
				case HBDrawFlags_None:
					IM_ASSERT(false && "Invalid draw location");
				case HBDrawFlags_OnParent:
					IM_ASSERT(p_Parent != nullptr && "Parent is nullptr");
					return p_Parent->getDrawList();
				case HBDrawFlags_MainImGuiWindow:
					IM_ASSERT(getMainImGuiWindow() != nullptr && "Main ImGui window is nullptr");
					IM_ASSERT(getMainImGuiWindow()->DrawList != nullptr && "Main ImGui window is nullptr");
					return getMainImGuiWindow()->DrawList;
				default:
					IM_ASSERT(false && "Invalid draw location");
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

		Properties::WidgetColorProperties  &getColorProperties() const;
		Properties::WidgetLayoutProperties &getLayoutProperties() const;

		HBItemBase *getParent() const;

		const std::vector<HBItemBase *> &getChildren() const;

private:
		std::string m_Label = "unnamed";

		HBUI::Properties::WidgetColorProperties *m_ColorProperties   = nullptr;
		HBUI::Properties::WidgetLayoutProperties *m_LayoutProperties = nullptr;

		HBItemBase *p_Parent = nullptr;

		std::vector<HBItemBase *> m_Children = {};
	};

}// namespace HBUI
#endif//IMGUI_HBUIITEMBASE_H
