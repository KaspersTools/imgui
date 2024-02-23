//
// Created by Kasper de Bruin on 21/02/2024.
//
//#include "HBUIItemBase.h"
#include <HBUI/HBUI.h>

void HBWidgetManager::appendWidget(IWidgetBase *widget) {
  if(sp_AppendingWidget != nullptr){
    sp_AppendingWidget->appendChild(widget);
    return;
  }

  IM_ASSERT(sp_AppendingWidget == nullptr && "A widget is already being appended");
  if (widget->getLocalPosition() == ImVec2(0, 0)) {//(0,0)means take the current cursor position
    widget->setLocalPosition(s_cursorPos);
  }
  sp_AppendingWidget = widget;
}

void HBWidgetManager::endAppendingWidget(const HBUIType type) {
  IM_ASSERT(sp_AppendingWidget != nullptr && "No widget is being appended");
  IM_ASSERT(sp_AppendingWidget->getUIType() == type &&
            "The id of the widget being ended does not match the id of the appending widget");

  if (sp_AppendingWidget->isVisible()) {
    sp_AppendingWidget->render();

    if (s_layoutType == HBLayoutType_::HBLayoutType_Horizontal) {
      s_cursorPos.x += sp_AppendingWidget->calculateTotalWidth() + 23; //fixme: add real spacing / padding
    } else {
      s_cursorPos.y += sp_AppendingWidget->calculateTotalHeight() + 2; //fixme: add real spacing / padding
    }
  }

  HBUI::addDebugWidget(sp_AppendingWidget->getLabel(), sp_AppendingWidget);
  sp_AppendingWidget = nullptr;
}
