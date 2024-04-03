//
// Created by Kasper de bruin on 07/03/2024.
//

#include "HBTaskBar.h"

#include "HBUI/HBWindow.h"
namespace HBUI::Bars {
  bool HBTaskBar::beforeBegin() {
    auto *parentWindow = reinterpret_cast<Windows::HBWindow *>(getParent());
    IM_ASSERT(parentWindow != nullptr && "TaskBar must be a child of a window");
    IM_ASSERT(parentWindow->windowFlagSet(HBUIWindowFlags_HasTaskBar) && "TaskBar must be a child of a window with the TaskBar flag set");
    return true;
  }

  bool HBTaskBar::afterEnd() {
    auto *parentWindow = reinterpret_cast<Windows::HBWindow *>(getParent());
    parentWindow->afterTaskBarEnd(this);
    return true;
  }

}// namespace HBUI::Bars
