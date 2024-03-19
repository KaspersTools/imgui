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
        ImVec2 sizeWithChildren = getSizeWithChildren();
        ImVec2 currentScreenPos = ImGui::GetCurrentWindow()->Pos + ImVec2(0, sizeWithChildren.y);

        ImGui::SetCursorScreenPos(currentScreenPos);
        ImGui::DockSpace(ImGui::GetID(std::string(getLabel() + "DockSpace").c_str()));
        m_ScreenPos = ImGui::GetWindowPos();
        ImGui::End();
        ImGui::Begin("test");
      }
    }

    ImVec2 HBWindow::getScreenPos() const {
      if(m_Flags &HBUIWindowFlags_MainWindow){
        return ImGui::GetMainViewport()->Pos - ImVec2(1,1);
      }else{
        return HBIWidget::getScreenPos();
      }
    }
  }// namespace Windows
}// namespace HBUI