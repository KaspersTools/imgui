//
// Created by Kasper de bruin on 11/03/2024.
//

#ifndef IMGUI_HBWIDGETDEBUGGER_H
#define IMGUI_HBWIDGETDEBUGGER_H
#include <map>

namespace HBUI {
  class HBIWidget;
  namespace Debuggers {
    struct widgetDebugData;
    class HBWidgetDebugger {
    public:
      HBWidgetDebugger() = default;
      ~HBWidgetDebugger() = default;
      void draw(bool* p_open);


      template<typename T>
      void addWidget(T* widget, ImGuiID parentID);
    private:
      void addToParent(ImGuiID parentID, const widgetDebugData &data);
      void drawWidgetData(const widgetDebugData& data);

    private:
      std::map<ImGuiID, widgetDebugData> m_Widgets;
    };

  }// namespace Debuggers
}// namespace HBUI

#endif//IMGUI_HBWIDGETDEBUGGER_H
