//
// Created by Kasper de Bruin on 21/02/2024.
//

#ifndef IMGUI_HBUIITEMBASE_H
#define IMGUI_HBUIITEMBASE_H

#include <HBUI/HBUI.h>
#include <Utils/HBIUpdatable.h>

class IWidgetBase;
class RectWidget;
class HBWidgetManager : public HBIUpdateable {
	public:
	HBWidgetManager(const ImVec2 &defaultCursorPosStart = {0, 0},
	                HBLayoutType_ defaultLayoutType     = HBLayoutType_::HBLayoutType_Horizontal) {
		s_defaultCursorPos  = defaultCursorPosStart;
		s_defaultLayoutType = defaultLayoutType;
		reset();
	}
	static void appendWidget(IWidgetBase *widget);
	static void endAppendingWidget(const HBUIType type);

	inline static IWidgetBase *getAppendingWidget() {
		return sp_AppendingWidget;
	}
	inline static ImVec2 getCursorPos() {
		return s_cursorPos;
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
	inline static IWidgetBase *sp_AppendingWidget = nullptr;// The widget that is currently being appended to the window

	inline static ImVec2 s_cursorPos        = ImVec2(0, 0);// Position relative to native window
	inline static ImVec2 s_defaultCursorPos = ImVec2(0, 0);// Position relative to native window

	inline static HBLayoutType_ s_layoutType        = HBLayoutType_::HBLayoutType_Horizontal;// The current layout type
	inline static HBLayoutType_ s_defaultLayoutType = HBLayoutType_::HBLayoutType_Horizontal;// The layout type which is used each frame to reset back to
};

struct WidgetColorProperties {
	bool m_WithBackground = true;
	bool m_IsVisible      = true;

	ImColor m_BackgroundColor = ImColor(255, 0, 0, 255);
	ImColor m_TextColor       = ImVec4(-1, -1, -1, -1);
	ImColor m_BorderColor     = ImVec4(-1, -1, -1, -1);
	ImColor m_HoverColor      = ImVec4(-1, -1, -1, -1);

	void setColorsBasedOnType(HBUIType_ type) {
		switch (type) {
			case HBUIType_SideBar:
				m_BackgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg);
				break;
			case HBUIType_SideBarButton:
				m_BackgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
				m_HoverColor      = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
				break;
		}
	}
	void setDefaultImGuiColors() {
		if (m_BackgroundColor == ImVec4(-1, -1, -1, -1))
			m_BackgroundColor = ImColor(255, 0, 0, 255);//this wont get set automaticlys
		if (m_TextColor == ImVec4(-1, -1, -1, -1))
			m_TextColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
		if (m_BorderColor == ImVec4(-1, -1, -1, -1))
			m_BorderColor = ImGui::GetStyleColorVec4(ImGuiCol_Border);
		if (m_HoverColor == ImVec4(-1, -1, -1, -1))
			m_HoverColor = m_BackgroundColor;
	}

	WidgetColorProperties() {
		setDefaultImGuiColors();
	}


	WidgetColorProperties(HBUIType_ type, const bool widthBackground = true, const HBDrawLocation drawLocation = HBDrawFlags_MainImguiWindowDrawlist)
	    : m_WithBackground(widthBackground),
	      m_DrawLocation(drawLocation) {
		setColorsBasedOnType(type);
		setDefaultImGuiColors();
	}

	WidgetColorProperties(const ImColor &backgroundColor,
	                      const ImColor &hoverColor,
	                      const ImColor &textColor = ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Text)))
	    : WidgetColorProperties(backgroundColor, hoverColor, textColor, ImGui::GetStyleColorVec4(ImGuiCol_Border)) {
	}

	WidgetColorProperties(const ImColor &backgroundColor,
	                      const ImColor &hoverColor,
	                      const ImColor &textColor,
	                      const ImColor &borderColor)
	    : m_BackgroundColor(backgroundColor),
	      m_TextColor(textColor),
	      m_BorderColor(borderColor),
	      m_HoverColor(hoverColor) {
	}

	HBDrawLocation m_DrawLocation = HBDrawFlags_MainImguiWindowDrawlist;

	HBDrawLocation getDrawLocation() const {
		return m_DrawLocation;
	}
};

struct WidgetSizingData {
	HBWidgetResizeOptions_ m_XResizeType = HBWidgetResizeOptions_Fixed;
	HBWidgetResizeOptions_ m_YResizeType = HBWidgetResizeOptions_Fixed;
};

struct WidgetLayoutData {
	HBLayoutType_ m_LayoutType = HBLayoutType_::HBLayoutType_Horizontal;
	ImVec4 m_Padding           = ImVec4(0, 0, 0, 0);
	ImVec4 m_Margin            = ImVec4(0, 0, 0, 0);
};

