//
// Created by Kasper de Bruin on 21/02/2024.
//

#ifndef IMGUI_HBUIITEMBASE_H
#define IMGUI_HBUIITEMBASE_H

class IWidgetBase;

class HBWidgetManager : public HBIUpdateable {
public:
  HBWidgetManager(const ImVec2 &defaultCursorPosStart = {0, 0},
                  HBLayoutType defaultLayoutType = 0) {
    s_defaultCursorPos  = defaultCursorPosStart;
    s_defaultLayoutType = defaultLayoutType;
    reset();
  }

  static void appendWidget(IWidgetBase *widget);

  static void endAppendingWidget(const HBUIType type);

  inline static IWidgetBase *getAppendingWidget() {
    return sp_AppendingWidget;
  }

  inline static const ImVec2 &getDefaultCursorPos() {
    return s_defaultCursorPos;
  }

  inline static void setDefaultCursorPos(const ImVec2 &newPos) {
    s_defaultCursorPos = newPos;
  }

  inline static const ImVec2 &getCursorPos() {
    return s_cursorPos;
  }

  inline static const ImVec2 &getCursorRealScreenPosition() {
    return HBUI::getViewportPos() + s_cursorPos;
  }

  inline static void setCursorPos(const ImVec2 &newPos) {
    s_cursorPos = newPos;
  }

  inline static void setLayoutType(HBLayoutType newType) {
    s_layoutType = newType;
  }

  inline static const HBLayoutType getLayoutType() {
    return s_layoutType;
  }

  inline static void reset() {
    s_cursorPos  = s_defaultCursorPos;
    s_layoutType = s_defaultLayoutType;
  }

public:
  void startFrame() override {
    reset();
  }

  void endFrame() override {
    IM_ASSERT(sp_AppendingWidget == nullptr && "A widget was not ended properly");
  }

private:
  inline static IWidgetBase *sp_AppendingWidget = nullptr;                  // The widget that is currently being appended to the window

  inline static ImVec2 s_cursorPos        = ImVec2(0, 0);                // Position relative to native window
  inline static ImVec2 s_defaultCursorPos = ImVec2(0, 0);                 // Position relative to native window

  inline static HBLayoutType s_layoutType        = HBLayoutType_::Horizontal; // The current layout type
  inline static HBLayoutType s_defaultLayoutType = HBLayoutType_::Horizontal; // The layout type which is used each frame to reset back to
};

class IWidgetBase {
protected:
  IWidgetBase(const ImGuiID &id, const std::string &label, const HBUIType uiType, IWidgetBase *parent,
              const ImVec2 &position, const bool withBackground, const HBDrawLocation &drawLocation,
              const HBLayoutType &layoutType,
              const HBWidgetResizeType_ resizeTypeXAxis, const HBWidgetResizeType_ resizeTypeYAxis) :
      c_Id(id), m_Label(label), m_UIType(uiType), m_Parent(parent),
      m_Position(position), m_withBackground(withBackground), c_drawLocation(drawLocation),
      m_LayoutType(layoutType),
      m_XResizeType(resizeTypeXAxis), m_YResizeType(resizeTypeYAxis) {
  }

  ~IWidgetBase() {
  }

//----------------------------------------------------------------------------------------------------------------------
// [SECTION] Virtuals
//----------------------------------------------------------------------------------------------------------------------
public:

  virtual void render() {
    for (auto &child: m_Children) {
      child->render();
    }
  }
//----------------------------------------------------------------------------------------------------------------------
// [SECTION] Functions
//----------------------------------------------------------------------------------------------------------------------
public:
  //----------------------------------------------------------------------------------------------------------------------
  // [SECTION] Children
  //----------------------------------------------------------------------------------------------------------------------
  void appendChild(IWidgetBase *child) {
    IM_ASSERT(child != nullptr && "Child is nullptr");
    IM_ASSERT(HBWidgetManager::getAppendingWidget() != nullptr && "No widget is being appended");

    m_Children.push_back(child);
    child->m_Position = this->m_CursorPos;
    child->m_Parent   = HBWidgetManager::getAppendingWidget();

    float itemTotalWidth  = child->calculateTotalWidth();
    float itemTotalHeight = child->calculateTotalHeight();

    if (m_LayoutType == HBLayoutType_::Horizontal) {
      this->m_CursorPos.x += itemTotalWidth;
    } else if (m_LayoutType == HBLayoutType_::Vertical) {
      this->m_CursorPos.y += itemTotalHeight;
    } else {
      IM_ASSERT(false && "Layout type not implemented");
    }

    if (m_CursorPos.x > calculateTotalWidth()) {
      m_CursorPos.x = 0;
      m_CursorPos.y += itemTotalHeight;
    }
    if (m_CursorPos.y > calculateTotalHeight()) {
      m_CursorPos.y = 0;
      m_CursorPos.x += itemTotalWidth;
    }
  }

