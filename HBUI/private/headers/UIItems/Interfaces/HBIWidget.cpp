//
// Created by Kasper de bruin on 07/03/2024.
//

#include "HBIWidget.h"
#include <ImVk/ImVk.h>

HBUI::HBIWidget::HBIWidget(std::string label,

                           const ImVec2 &localPos,
                           const ImVec2 &size,

                           HBUIType_ type,

                           HBStyleFlags styleFlags,

                           bool canBeOwnParent,

                           HBUI::Properties::WidgetColorProperties *colorProperties,

                           HBDirection_  direction,
                           const ImVec2 &startCursorPos,
                           HBIWidget    *parentWindow)
    : HBIWidget(
          getNewID(label, parentWindow),
          std::move(label),
          localPos,
          size,
          type,
          styleFlags,
          canBeOwnParent,
          colorProperties,
          direction,
          startCursorPos,
          parentWindow) {
}


HBUI::HBIWidget::HBIWidget(
    ImGuiID                                  id,
    std::string                              label,
    const ImVec2                            &localPos,
    const ImVec2                            &size,
    HBUIType_                                type,
    HBStyleFlags                             styleFlags,
    bool                                     canBeOwnParent,
    HBUI::Properties::WidgetColorProperties *colorProperties,
    HBDirection_                             direction,
    const ImVec2                            &startCursorPos,
    HBIWidget                               *parentWindow)
    : c_ID(id),
      c_Type(type),
      c_CanBeOwnParent(canBeOwnParent),
      c_Label(std::move(label)),
      m_InputSize(size),
      m_HasSetPos(localPos != ImVec2(0, 0)),
      m_HasSetSize(size != ImVec2(0, 0)),
      m_Pos(localPos),
      m_StyleFlags(styleFlags),
      m_Direction(direction),
      m_StartCursorPos(startCursorPos),
      p_ColorProperties(colorProperties),
      p_ParentWidget(parentWindow) {

  if (getStyleFlags() & HBStyleFlags_StretchHorizontal) {
    IM_ASSERT(getInputSize().x == 0 && "Stretch horizontal is set but also a size is set on the x axis");
  }
  if (getStyleFlags() & HBStyleFlags_StretchVertical) {
    IM_ASSERT(getInputSize().y == 0 && "Stretch vertical is set but also a size is set on the y axis");
  }

  m_StartCursorPos += HBUI::getStyle().getFramePadding();
  m_CursorPos = m_StartCursorPos;
  IM_ASSERT(p_ColorProperties != nullptr && "Color properties cannot be null");
  if (parentWindow == nullptr) {
    IM_ASSERT(HBUI::getCurrentContext() != nullptr && "HBUI context must be initialized before creating a widget");
    if (HBUI::getCurrentContext()->currentAppingWindow != nullptr) {
      p_ParentWidget = HBUI::getCurrentContext()->currentAppingWindow;
    }
  }
}


ImVec2 HBUI::HBIWidget::appendChild(HBUI::HBIWidget *child) {
  ImVec2 childSize            = child->calculateSize_impl();
  ImVec2 childSizeWithPadding = child->calculateSizeWithPadding();
  childSizeWithPadding.x += 4;
  childSizeWithPadding.y += 4;

  switch (m_Direction) {
    case HBDirection_LeftToRight: {
      m_LeftToRightCursorPos += ImVec2(childSizeWithPadding.x, 0) + ImVec2(4, 0);
      m_CursorPos = m_LeftToRightCursorPos;
      if ((m_TopToBottomCursorPos + ImVec2(0, childSizeWithPadding.y)).y > m_CursorPos.y) {
        m_TopToBottomCursorPos.y += childSizeWithPadding.y;
      }
      break;
    }
    case HBDirection_RightToLeft: {
      m_RightToLeftCursorPos -= ImVec2(childSizeWithPadding.x, 0) - ImVec2(4, 0);
      m_CursorPos = m_RightToLeftCursorPos;
      if ((m_TopToBottomCursorPos + ImVec2(0, childSizeWithPadding.y)).y > m_CursorPos.y) {
        m_TopToBottomCursorPos.y += childSizeWithPadding.y;
      }
      break;
    }
    case HBDirection_BottomToTop:
      m_BottomToTopCursorPos -= ImVec2(0, childSizeWithPadding.y);
      m_CursorPos = m_BottomToTopCursorPos;
      if ((m_LeftToRightCursorPos + ImVec2(childSizeWithPadding.x, 0)).x > m_CursorPos.x) {
        m_LeftToRightCursorPos.x += childSizeWithPadding.x;
      }
      break;
    case HBDirection_TopToBottom:
      m_TopToBottomCursorPos += ImVec2(0, childSizeWithPadding.y);
      m_CursorPos = m_TopToBottomCursorPos;
      if ((m_LeftToRightCursorPos + ImVec2(childSizeWithPadding.x, 0)).x > m_CursorPos.x) {
        m_LeftToRightCursorPos.x += childSizeWithPadding.x + 4;
      }
      break;
  }

  m_SizeWithChildren = ImVec2(
      std::max(m_SizeWithChildren.x, childSizeWithPadding.x),
      std::max(m_SizeWithChildren.y, childSizeWithPadding.y));

  ImGui::SetCursorPos(m_CursorPos);


  if (child->hasSetPos()) {
    return child->getPos();
  }

  return getCursorPos();
}

