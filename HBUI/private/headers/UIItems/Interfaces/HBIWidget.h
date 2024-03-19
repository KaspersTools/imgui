//
// Created by Kasper de bruin on 07/03/2024.
//

#ifndef IMGUI_HBIWIDGET_H
#define IMGUI_HBIWIDGET_H

#include <HBUI/HBUI.h>

#include <UIItems/Builders/HBWidgetColorPropertiesBuilder.h>
#include <UIItems/Properties/HBWidgetColorProperties.h>

#include <utility>

namespace HBUI::Debuggers {
  class HBWidgetDebugger;
}// namespace HBUI::Debuggers

namespace HBUI {

  class HBIWidget {
    friend class Debuggers::HBWidgetDebugger;
    friend class ::HBContext;


  public:
    HBIWidget(ImGuiID                            id,
              std::string                        label,
              const ImVec2                      &localPos,
              const ImVec2                      &size,
              HBUIType_                          type,
              HBStyleFlags                       styleFlags,
              bool                               canBeOwnParent,
              Properties::WidgetColorProperties *colorProperties,

              HBDirection_  direction,
              const ImVec2 &startCursorPos = ImVec2(0, 0),
              HBIWidget    *parentWidget   = nullptr);


    HBIWidget(std::string                        label,
              const ImVec2                      &size,
              const ImVec2                      &localPos,
              HBUIType_                          type,
              HBStyleFlags                       styleFlags,
              bool                               canBeOwnParent,
              Properties::WidgetColorProperties *colorProperties,

              HBDirection_  direction,
              const ImVec2 &startCursorPos = ImVec2(0, 0),
              HBIWidget    *parentWidget   = nullptr);

    virtual ~HBIWidget() {
      delete p_ColorProperties;
    }

  private:
    bool drawChecks() {
      if (!canBeOwnParent()) {
        IM_ASSERT(p_ParentWidget != nullptr && "Parent widget is nullptr");
        return false;
      }

      return true;
    }

  protected:
    bool begin();
    bool end();

  public:
    [[nodiscard]] const std::string &getLabel() const {
      return c_Label;
    }

    [[nodiscard]] const ImVec2 &getPos() const {
      return m_Pos;
    }

    [[nodiscard]] const ImVec2 &getCursorPos() const {
      return getCursorPos(m_Direction);
    }

    [[nodiscard]] const ImVec2 &getCursorPos(HBDirection_ dir) const {
      if (dir == HBDirection_BottomToTop)
        return m_BottomToTopCursorPos;
      if (dir == HBDirection_TopToBottom)
        return m_TopToBottomCursorPos;
      if (dir == HBDirection_LeftToRight)
        return m_LeftToRightCursorPos;
      if (dir == HBDirection_RightToLeft)
        return m_RightToLeftCursorPos;

      return m_CursorPos;
      IM_ASSERT(false && "Direction not set");
    }

    [[nodiscard]] const HBUIType_ &getType() const {
      return c_Type;
    }

    [[nodiscard]] const ImGuiID &getID() const {
      return c_ID;
    }

    [[nodiscard]] const bool &canBeOwnParent() const {
      return c_CanBeOwnParent;
    }

    [[nodiscard]] ImDrawList *getDrawList() {
      if (p_DrawList != nullptr) {
        return p_DrawList;
      }
      if (!canBeOwnParent()) {
        IM_ASSERT(p_ParentWidget != nullptr && "Parent widget is nullptr");
        return p_ParentWidget->getDrawList();
      } else {
        p_DrawList = ImGui::GetWindowDrawList();
      }
      return p_DrawList;
    }

    [[nodiscard]] const ImVec2 getInputSize() const {
      return m_InputSize;
    }

    [[nodiscard]] HBIWidget *getParent() const {
      return p_ParentWidget;
    }

    [[nodiscard]] HBStyleFlags getStyleFlags() const {
      return m_StyleFlags;
    }

    [[nodiscard]] const bool &hasSetPos() const {
      return m_HasSetPos;
    }

    [[nodiscard]] bool hasBegun() const {
      return m_HasBegun;
    }

    [[nodiscard]] bool isEnded() const {
      return m_IsEnded;
    }
    [[nodiscard]] ImVec2 calculateSizeWithPadding();

  private:
    [[nodiscard]] virtual ImVec2 calculateSize_impl();