  ImDrawList *getDrawLocation() const {
    switch (c_drawLocation) {
      case HBDrawFlags_ForegroundDrawList:
        return ImGui::GetForegroundDrawList();
      case HBDrawFlags_DrawOnParent:
        IM_ASSERT(m_Parent != nullptr && "Parent is nullptr");
        return m_Parent->getDrawLocation();//recursive call to parent
      case HBDrawFlags_CreateOwnImGuiWindow:
        return ImGui::GetForegroundDrawList();                          //TODO: Implement this
      case HBDrawFlags_BackgroundDrawList:
        return ImGui::GetBackgroundDrawList();
      case HBDrawFlags_CurrentViewportForegroundDrawList:
        return ImGui::GetForegroundDrawList(ImGui::GetMainViewport());;//TODO: Implement this
      case HBDrawFlags_CurrentViewportBackgroundDrawList:
        return ImGui::GetBackgroundDrawList(ImGui::GetMainViewport());;//TODO: Implement this
      case HBDrawFlags_CurrentWindowDrawList:
        return ImGui::GetWindowDrawList();;                            //TODO: Implement this
    }
  }

//----------------------------------------------------------------------------------------------------------------------
// [SECTION] Calculations
//----------------------------------------------------------------------------------------------------------------------

public:
  virtual float calculateTotalWidth() = 0;//{ return -1; }s
  virtual float calculateTotalHeight() = 0;//{ return -1; }

  const ImVec2 &getWidgetScreenPosition() const {
    ImVec2 parentPos = getParentPosition();
    ImVec2 screenPos = parentPos + m_Position;
    return screenPos;
  }

private:
//COMMON
  const ImGuiID c_Id;           // The id of the widget

  //drawing
  const HBDrawLocation c_drawLocation; // The location (foreground, currentwidget etc..)where the widget is drawn
  bool                 m_withBackground; // If the widget has a background

  HBUIType    m_UIType    = HBNONE; // The type of the widget
  std::string m_Label     = "";     // Label of the widget
  bool        m_IsVisible = true;   // Visibility of the widget

//POSITIONING & Scaling
  ImVec2 m_Position  = {0, 0}; //Position relative to the parent window
  ImVec2 m_CursorPos = {0, 0}; //Position relative to this widget

  HBWidgetResizeType_ m_XResizeType;  // The type of resizing the widget on the x axis
  HBWidgetResizeType_ m_YResizeType;  // The type of resizing the widget on the y axis

  HBLayoutType m_LayoutType = HBLayoutType_::Horizontal; // Default to horizontal is used to determine the cursor position

//children
  IWidgetBase                *m_Parent = nullptr;        // The parent of the widget
  std::vector<IWidgetBase *> m_Children;
public:

//GETTERS
  HBUIType getUIType() const {
    return m_UIType;
  }

  ImVec2 getParentPosition() const {
    if (m_Parent != nullptr) {
      ImVec2 wsp = m_Parent->getWidgetScreenPosition();
      return wsp;
    } else {
      return HBWidgetManager::getCursorRealScreenPosition();
    }
  }

  ImVec2 getLocalPosition() const {
    return m_Position;
  }

  bool isVisible() const {
    return m_IsVisible;
  }

  bool withBackground() const {
    return m_withBackground;
  }

  void setLocalPosition(const ImVec2 &newPos) {
    m_Position = newPos;
  }

  void setLayoutType(HBLayoutType newType) {
    m_LayoutType = newType;
  }

  void setXAxiResizeType(HBWidgetResizeType_ newType) {
    m_XResizeType = newType;
  }

  void setYAxiResizeType(HBWidgetResizeType_ newType) {
    m_YResizeType = newType;
  }

};

template<typename SizeType>
class IWidget : public IWidgetBase {
protected:
  IWidget(
      const ImGuiID &id, const std::string &label,
      const HBUIType uiType, IWidgetBase *parent,
      const ImVec2 &position, const SizeType &size, const bool withBackground,
      const HBDrawLocation drawLocationFlag,
      const HBLayoutType &layoutType,
      const HBWidgetResizeType_ resizeTypeXAxis, const HBWidgetResizeType_ resizeTypeYAxis
  ) : IWidgetBase(
      id, label, uiType, parent, position, withBackground, drawLocationFlag, layoutType, resizeTypeXAxis,
      resizeTypeYAxis) {
//    std::cout << "IWidget::IWidget" << std::endl;
    setSize(size);
  }

public:
  const SizeType &getSize() const { return size; }

  void setSize(const SizeType &newSize) { size = newSize; }

  virtual void render() override {
    IWidgetBase::render();
  };

  virtual float calculateTotalWidth() override = 0;

  virtual float calculateTotalHeight() override = 0;

protected:
  SizeType size;
};

class RectWidget : public IWidget<ImVec2> {
public:
  RectWidget(
      const ImGuiID &id, const std::string &label,
      const HBUIType uiType, IWidgetBase *parent,
      const ImVec2 &position, const ImVec2 &size, const bool withBackground,
      const HBDrawLocation drawLocationFlag,
      const HBLayoutType &layoutType,
      const HBWidgetResizeType_ resizeTypeXAxis, const HBWidgetResizeType_ resizeTypeYAxis
  ) : IWidget(id, label, uiType, parent, position, size, withBackground, drawLocationFlag, layoutType,
              resizeTypeXAxis, resizeTypeYAxis) {
  }

public:
  float calculateTotalWidth() override {
    return size.x;
  }

  float calculateTotalHeight() override {
    return size.y;
  }

  virtual void render() override {
    ImDrawList *drawList = getDrawLocation();
    IM_ASSERT(drawList != nullptr && "Drawlist is nullptr");

    ImVec2 min = getWidgetScreenPosition();
    ImVec2 max = min + size;
    if(withBackground()){
      drawList->AddRectFilled(min, max, IM_COL32(255, 255, 255, 255));
    }else {
      drawList->AddRect(min,       max, IM_COL32(255, 0, 255, 255));
    }

    IWidget::render();
  }
};

#endif //IMGUI_HBUIITEMBASE_H
