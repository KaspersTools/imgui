//
// Created by Kasper de bruin on 11/03/2024.
//

#ifndef IMGUI_HBWIDGETDEBUGGER_H
#define IMGUI_HBWIDGETDEBUGGER_H

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
      void addWidget(T* widget);
    private:
      static void drawWidgetData(const widgetDebugData& data);
    private:
      std::unordered_map<ImGuiID, widgetDebugData> m_Widgets;
    };

  }// namespace Debuggers
}// namespace HBUI

#endif//IMGUI_HBWIDGETDEBUGGER_H