struct WidgetDrawData {
	ImVec2 m_Position;
	ImVec2 m_CursorPos;

	WidgetSizingData m_SizingData;
	WidgetLayoutData m_LayoutData;

	WidgetDrawData(const WidgetSizingData &sizeData,
	               const WidgetLayoutData &layoutData) : m_SizingData(sizeData),
	                                                     m_LayoutData(layoutData) {
		m_Position  = {};
		m_CursorPos = {};
	}

	WidgetDrawData(
	    const ImVec2 &position,
	    const ImVec2 &cursorPos,
	    const ImVec4 &padding,
	    const ImVec4 &margin,

	    HBLayoutType_ layoutType,
	    HBWidgetResizeOptions_ resizeTypeXAxis,
	    HBWidgetResizeOptions_ resizeTypeYAxis) : m_Position(position),
	                                              m_CursorPos(cursorPos) {
		m_SizingData.m_XResizeType = resizeTypeXAxis;
		m_SizingData.m_YResizeType = resizeTypeYAxis;

		m_LayoutData.m_LayoutType = layoutType;
		m_LayoutData.m_Padding    = padding;
		m_LayoutData.m_Margin     = margin;
	}

	ImVec4 getMargin() const {
		return m_LayoutData.m_Margin;
	}

	ImVec4 getPadding() const {
		return m_LayoutData.m_Padding;
	}

	HBLayoutType_ getLayoutType() const {
		return m_LayoutData.m_LayoutType;
	}
};

class IWidgetBase {
	public:
	const ImGuiID cId;
	const HBUIType_ cType;
	std::string m_Label;


	//----------------------------------------------------------------------------------------------------------------------
	// [SECTION] Properties
	//----------------------------------------------------------------------------------------------------------------------
	WidgetColorProperties m_ColorProperties{};

	public:
	IWidgetBase(const std::string &label,
							const HBUIType_ uiType,
	            const HBItemFlags itemFlags,
	            const WidgetColorProperties &drawingData,
	            const WidgetSizingData &sizingData,
	            const WidgetLayoutData &layoutData)
	    : cId(ImGui::GetID(label.c_str())),
				cType(uiType),
	      m_Label(label),
	      itemFlags(itemFlags),
	      m_ColorProperties(drawingData),
	      m_drawData(WidgetDrawData(sizingData, layoutData)),
	      m_Parent(nullptr),
	      m_Children() {
		setFirstCursorPos();
	}

	IWidgetBase(const std::string &label,
	            const HBUIType_ uiType,
	            const WidgetColorProperties &colorProperties,
	            const WidgetDrawData &data,
	            const HBItemFlags itemFlags) : cId(ImGui::GetID(label.c_str())),
	                                           cType(uiType),
	                                           m_Label(label),
	                                           m_ColorProperties(colorProperties),
	                                           m_drawData(data),
	                                           m_Parent(nullptr),
	                                           m_Children(),
	                                           itemFlags(itemFlags) {
		setFirstCursorPos();
	}

	virtual ~IWidgetBase() {
		for (auto &child: m_Children) {
			delete child;
		}
	}

	//----------------------------------------------------------------------------------------------------------------------
	// [SECTION] Children
	//----------------------------------------------------------------------------------------------------------------------
	IWidgetBase *appendChild(IWidgetBase *child) {
		m_Children.push_back(child);
		child->setParent(HBWidgetManager::getAppendingWidget());

		IM_ASSERT(child != nullptr && "Child is nullptr");
		IM_ASSERT(HBWidgetManager::getAppendingWidget() != nullptr && "No widget is being appended");
		if (child->m_drawData.m_Position == ImVec2(0, 0)) {
			child->m_drawData.m_Position = m_drawData.m_CursorPos;
		}

		ImVec2 childSize = ImVec2(child->getXSize(), child->getYSize());
		if (HBUI::isFlagSet(&itemFlags, HBItemFlags_ResizeChildrenToBiggestChild)) {
			if (childSize.x > m_BiggestSize.x) {
				m_BiggestSize.x = childSize.x;
				for (auto &c: m_Children) {
					c->setSizeX(m_BiggestSize.x);
				}
			} else {
				child->setSizeX(m_BiggestSize.x);
			}
			if (childSize.y > m_BiggestSize.y) {
				m_BiggestSize.y = childSize.y;
				for (auto &c: m_Children) {
					c->setSizeY(m_BiggestSize.y);
				}
			} else {
				child->setSizeY(m_BiggestSize.y);
			}
		}

		child->m_drawData.m_Position.x += child->m_drawData.getMargin().x;
		child->m_drawData.m_Position.y += child->m_drawData.getMargin().y;

		bool isNewLine = child->cType == HBUIType_NewLine;

		getNextCursorPos(child);

		return child;
	}

