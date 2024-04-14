//
// Created by Kasper de bruin on 11/03/2024.
//
#include <HBUI/HBUI.h>
#include <magic_enum.hpp>

#include "HBWidgetDebugger.h"
#include <UIItems/Interfaces/HBIWidget.h>

namespace HBUI::Debuggers {

  void HBWidgetDebugger::draw(bool *p_open) {
    ImGui::Begin("Widget Debugger", p_open);

    for (auto &widget: m_Widgets) {
      drawWidgetData(widget.second);
    }

    ImGui::End();
  }

  void HBWidgetDebugger::drawWidgetData(const widgetDebugData &data) {
    const std::string lbl = ("ID: " + std::to_string(data.c_ID) + " | Label: " + data.c_Label + " | Type: " +
                             magic_enum::enum_name(data.c_Type).data());

    if (ImGui::TreeNode((void *) (intptr_t) data.c_ID, "%s", lbl.c_str())) {
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

      ImGui::TreePop();
    }
  }

  template<typename T>
  void HBWidgetDebugger::addWidget(T *widget) {}


  template<>
  void HBWidgetDebugger::addWidget(HBIWidget *widget) {
    widgetDebugData data;

    data.c_ID             = widget->c_ID;
    data.c_Label          = widget->c_Label;
    data.c_Type           = widget->c_Type;
    data.c_CanBeOwnParent = widget->c_CanBeOwnParent;

    data.m_HasBegun = widget->m_HasBegun;
    data.m_IsEnded  = widget->m_IsEnded;

    data.m_StyleFlags = widget->m_StyleFlags;

    data.c_InputSize = widget->m_InputSize;

    data.m_Pos       = widget->m_Pos;
    data.m_ScreenPos = widget->getScreenPos();

    data.m_Direction      = widget->m_Direction;
    data.m_StartCursorPos = widget->m_StartCursorPos;
    data.m_CursorPos      = widget->m_CursorPos;

    data.m_TopToBottomCursorPos = widget->m_TopToBottomCursorPos;
    data.m_LeftToRightCursorPos = widget->m_LeftToRightCursorPos;
    data.m_BottomToTopCursorPos = widget->m_BottomToTopCursorPos;
    data.m_RightToLeftCursorPos = widget->m_RightToLeftCursorPos;

    data.m_ParentID = widget->p_ParentWidget == nullptr ? 0 : widget->p_ParentWidget->c_ID;

    m_Widgets[widget->c_ID] = data;
  }

}// namespace HBUI::Debuggers