ImVec2 HBUI::HBIWidget::calculateSizeWithPadding() {
  auto style = HBUI::getStyle();

  ImVec2 pad  = style.getFramePadding();
  ImVec2 size = calculateSize_impl();

  return size + pad;
}

ImVec2 HBUI::HBIWidget::calculateSize_impl() {// NOLINT(*-no-recursion)
  if (m_HasSetSize) {
    return m_InputSize;
  }

  float totalWidth  = 0;
  float totalHeight = 0;

  if (getInputSize().x == 0) {//only has set on the x axis
    float xMin = getPos().x;
    float xMax = getPos().x + m_SizeWithChildren.x;
    totalWidth = xMax - xMin;
  }
  if (getInputSize().y == 0) {//only has set on the y axis
    float yMin  = getPos().y;
    float yMax  = getPos().y + m_SizeWithChildren.y;
    totalHeight = yMax - yMin;
  }

  if (getInputSize().x != 0) {
    totalWidth = getInputSize().x;
  }

  if (getInputSize().y != 0) {
    totalHeight = getInputSize().y;
  }

  if (m_StyleFlags & HBStyleFlags_StretchHorizontal) {
    if (p_ParentWidget == nullptr) {
      totalWidth = HBUI::Backend::getWindowSize().x - m_Pos.x;
    } else {
      totalWidth = p_ParentWidget->calculateSizeWithPadding().x - m_Pos.x;
    }
  }
  if (m_StyleFlags & HBStyleFlags_StretchVertical) {
    if (p_ParentWidget == nullptr) {
      totalHeight = HBUI::Backend::getWindowSize().y - m_Pos.y;
    } else {
      totalWidth = p_ParentWidget->calculateSizeWithPadding().x - m_Pos.x;
    }
  }

  return {totalWidth, totalHeight};
}

ImVec2 HBUI::HBIWidget::getScreenPos() const {

  if (p_ParentWidget == nullptr) {
    return getPos();
  }
  //fixme: this is not correct, this is now called recursively
  ImVec2 screenPos = p_ParentWidget->getScreenPos() + getPos();
  return screenPos;
}

bool HBUI::HBIWidget::begin() {
  IM_ASSERT(!m_HasBegun && "Widget has already been begun");
  if(!canBeOwnParent()){
    IM_ASSERT(p_ParentWidget != nullptr && "Widget cannot be its own parent");
  }
    m_CursorPos            = m_StartCursorPos;
    m_LeftToRightCursorPos = m_StartCursorPos;
    m_RightToLeftCursorPos = m_StartCursorPos;
    m_TopToBottomCursorPos = m_StartCursorPos;
    m_BottomToTopCursorPos = m_StartCursorPos;

    m_HasBegun             = true;
    m_IsEnded              = false;

  if (!hasSetPos()) {
    if (p_ParentWidget != nullptr) {
      ImVec2 pos = p_ParentWidget->getCursorPos();
      setPos(pos);
    } else {
      //cast hbui window to hbiwidget
      ImVec2 cursorPos = HBUI::getMainWindowCursorPos();
      setPos(cursorPos);
    }
  }

  if (!beforeBegin()) {
    return false;
  }


  return afterBegin();
}
bool HBUI::HBIWidget::end() {
  IM_ASSERT(m_HasBegun && "Widget has not been begun");

  if (!beforeEnd()) {
    return false;
  }

  ImVec2 calculateSize = calculateSize_impl();
  if (beforeDraw()) {
    p_ColorProperties->pushColorProperties();
    drawChecks();
    ImVec2 size = calculateSize_impl();
    ImVec2 screenPos = getScreenPos();
    draw(size,screenPos);
    p_ColorProperties->popColorProperties();
    afterDraw();
  } else {
    return false;
  }

  if (p_ParentWidget != nullptr) {
    p_ParentWidget->appendChild(this);
  }

  m_HasBegun  = false;
  m_IsEnded   = true;
  bool result = afterEnd();
  reset();
  return result;
}