	//----------------------------------------------------------------------------------------------------------------------
	// [SECTION] Drawing
	//----------------------------------------------------------------------------------------------------------------------
	ImDrawList *getDrawlist() const {
		switch (m_ColorProperties.getDrawLocation()) {
			case HBDrawFlags_MainImguiWindowDrawlist: {
				IM_ASSERT(HBUI::getMainImGuiWindow() != nullptr && "Main ImGui window is nullptr");
				return HBUI::getMainImGuiWindow()->DrawList;
			}
			case HBDrawFlags_DrawOnParent: {
				IM_ASSERT(m_Parent != nullptr && "Parent is nullptr");
				return m_Parent->getDrawlist();//recursive call to parent}
			}
			case HBDrawFlags_CreateOwnImGuiWindow:
				return ImGui::GetForegroundDrawList();//TODO: Implement this
			case HBDrawFlags_BackgroundDrawList:
				return ImGui::GetBackgroundDrawList();
			case HBDrawFlags_CurrentViewportForegroundDrawList:
				return ImGui::GetForegroundDrawList(ImGui::GetMainViewport());
				;//TODO: Implement this
			case HBDrawFlags_CurrentViewportBackgroundDrawList:
				return ImGui::GetBackgroundDrawList(ImGui::GetMainViewport());
				;//TODO: Implement this
			case HBDrawFlags_CurrentWindowDrawList:
				return ImGui::GetWindowDrawList();
				;//TODO: Implement this
		}
	}

	virtual void render();

	//----------------------------------------------------------------------------------------------------------------------
	// [SECTION] Calculations
	//----------------------------------------------------------------------------------------------------------------------
	public:
	private:
	WidgetDrawData m_drawData;
	HBItemFlags itemFlags;

	//not owned
	IWidgetBase *m_Parent = nullptr;// The parent of the widget
	std::vector<IWidgetBase *> m_Children{};
	ImVec2 m_BiggestSize = {};

	private:
	ImVec2 setFirstCursorPos() {
		if (m_drawData.m_CursorPos != ImVec2(0, 0)) {
			return m_drawData.m_CursorPos;
		} else {
			ImVec4 padding           = m_drawData.getPadding();//left top right bottom
			m_drawData.m_CursorPos.x = padding.x;
			m_drawData.m_CursorPos.y = padding.y;
		}
	}

	ImVec2 getNextCursorPos(IWidgetBase *child) {
		ImVec2 cursorPos = m_drawData.m_CursorPos;

		bool drawChildrenCentered = itemFlags & HBItemFlags_DrawChildrenCentered;
		{
			if (m_drawData.getLayoutType() == HBLayoutType_::HBLayoutType_Horizontal) {
				cursorPos.x =
				    child->m_drawData.m_Position.x + child->getXSize() + child->m_drawData.getMargin().z;
			} else if (m_drawData.getLayoutType() == HBLayoutType_::HBLayoutType_Vertical) {
				cursorPos.y =
				    child->m_drawData.m_Position.y + child->getYSize() + child->m_drawData.getMargin().w;
			}
		}
		resizeToChild(child);
		m_drawData.m_CursorPos = cursorPos;
		return cursorPos;
	}
	void resizeToChild(IWidgetBase *child) {
		ImVec2 cursorPos = m_drawData.m_CursorPos;

		if (m_drawData.getLayoutType() == HBLayoutType_::HBLayoutType_Horizontal) {
			setSizeY(child->getDrawData().m_Position.y + child->getYSize() + child->getDrawData().getMargin().w +
			         this->getDrawData().getPadding().w);
		}
		if (m_drawData.getLayoutType() == HBLayoutType_::HBLayoutType_Vertical) {
			setSizeX(child->getDrawData().m_Position.x + child->getXSize() + child->getDrawData().getMargin().z +
			         this->getDrawData().getMargin().z);
		}
	}

	public:
	virtual void setSizeX(const float newSize) = 0;
	virtual void setSizeY(const float newSize) = 0;
	virtual float getXSize() const             = 0;
	virtual float getYSize() const             = 0;

	void setPosition(const ImVec2 newPos) {
		m_drawData.m_Position = newPos;
	}

	ImVec2 getScreenPosMin() const {
		if (m_Parent != nullptr) {
			return m_drawData.m_Position + m_Parent->getScreenPosMin();
		} else {
			return m_drawData.m_Position + HBUI::getMainViewportPos();
		}
		return HBUI::getCursorScreenPos();
	}
	ImVec2 getScreenPosMax() const {
		return getScreenPosMin() + ImVec2(getXSize(), getYSize());
	}
	ImVec4 getPadding() const {
		return m_drawData.getPadding();//todo:remove
	}
	bool isWithBackground() const {
		return m_ColorProperties.m_WithBackground;
	}

	public:
	void setParent(IWidgetBase *newParent) {
		m_Parent = newParent;
	}

