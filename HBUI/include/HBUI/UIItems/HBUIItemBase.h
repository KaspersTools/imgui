//
// Created by Kasper de Bruin on 21/02/2024.
//

#ifndef IMGUI_HBUIITEMBASE_H
#define IMGUI_HBUIITEMBASE_H

class IWidgetBase;

class HBWidgetManager : public HBIUpdateable {
public:
  HBWidgetManager(const ImVec2 &defaultCursorPosStart = {0, 0},
                  HBLayoutType_ defaultLayoutType = HBLayoutType_::HBLayoutType_Horizontal) {
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

  inline static void setLayoutType(HBLayoutType_ newType) {
    s_layoutType = newType;
  }

  inline static const HBLayoutType_ getLayoutType() {
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
  inline static IWidgetBase *sp_AppendingWidget = nullptr;                    // The widget that is currently being appended to the window

  inline static ImVec2 s_cursorPos        = ImVec2(0, 0);                     // Position relative to native window
  inline static ImVec2 s_defaultCursorPos = ImVec2(0, 0);                     // Position relative to native window

  inline static HBLayoutType_ s_layoutType        = HBLayoutType_::HBLayoutType_Horizontal; // The current layout type
  inline static HBLayoutType_ s_defaultLayoutType = HBLayoutType_::HBLayoutType_Horizontal; // The layout type which is used each frame to reset back to
};

struct WidgetDrawData {
  const ImGuiID       cId;
  HBDrawLocation      m_DrawLocation;
  bool                m_WithBackground;
  HBUIType            m_UiType;
  std::string         m_Label;
  bool                m_IsVisible;
  ImVec2              m_Position;
  ImVec2              m_CursorPos;
  HBLayoutType_       m_LayoutType;
  HBWidgetResizeType_ m_XResizeType;
  HBWidgetResizeType_ m_YResizeType;

  explicit WidgetDrawData() = default;

  WidgetDrawData(
      const ImGuiID &id, const HBDrawLocation drawLocationFlag, bool withBackground, HBUIType uiType,
      const std::string &label, bool isVisible, const ImVec2 &position, const ImVec2 &cursorPos,
      HBLayoutType_ layoutType, HBWidgetResizeType_ resizeTypeXAxis, HBWidgetResizeType_ resizeTypeYAxis
  ) :
      cId(id),
      m_DrawLocation(drawLocationFlag),
      m_WithBackground(withBackground),
      m_UiType(uiType),
      m_Label(label),
      m_IsVisible(isVisible),
      m_Position(position),
      m_CursorPos(cursorPos),
      m_LayoutType(layoutType),
      m_XResizeType(resizeTypeXAxis),
      m_YResizeType(resizeTypeYAxis) {

    if(label.empty()){
      m_Label = std::to_string(cId);
    }
  }
};

class IWidgetBase {
protected:
public:
  explicit IWidgetBase(const WidgetDrawData &data) :
      m_drawData(data),
      m_Parent(nullptr),
      m_Children() {

    m_drawData.m_CursorPos.x += 2;
    m_drawData.m_CursorPos.y += 2;
  }

  virtual ~IWidgetBase() {
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
  virtual void addWidth(float toAdd) = 0;

  virtual void addHeight(float toAdd) = 0;

  virtual float calculateTotalWidth() = 0;

  virtual float calculateTotalHeight() = 0;

  //----------------------------------------------------------------------------------------------------------------------
  // [SECTION] Children
  //----------------------------------------------------------------------------------------------------------------------
  void appendChild(IWidgetBase *child) {
    IM_ASSERT(child != nullptr && "Child is nullptr");
    IM_ASSERT(HBWidgetManager::getAppendingWidget() != nullptr && "No widget is being appended");

    m_Children.push_back(child);

    child->m_drawData.m_Position = m_drawData.m_CursorPos;
    child->m_Parent              = HBWidgetManager::getAppendingWidget();

    bool   isNewLine       = child->m_drawData.m_UiType == HBUIType_::HBNEWLINE;
    ImVec2 newCursorPos    = m_drawData.m_CursorPos;

    if (isNewLine) {
      ImVec2 totalChildSize(child->calculateTotalWidth(), child->calculateTotalHeight());
      if (m_drawData.m_LayoutType == HBLayoutType_::HBLayoutType_Horizontal) {
        newCursorPos.x = 2;
        if (m_BiggestChildCurrentLine.y > 0) {
          newCursorPos.y += m_BiggestChildCurrentLine.y + 2;
        } else {//fixme add flag checks
          newCursorPos.y += totalChildSize.y + 2;
        }
      } else if (m_drawData.m_LayoutType == HBLayoutType_::HBLayoutType_Vertical) {
        newCursorPos.y = 2;
        if (m_BiggestChildCurrentLine.x > 0) {
          newCursorPos.x += m_BiggestChildCurrentLine.x + 2;
        } else {//fixme add flag checks
          newCursorPos.x += totalChildSize.x + 2;
        }
      }
      m_BiggestChildCurrentLine = {};
    } else {
      ImVec2 totalChildSize(child->calculateTotalWidth(), child->calculateTotalHeight());
      m_BiggestChildCurrentLine = {
          ImMax(m_BiggestChildCurrentLine.x, totalChildSize.x),
          ImMax(m_BiggestChildCurrentLine.y, totalChildSize.y)
      };

      if (m_drawData.m_LayoutType == HBLayoutType_::HBLayoutType_Horizontal) {
        newCursorPos.x += totalChildSize.x + 2;//fixme: spacing
      } else {
        newCursorPos.y += totalChildSize.y + 2;//fixme: spacing
      }

      ImVec2 selfSize = {calculateTotalWidth(), calculateTotalHeight()};
      ImVec2 diff     = newCursorPos - selfSize;

      if (m_drawData.m_LayoutType == HBLayoutType_::HBLayoutType_Horizontal) {
        if (m_drawData.m_YResizeType == HBWidgetResizeType_ScaleToChildren) {
          addHeight(diff.y + totalChildSize.y + 2);
        }

        if (diff.x > 0) {
          if (m_drawData.m_XResizeType == HBWidgetResizeType_Fixed) {
            newCursorPos.x += 2;
            newCursorPos.y += totalChildSize.y + 2;
            child->m_drawData.m_Position = newCursorPos;
          } else if (m_drawData.m_XResizeType == HBWidgetResizeType_ScaleToChildren) {
            addWidth(diff.x);
          }
        }

      } else if (m_drawData.m_LayoutType == HBLayoutType_::HBLayoutType_Vertical) {
        if (m_drawData.m_XResizeType == HBWidgetResizeType_ScaleToChildren) {
          addWidth(diff.x + totalChildSize.x + 2);
        }

        if (diff.y > 0) {
          if (m_drawData.m_YResizeType == HBWidgetResizeType_Fixed) {
            newCursorPos.x += totalChildSize.x + 2;
            newCursorPos.y += 2;
            child->m_drawData.m_Position = newCursorPos;
          } else if (m_drawData.m_YResizeType == HBWidgetResizeType_ScaleToChildren) {
            addHeight(diff.y);
          }
        }
      }
    }
    m_drawData.m_CursorPos = newCursorPos;
  }

  ImDrawList *getDrawLocation() const {
    switch (m_drawData.m_DrawLocation) {
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
  const ImVec2 &getWidgetScreenPosition() const {
    ImVec2 parentPos = getParentPosition();
    ImVec2 screenPos = parentPos + m_drawData.m_Position;
    return screenPos;
  }

private:
  WidgetDrawData             m_drawData;
  IWidgetBase                *m_Parent = nullptr;        // The parent of the widget
  std::vector<IWidgetBase *> m_Children{};

  ImVec2 m_BiggestChildCurrentLine = {};
public:

//GETTERS
  const std::string &getLabel() const {
    return m_drawData.m_Label;
  }

  const ImGuiID &getId() const {
    return m_drawData.cId;
  }

  const std::vector<IWidgetBase *> &getChildren() const {
    return m_Children;
  }

  const HBUIType getUIType() const {
    return m_drawData.m_UiType;
  }

  const ImVec2 &getParentPosition() const {
    if (m_Parent != nullptr) {
      ImVec2 wsp = m_Parent->getWidgetScreenPosition();
      return wsp;
    } else {
      return HBWidgetManager::getCursorRealScreenPosition();
    }
  }

  const ImVec2 &getLocalPosition() const {
    return m_drawData.m_Position;
  }

  const bool isVisible() const {
    return m_drawData.m_IsVisible;
  }

  const bool withBackground() const {
    return m_drawData.m_WithBackground;
  }

  WidgetDrawData &getDrawData() {
    return m_drawData;
  }

//SETTERS
  void setLocalPosition(const ImVec2 &newPos) {
    m_drawData.m_Position = newPos;
  }

  void setDrawLocationFlags(const HBDrawLocation &newDrawLocation) {
    m_drawData.m_DrawLocation = newDrawLocation;
  }

  void setLayoutType(const HBLayoutType_ &newLayoutType) {
    m_drawData.m_LayoutType = newLayoutType;
  }
};

template<typename SizeType>
class IWidget : public IWidgetBase {
protected:
  IWidget(
      const ImGuiID id,
      const HBDrawLocation drawLocationFlag,
      bool withBackground,
      HBUIType uiType,
      const std::string &label,
      bool isVisible,
      const ImVec2 &position,
      const SizeType &size,
      HBWidgetResizeType_ resizeTypeXAxis,
      HBWidgetResizeType_ resizeTypeYAxis,
      HBLayoutType_ layoutType
  ) : IWidgetBase(
      {
          id,                   //const ImGuiID       cId;
          drawLocationFlag,     //HBDrawLocation      m_DrawLocation;
          withBackground,       //bool                m_WithBackground;
          uiType,               //HBUIType            m_UiType;
          label,                //std::string         m_Label;
          isVisible,            //bool                m_IsVisible;
          position,             //ImVec2              m_Position;
          ImVec2(0, 0),          //ImVec2              m_CursorPos;
          layoutType,           //HBLayoutType        m_LayoutType;
          resizeTypeXAxis,      //HBWidgetResizeType_ m_XResizeType;
          resizeTypeYAxis       //HBWidgetResizeType_ m_YResizeType;
      }
  ),

      size(size) {
  }

  ~IWidget() override {

  }

public:
  const SizeType &getSize() const { return size; }

  void setSize(const SizeType &newSize) { size = newSize; }

  virtual void render() override {
    IWidgetBase::render();
  };

  virtual float calculateTotalWidth() override = 0;

  virtual float calculateTotalHeight() override = 0;

  virtual void addWidth(float toAdd) override = 0;

  virtual void addHeight(float toAdd) override = 0;


protected:
  SizeType size;
};

class RectWidget : public IWidget<ImVec2> {
public:
  RectWidget(
      const ImGuiID &id, const std::string &label,
      const HBUIType uiType,
      const ImVec2 &position, const ImVec2 &size, const bool withBackground,
      const HBDrawLocation drawLocationFlag,
      const HBLayoutType_ &layoutType,
      const HBWidgetResizeType_ resizeTypeXAxis, const HBWidgetResizeType_ resizeTypeYAxis
  ) : IWidget(
      id,
      drawLocationFlag,
      withBackground,
      uiType,
      label,
      true,
      position,
      size,
      resizeTypeXAxis,
      resizeTypeYAxis,
      layoutType
  ) {
  }

  ~RectWidget() override {

  }

public:
  void addWidth(float toAdd) override {
    size.x += toAdd;
  };

  void addHeight(float toAdd) override {
    size.y += toAdd;
  };

  float calculateTotalWidth() override {
    return size.x;
  }

  float calculateTotalHeight() override {
    return size.y;
  }

  virtual void render() override {
    if (!isVisible()) {
      return;
    }
    ImDrawList *drawList = getDrawLocation();
    IM_ASSERT(drawList != nullptr && "Drawlist is nullptr");

    ImVec2 min = getWidgetScreenPosition();
    ImVec2 max = min + size;
    if (withBackground()) {
      drawList->AddRectFilled(min, max, IM_COL32(255, 255, 255, 255));
    } else {
      drawList->AddRect(min, max, IM_COL32(255, 0, 255, 255));
    }

    IWidget::render();
  }
};

#endif //IMGUI_HBUIITEMBASE_H