  public:
#pragma clang diagnostic push
#pragma ide diagnostic   ignored "NullDereference"
#pragma ide diagnostic   ignored "misc-no-recursion"
    [[nodiscard]] virtual ImVec2 getScreenPos() const;
#pragma clang diagnostic pop

  protected:
    virtual bool beforeBegin() { return true; };
    virtual bool afterBegin() { return true; };

    virtual bool beforeDraw() { return true; };
    virtual void draw(const ImVec2 &size, const ImVec2 &screenPos) = 0;
    virtual void afterDraw(){};

    virtual bool beforeEnd() { return true; };
    virtual bool afterEnd() {return true;};

  protected:
    virtual ImVec2 appendChild(HBIWidget *child);

    void setDirection(const HBDirection_ &direction) {
      m_Direction = direction;
    }
    void setDrawList(ImDrawList *drawList) {
      p_DrawList = drawList;
    }
    void setPos(const ImVec2 &pos) {
      m_Pos = pos;
    }
    void setCursorPos(const ImVec2 &pos) {
      m_CursorPos = pos;
      ImGui::SetCursorPos(m_CursorPos);
    }
    void setParent(HBIWidget *parent) {
      p_ParentWidget = parent;
    }

  private:
    void reset() {
      m_Pos       = ImVec2(0, 0);//Local Position of the widget
      m_InputSize = ImVec2(0, 0);//Size of the widget

      m_Direction;                      //Direction in which the children are placed, also the grow direction
      m_SizeWithChildren = ImVec2(0, 0);//Size of the widget with children

      m_StartCursorPos       = ImVec2(0, 0);//Local Position of the cursor
      m_CursorPos            = ImVec2(0, 0);//Local Position of the cursor
      m_BottomToTopCursorPos = ImVec2(0, 0);//Local Position of the cursor
      m_TopToBottomCursorPos = ImVec2(0, 0);//Local Position of the cursor
      m_LeftToRightCursorPos = ImVec2(0, 0);//Local Position of the cursor
      m_RightToLeftCursorPos = ImVec2(0, 0);//Local Position of the cursor
      m_HasBegun             = false;
      m_IsEnded              = true;
    }

  public:
    ImVec2 &getSizeWithChildren() {
      return m_SizeWithChildren;
    }

    //getters
  private:
    inline static ImGuiID getNewID(const std::string &label, HBIWidget *parentWindow = nullptr) {
      return ImGui::GetID(std::string(label + (parentWindow == nullptr ? "nopp" : parentWindow->getLabel())).c_str());
    }

  private:
    const HBUIType_   c_Type = HBUIType_None;//Type of the widget
    const ImGuiID     c_ID;                  //ID of the widget
    const bool        c_CanBeOwnParent;      //If the widget can be its own parent
    const std::string c_Label;               //Label of the widget

    bool m_HasSetSize;//If the widget has a set size
    bool m_HasSetPos; //If the widget has a set position

    ImVec2 m_Pos       = ImVec2(0, 0);//Local Position of the widget
    ImVec2 m_InputSize = ImVec2(0, 0);//Size of the widget

    HBDirection_ m_Direction;                      //Direction in which the children are placed, also the grow direction
    ImVec2       m_SizeWithChildren = ImVec2(0, 0);//Size of the widget with children

    ImVec2 m_StartCursorPos       = ImVec2(0, 0);//Local Position of the cursor
    ImVec2 m_CursorPos            = ImVec2(0, 0);//Local Position of the cursor
    ImVec2 m_BottomToTopCursorPos = ImVec2(0, 0);//Local Position of the cursor
    ImVec2 m_TopToBottomCursorPos = ImVec2(0, 0);//Local Position of the cursor
    ImVec2 m_LeftToRightCursorPos = ImVec2(0, 0);//Local Position of the cursor
    ImVec2 m_RightToLeftCursorPos = ImVec2(0, 0);//Local Position of the cursor
  private:
    bool m_HasBegun = false;
    bool m_IsEnded  = false;

  private:
    HBStyleFlags m_StyleFlags;//Style flags of the widget

    HBIWidget  *p_ParentWidget = nullptr;//Parent widget of the widget
    ImDrawList *p_DrawList     = nullptr;//Draw list of the widget (where to draw on)

    Properties::WidgetColorProperties *p_ColorProperties = nullptr;//Color properties of the widget
  };
}// namespace HBUI

#endif//IMGUI_HBIWIDGET_H