	//GETTERS
	IWidgetBase *getParent() const {
		return m_Parent;
	}

	const std::string &getLabel() const {
		return m_Label;
	}
	const ImGuiID &getId() const {
		return cId;
	}
	const HBUIType_ getUIType() const {
		return cType;
	}


	ImColor getBackgroundColor(const bool useHBUIColor = false) const {
		return m_ColorProperties.m_BackgroundColor;
	};
	ImColor getTextColor(const bool useHBUIColor = false) const {
		return m_ColorProperties.m_TextColor;
	};
	ImColor getBorderColor(const bool useHBUIColor = false) const {
		return m_ColorProperties.m_BorderColor;
	};
	ImColor getHoverColor(const bool useHBUIColor = false) const {
		return m_ColorProperties.m_HoverColor;
	};

	bool isVisible() const {
		return m_ColorProperties.m_IsVisible;
	}

	virtual bool isHovered() const {
		return false;//todo: implement
	}

	const WidgetDrawData &getDrawData() const {
		return m_drawData;
	}

	private:
};

template<typename SizeType>
class IWidget : public IWidgetBase {
	protected:
	IWidget(
	    const ImGuiID id,
	    const HBDrawLocation drawLocationFlag,
	    bool withBackground,
	    HBUIType_ uiType,
	    const std::string &label,
	    bool isVisible,
	    const ImVec2 position,
	    const SizeType size,
	    const ImVec2 cursorPos,
	    const ImVec4 padding,
	    const ImVec4 margin,
	    HBWidgetResizeOptions_ resizeTypeXAxis,
	    HBWidgetResizeOptions_ resizeTypeYAxis,
	    HBLayoutType_ layoutType,
	    const HBItemFlags itemFlags) : IWidgetBase(label,
																								 uiType,
	                                               WidgetColorProperties(uiType, withBackground, drawLocationFlag),//const
	                                               WidgetDrawData(
	                                                   position,
	                                                   cursorPos,
	                                                   padding,
	                                                   margin,
	                                                   layoutType,
	                                                   resizeTypeXAxis,
	                                                   resizeTypeYAxis),
	                                               itemFlags),
	                                   size(size) {
	}

	~IWidget() override {
	}

	public:
	virtual float getXSize() const override { -1; };
	virtual float getYSize() const override { -1; };
	virtual void setSizeX(const float newSize) override {
		IM_ASSERT(false && "Function not implemented on child class");
	};

	virtual void setSizeY(const float newSize) override {
		IM_ASSERT(false && "Function not implemented on child class");
	};

	//	virtual float getEndXScreenPos() const override { -1; }
	//	virtual float getEndYScreenPos() const override { -1; }

	public:
	virtual bool isHovered() const override {
		return false;
	}
	virtual void render() override {
		IWidgetBase::render();
	};

	protected:
	SizeType size;
};

class RectWidget : public IWidget<ImVec2> {
	public:
	RectWidget(
	    const ImGuiID &id,
	    const std::string &label,
	    const HBUIType_ uiType,
	    const ImVec2 position,
	    const ImVec2 size,
	    const ImVec2 cursorPos,
	    const ImVec4 padding,
	    const ImVec4 margin,
	    const bool withBackground,
	    const HBDrawLocation drawLocationFlag,
	    const HBLayoutType_ &layoutType,
	    const HBWidgetResizeOptions_ resizeTypeXAxis,
	    const HBWidgetResizeOptions_ resizeTypeYAxis,
	    const HBItemFlags itemFlags) : IWidget(id,
	                                           drawLocationFlag,
	                                           withBackground,
	                                           uiType,
	                                           label,
	                                           true,
	                                           position,
	                                           size,
	                                           cursorPos,
	                                           padding,
	                                           margin,
	                                           resizeTypeXAxis,
	                                           resizeTypeYAxis,
	                                           layoutType,
	                                           itemFlags) {
	}

	~RectWidget() override {
	}

	public:
	void setSizeX(const float newSize) override {
		size.x = newSize;
	}
	void setSizeY(const float newSize) override {
		size.y = newSize;
	}

	float getXSize() const override { return size.x; };
	float getYSize() const override { return size.y; };

	public:
	bool isHovered() const override {
		return false;
	}

	virtual void render() override {
		if (!isVisible()) {
			return;
		}
		ImDrawList *drawList = getDrawlist();
		IM_ASSERT(drawList != nullptr && "Drawlist is nullptr");

		ImVec2 min = getScreenPosMin();
		ImVec2 max = getScreenPosMax();

		if (isWithBackground()) {
			drawList->AddRectFilled(min, max, getBackgroundColor());
		} else {
			drawList->AddRect(min, max, getBackgroundColor());
		}

		IWidget::render();
	}
};

#endif//IMGUI_HBUIITEMBASE_H
