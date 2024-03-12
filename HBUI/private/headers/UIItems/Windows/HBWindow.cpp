//
// Created by Kasper de bruin on 08/03/2024.
//

#include "HBWindow.h"
#include <UIItems/Bars/HBTaskBar.h>

namespace HBUI {
  namespace Windows {

    void HBWindow::afterTaskBarEnd(Bars::HBTaskBar *taskBar) {
      IM_ASSERT(taskBar != nullptr && "Task bar is nullptr");
      IM_ASSERT(taskBar->isEnded() && "Task bar is not ended");
      IM_ASSERT(m_Flags & HBUIWindowFlags_HasTaskBar && "Window does not have a task bar");

      if (m_Flags & HBUIWindowFlags_HasDock) {
        ImGui::DockSpace(ImGui::GetID(std::string(getLabel() + "DockSpace").c_str()));
        m_ScreenPos = ImGui::GetWindowPos();
        ImGui::End();
        drawBase();
      }
    }

    ImVec2 HBWindow::getScreenPos() {
      if(windowFlagSet(HBUIWindowFlags_MainWindow)) {
        m_ScreenPos = HBUI::getNativeWindowPos();
      }else{
        if(getPos() == ImVec2(0, 0)) {
          return m_ScreenPos;
        }

        m_ScreenPos =  getParent()->getScreenPos() + getPos();
      }
      return m_ScreenPos;
    }
  }// namespace Windows
}// namespace HBUI