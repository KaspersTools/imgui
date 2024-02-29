//
// Created by Kasper de Bruin on 10/02/2024.
//

// clang-format off
#include <imgui.h>

#include <UIItems/HBDefaultItems.h>

#include <codecvt>
#include <locale>

// clang-format on

static bool
    aabb(const ImVec2 &min1, const ImVec2 &max1, const ImVec2 &min2, const ImVec2 &max2) {
	return (min1.x < max2.x && max1.x > min2.x) && (min1.y < max2.y && max1.y > min2.y);
}

namespace HBUI {
	HBUI_API void
	    newLine(float size, HBWidgetResizeOptions_ resizeType) {
	}

	HBUI_API std::string wchar32ToUtf8(const ImWchar &wchar) {
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
		return converter.to_bytes(wchar);
	}

	HBUI_API bool beginSideBar(
	    const std::string &label,
	    const HBSideBarFlags flags,
	    const ImColor &backgroundColor,
	    const ImVec2 &position,
	    const ImVec2 &size,
	    const ImVec4 &padding,
	    const ImVec4 &margin) {

		ImGuiID id = ImGui::GetID(label.c_str());

		///\\\\\\\\\\\\\\\\//id   //bgColor  //flags   //pos  //size //cursorPos  //padding  //margin //label
		return beginSideBar(id, backgroundColor, flags, position, size, ImVec2(0, 0), padding, margin, "label",
		                    ///itemFlags
		                    HBItemFlags_ResizeChildrenToBiggestChild,
		                    ///drawLocationFlag
		                    HBDrawFlags_MainImguiWindowDrawlist);
	}

	HBUI_API bool
	    beginSideBar(const ImGuiID &id,                      //id
	                 const ImColor &backgroundColor,         //backgroundColor
	                 const HBSideBarFlags flags,             //flags
	                 const ImVec2 &position,                 //position
	                 ImVec2 size,                            //size
	                 const ImVec2 &cursorPos,                //cursorPos
	                 const ImVec4 &padding,                  //padding
	                 const ImVec4 &margin,                   //margin
	                 const std::string &label,               //label
	                 const HBItemFlags itemFlags,            //itemFlags
	                 const HBDrawLocation drawLocationFlag) {//drawLocationFlag
		bool isHorizontal = flags & HBSideBarFlags_Horizontal;
		bool isVertical   = flags & HBSideBarFlags_Vertical;

		IM_ASSERT((isHorizontal || isVertical) && "A sidebar must be either horizontal or vertical");
		IM_ASSERT(!(isHorizontal && isVertical) && "A sidebar cannot be both horizontal and vertical");
		HBLayoutType_ layoutType = HBLayoutType_::HBLayoutType_Horizontal;

		if (isVertical) {
			layoutType = HBLayoutType_::HBLayoutType_Vertical;
		}
		HBWidgetResizeOptions_ resizeTypeXAxis = HBWidgetResizeOptions_::HBWidgetResizeOptions_ScaleToChildren;
		HBWidgetResizeOptions_ resizeTypeYAxis = HBWidgetResizeOptions_::HBWidgetResizeOptions_ScaleToChildren;

		if (flags & HBSideBarFlags_FullSize) {
			if (isHorizontal) {//todo: add cursor position if not over vp or foreground drawlist
				size.x = HBUI::getViewportSize().x;
				if (size.y != 0) {
					resizeTypeYAxis = HBWidgetResizeOptions_::HBWidgetResizeOptions_Fixed;
				}
			} else {
				size.y = HBUI::getViewportSize().y;
				if (size.x != 0) {
					resizeTypeXAxis = HBWidgetResizeOptions_::HBWidgetResizeOptions_Fixed;
				}
			}
		}

		HBSideBar *sideBar =
		    new HBSideBar(
		        id,
		        label,
		        flags,
		        backgroundColor,
		        itemFlags,
		        position,
		        size,
		        cursorPos,
		        padding,
		        margin,
		        drawLocationFlag,
		        layoutType,
		        resizeTypeXAxis,
		        resizeTypeYAxis);

		HBWidgetManager::appendWidget(sideBar);
		return true;
	}

	IMGUI_API void
	    endSideBar() {
		HBUI::getCurrentContext()->widgetManager->endAppendingWidget(HBUIType_SideBar);
	}

	//menu items
	IMGUI_API bool
	    sideBarBarButton(const ImGuiID id, const ImWchar label,
	                     const ImVec2 &position, const ImVec2 &size,
	                     const float fontSize, const ImVec2 &cursorPos,
	                     const ImVec4 padding, const ImVec4 margin, ImFont *font) {
		const std::string lblString = HBUI::wchar32ToUtf8(label);
		if (font == nullptr) font = HBUI::getBigFont();
		return sideBarBarButton(id, lblString, position, size, fontSize, cursorPos, padding, margin, font);
	}

	IMGUI_API bool
	    sideBarBarButton(const ImGuiID id, const std::string &label, const ImVec2 &position, const ImVec2 &size,
	                     const float fontSize, const ImVec2 &cursorPos,
	                     const ImVec4 padding, const ImVec4 margin, ImFont *font) {
		if (font == nullptr) {
			font = HBUI::getBigFont();//fixme:  get regular font
		}

		IM_ASSERT(font != nullptr && "Font not found/loaded???");

		HBSideBarButton *button = new HBSideBarButton(id, label,
		                                              position,
		                                              size,
		                                              cursorPos,
		                                              padding,
		                                              margin,
		                                              HBDrawLocation::HBDrawFlags_DrawOnParent,
		                                              font);
		HBWidgetManager::appendWidget(button);
		HBWidgetManager::endAppendingWidget(HBUIType_SideBarButton);
		return true;//fixme: return true if the button is clicked
	}
	//	}
}// namespace HBUI