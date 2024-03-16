//
// Created by Kasper de bruin on 11/03/2024.
//
#include <HBUI/HBUI.h>
#include <magic_enum.hpp>

namespace HBUI {
  namespace Debuggers {
  };// namespace Debuggers
};  // namespace HBUI

#include "HBWidgetDebugger.h"
#include <UIItems/Interfaces/HBIWidget.h>

namespace HBUI::Debuggers {
  void widgetDebugData::addChild(ImGuiID parentID, const HBUI::Debuggers::widgetDebugData &child) {
    m_Children[parentID] = child;
  }

  void HBWidgetDebugger::draw(bool *p_open) {
    ImGui::Begin("Widget Debugger", p_open);

    for (auto &widget: m_Widgets) {
      drawWidgetData(widget.second);
    }

    ImGui::End();
    m_Widgets.clear();
  }

  void HBWidgetDebugger::drawWidgetData(const widgetDebugData &data) {
    if (ImGui::TreeNode((void *) (intptr_t) data.c_ID, "ID: %d", data.c_ID)) {
      ImGui::Text("ID: %d", data.c_ID);
      ImGui::Text("Parent ID: %d", data.m_ParentID);

      ImGui::Text("Type: %s", magic_enum::enum_name(data.c_Type).data());
      ImGui::Text("Label: %s", data.c_Label.c_str());
      ImGui::Text("Can be own parent: %s", data.c_CanBeOwnParent ? "true" : "false");

      ImGui::Text("Input size: (%f, %f)", data.c_InputSize.x, data.c_InputSize.y);
      ImGui::Text("Position: (%f, %f)", data.m_Pos.x, data.m_Pos.y);
      ImGui::Text("Screen position: (%f, %f)", data.m_ScreenPos.x, data.m_ScreenPos.y);
      ImGui::Text("Position: (%f, %f)", data.m_Pos.x, data.m_Pos.y);
      ImGui::Text("Direction: %s", magic_enum::enum_name(data.m_Direction).data());

      ImGui::Text("Start cursor position: (%f, %f)", data.m_StartCursorPos.x, data.m_StartCursorPos.y);
      ImGui::Text("Cursor position: (%f, %f)", data.m_CursorPos.x, data.m_CursorPos.y);
      ImGui::Text("Bottom to top cursor position: (%f, %f)", data.m_BottomToTopCursorPos.x, data.m_BottomToTopCursorPos.y);
      ImGui::Text("Top to bottom cursor position: (%f, %f)", data.m_TopToBottomCursorPos.x, data.m_TopToBottomCursorPos.y);
      ImGui::Text("Left to right cursor position: (%f, %f)", data.m_LeftToRightCursorPos.x, data.m_LeftToRightCursorPos.y);
      ImGui::Text("Right to left cursor position: (%f, %f)", data.m_RightToLeftCursorPos.x, data.m_RightToLeftCursorPos.y);

      ImGui::Text("Has begun: %s", data.m_HasBegun ? "true" : "false");
      ImGui::Text("Is ended: %s", data.m_IsEnded ? "true" : "false");

      for (auto &styleFlag: magic_enum::enum_values<HBStyleFlags_>()) {
        if (data.m_StyleFlags & styleFlag) {
          ImGui::Text("Style flag: %s", magic_enum::enum_name(styleFlag).data());
        }
      }

      for (auto &child: data.m_Children) {
        drawWidgetData(child.second);
      }

      ImGui::TreePop();
    }
  }

  void HBWidgetDebugger::addToParent(ImGuiID parentID, const HBUI::Debuggers::widgetDebugData &data) {
    auto it = m_Widgets.find(parentID);
    if (it != m_Widgets.end()) {
      it->second.addChild(parentID, data);
    } else {
      m_Widgets[parentID] = widgetDebugData();
    }
  }

  template<typename T>
  void HBWidgetDebugger::addWidget(T *widget, ImGuiID parentID) {}


  template<>
  void HBWidgetDebugger::addWidget(HBIWidget *widget, ImGuiID parentID) {
    widgetDebugData data;

    data.c_Type                 = widget->c_Type;
    data.c_ID                   = widget->c_ID;
    data.c_CanBeOwnParent       = widget->c_CanBeOwnParent;
    data.c_InputSize            = widget->m_InputSize;
    data.c_Label                = widget->c_Label;
    data.m_Pos                  = widget->m_Pos;
    data.m_StartCursorPos       = widget->m_StartCursorPos;
    data.m_CursorPos            = widget->m_CursorPos;
    data.m_Direction            = widget->m_Direction;
    data.m_BottomToTopCursorPos = widget->m_BottomToTopCursorPos;
    data.m_TopToBottomCursorPos = widget->m_TopToBottomCursorPos;
    data.m_LeftToRightCursorPos = widget->m_LeftToRightCursorPos;
    data.m_RightToLeftCursorPos = widget->m_RightToLeftCursorPos;
    data.m_HasBegun             = widget->m_HasBegun;
    data.m_IsEnded              = widget->m_IsEnded;
    data.m_StyleFlags           = widget->m_StyleFlags;
    data.m_ScreenPos            = widget->getScreenPos();
    data.m_ParentID             = parentID;

    m_Widgets[widget->c_ID] = data;
  }

}// namespace HBUI::Debuggers