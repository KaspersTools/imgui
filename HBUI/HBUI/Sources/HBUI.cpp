//
// Created by Kasper de Bruin on 06/02/2024.
//todo: cleanup with sections

// clang-format off
#include <codecvt>
#include <locale>

#include <UIItems/Windows/HBWindow.h>
#include <UIItems/Windows/HBMainWindow.h>
#include <UIItems/Interfaces/HBIWidget.h>
#include <UIItems/Debugger/HBWidgetDebugger.h>

#include <HBUI/HBUI.h>
#include <ImVK/ImVk.h>

#include <fonts/FontLoader.h>

#include <Animation/Animations.h>
#include <Animation/Animation.h>

#ifndef g_HBUICTX
HBContext *g_HBUICTX = nullptr;
#endif


// clang-format on
//-------------------------------------
// [SECTION] HBContext
//-------------------------------------

HBContext::~HBContext() {
  delete animManager;
  delete mainWindow;
}

void HBContext::initialize() {
  IM_ASSERT(!initialized && "HBContext::initialize() called twice");
}

void HBContext::afterBackendInitialized() {
  animManager = new HBUI::Animation::HBAnimManager();

  widgetDebugger = new HBUI::Debuggers::HBWidgetDebugger();

  mainWindow = new HBUI::Windows::HBWindow(
      HBUI::Backend::getLabel(),
      ImGuiID(99999),
      mainWindowFlags,
      imGuiMainWindowFlags,
      ImVec2(0, 0),
      ImVec2(0, 0),
      HBDirection_TopToBottom,
      ImVec2(0, 0),
      mainWindowStyleFlags,
      nullptr,
      HBUIType_Window);

  currentAppingWindow = mainWindow;

  initialized = true;
}

void HBContext::startFrame() {
  IM_ASSERT(initialized && "HBContext::startFrame() called before HBContext::initialize()");

  //todo: do this in the backend
  io.mousePos = ImGui::GetMousePos();

  mainWindow->begin();
  animManager->startFrame();
}

void HBContext::endFrame() const {
  IM_ASSERT(initialized && "HBContext::endFrame() called before HBContext::initialize()");
  animManager->endFrame();
  mainWindow->end();
}

bool HBContext::isActiveButton(const ImGuiID id) const {
  if (std::find(activeButtons.begin(), activeButtons.end(), id) != activeButtons.end()) {
    return true;
  }
  return false;
}

HBUI::Windows::HBWindow &HBContext::getMainWindow() const {
  IM_ASSERT(mainWindow != nullptr && "Main window is nullptr");
  return *mainWindow;
}

[[maybe_unused]] void HBContext::setMainWindowAsCurrent() {
  currentAppingWindow = mainWindow;
}
[[maybe_unused]] void HBContext::startWidget(HBUI::HBIWidget *widget) {
  IM_ASSERT(widget != nullptr && "Widget is nullptr");
  IM_ASSERT(!widget->hasBegun() && "Widget has already been begun");
  widget->p_ParentWidget = currentAppingWindow;

  currentAppingWindow = widget;
  widget->begin();
}

[[maybe_unused]] void HBContext::endCurrentWidget() {
  IM_ASSERT(currentAppingWindow != nullptr && "Current appending window is nullptr");
  IM_ASSERT(currentAppingWindow != mainWindow && "Cannot end main window");
  IM_ASSERT(currentAppingWindow->hasBegun() && "Current appending window has not been begun");

  auto windowBackup = currentAppingWindow;
  currentAppingWindow->end();

  if (currentAppingWindow->p_ParentWidget == nullptr) {
    currentAppingWindow = mainWindow;
  } else {
    currentAppingWindow = currentAppingWindow->p_ParentWidget;
  }

  delete windowBackup;
}

//-------------------------------------
// [SECTION] HBUI
//-------------------------------------
namespace HBUI {
  HBContext *initialize(const std::string &title, int width, int height, HBBackendWindowFlags backendWindowFlags) {
    IM_ASSERT(g_HBUICTX == nullptr && "HBUI::initialize() called twice");

    g_HBUICTX = new HBContext();
    g_HBUICTX->initialize();

    auto io = HBIO{
        .title  = title,
        .width  = width,
        .height = height};
    g_HBUICTX->io = io;

    if (!Backend::initPlatformBackend(width, height, backendWindowFlags)) {
      std::cerr << "Failed to initialize platform backend" << std::endl;
      return nullptr;
    }

    if (!Backend::initGraphicsBackend()) {
      std::cerr << "Failed to initialize graphics backend" << std::endl;
      return nullptr;
    }

    ImGuiIO &imIo             = ImGui::GetIO();
    imIo.FontAllowUserScaling = true;// activate zoom feature with ctrl + mousewheel

    afterBackendInitialized();
    return g_HBUICTX;
  }

  void afterBackendInitialized() {
    g_HBUICTX->afterBackendInitialized();
    g_HBUICTX->fontLoader = new HBUI::Fonts::FontLoader(true);
    HBTime::init();
  }


  //-------------------------------------
  // [SECTION] Context
  //-------------------------------------
  void setCurrentContext(HBContext *ctx) {
    g_HBUICTX = ctx;
  }

  HBContext *getCurrentContext() {
    return g_HBUICTX;
  }

  void clearContext() {//todo: implement
  }


  //-------------------------------------
  // [SECTION] IO
  //-------------------------------------
  HBUI_API HBIO &getIO() {
    IM_ASSERT(g_HBUICTX != nullptr && "Current Context is nullptr");
    return g_HBUICTX->io;
  }

  HBUI_API ImVec2 getMousePos() {
    IM_ASSERT(g_HBUICTX != nullptr && "Current Context is nullptr");
    return g_HBUICTX->io.mousePos;
  }

  //-------------------------------------
  // [SECTION] Main Window
  //-------------------------------------
  HBUI_API ImGuiViewport *getCurrentViewport() {
    return ImGui::GetCurrentContext()->CurrentViewport;
  }
  HBUI_API ImGuiViewport *getMainViewport() {
    return ImGui::GetMainViewport();
  }

  HBUI_API ImVec2 getNativeWindowSize() {
    return Backend::getWindowSize();
  }
  HBUI_API ImVec2 getViewportSize(ImGuiViewport *viewport) {
    IM_ASSERT(viewport != nullptr && "HBUI::getViewportSize() but viewport is null");
    return viewport->Size;
  }
  HBUI_API ImVec2 getMainViewportSize() {
    return getViewportSize(ImGui::GetMainViewport());
  }

  /**
   * @brief Get the main window position
   * @return ImVec2 position of the main window in local coordinates
   */
  ImVec2 getMainWindowPos() {
    return HBUI::getCurrentContext()->getMainWindow().getPos();
  }

  //native window
  ImVec2 getNativeWindowPos() {
    return Backend::getWindowPosition();
  }

  ImColor &getNativeWindowClearColor() {
    return HBUI::getCurrentContext()->windowData.ClearColor;
  }
  void setNativeWindowClearColor(const ImColor &color) {
    HBUI::getCurrentContext()->windowData.ClearColor = color;
  }

  ImVec2 getViewportPos(ImGuiViewport *viewport) {
    IM_ASSERT(viewport != nullptr && "HBUI::getViewportPos() but viewport is null");
    return viewport->Pos;
  }

  ImVec2 getMainViewportPos() {
    return getViewportPos(ImGui::GetMainViewport());
  }

  ImVec2 getCurrentViewportPos() {
    return getViewportPos(getCurrentViewport());
  }

  HBIWidget *getCurrentAppendingWidget() {
    HBContext *ctx = getCurrentContext();

    IM_ASSERT(ctx != nullptr && "Current Context is nullptr");

    return ctx->currentAppingWindow;
  }

  //-------------------------------------
  // [SECTION] Cursor
  //-------------------------------------
  ImVec2 getMainWindowCursorPos() {
    HBContext *ctx = getCurrentContext();
    IM_ASSERT(ctx != nullptr && "Current Context is nullptr");
    return ctx->getMainWindow().getCursorPos();
  }

  ImVec2 getMainWindowCursorScreenPos() {
    return getMainViewportPos() + getMainWindowCursorPos();
  }

  //todo:	 ImVec2 getMainWindowCursorScreenPos(ImGuiViewport *viewport);
  ImVec2 getContentRegionMaxMainWindow() {
    IM_ASSERT(false && "Not implemented");
  }

  ImVec2 getContentRegionAvailMainWindow() {
    IM_ASSERT(getCurrentContext() != nullptr && "Main window is nullptr");
    return getCurrentContext()->getMainWindow().calculateSize() - getMainWindowCursorPos();
  }

  //todo:  ImVec2 getContentRegionAvail(ImGuiViewport *viewport);
  HBLayoutType_ getCurrentLayout() {
    HBContext *ctx = getCurrentContext();
    IM_ASSERT(ctx != nullptr && "Current Context is nullptr");
    HBWidgetManager *widgetManager = ctx->widgetManager;
    IM_ASSERT(widgetManager != nullptr && "Widget Manager is nullptr in current context");
    IM_ASSERT(false && "Not implemented");
    //		return widgetManager->getLayoutType();
  }

  void setLayout(HBLayoutType_ layoutType) {
    HBContext *ctx = getCurrentContext();
    IM_ASSERT(ctx != nullptr && "Current Context is nullptr");
    HBWidgetManager *widgetManager = ctx->widgetManager;
    IM_ASSERT(widgetManager != nullptr && "Widget Manager is nullptr in current context");
    IM_ASSERT(false && "Not implemented");
    //		widgetManager->setLayoutType(layoutType);
  }


  void update([[maybe_unused]] float deltatime) {
  }

  void startFrame() {
    IM_ASSERT(g_HBUICTX != nullptr && "Current Context is nullptr");
    IM_ASSERT(!g_HBUICTX->startFrameCalled && "HBUI::startFrame() called twice");
    IM_ASSERT(g_HBUICTX->endFrameCalled && "HBUI::endFrame() has not been called");

    g_HBUICTX->endFrameCalled   = false;
    g_HBUICTX->startFrameCalled = true;

    HBTime::startFrame();
    Backend::startRenderBackend();

    g_HBUICTX->startFrame();
  }

  void endFrame() {
    IM_ASSERT(g_HBUICTX != nullptr && "Current Context is nullptr");
    IM_ASSERT(g_HBUICTX->startFrameCalled && "HBUI::startFrame() has not been called");
    IM_ASSERT(!g_HBUICTX->endFrameCalled && "HBUI::endFrame() called twice");

    g_HBUICTX->endFrameCalled   = true;
    g_HBUICTX->startFrameCalled = false;
    g_HBUICTX->endFrame();

    Backend::endRenderBackend(g_HBUICTX->windowData);
    HBTime::endFrame();
  }

  //  void beginMainWindow() {
  //    HBContext *ctx = getCurrentContext();
  //    IM_ASSERT(ctx != nullptr && "Current Context is nullptr");
  //
  //    ctx->currentAppingWindow = ctx->mainWindow;
  //    ctx->mainWindow->begin();
  //  }

  //  void endMainWindow() {
  //    HBContext *ctx = getCurrentContext();
  //    IM_ASSERT(ctx != nullptr && "Current Context is nullptr");
  //    IM_ASSERT(ctx->mainWindow != nullptr && "Main window is nullptr");
  //    ctx->mainWindow->end();
  //  }

  bool wantToClose() {
    return Backend::getWindowShouldCloseBackend();
  }

  void shutdown() {
    Backend::shutdownBackend();
    delete g_HBUICTX;
    g_HBUICTX = nullptr;
  }

  ImFont *getBigFont() {
    return g_HBUICTX->fontLoader->getBigFont();
  }

  bool isFlagSet(int *flags, int flag) {
    return (*flags & flag) == flag;
  }

  //-------------------------------------
  // [SECTION] Style
  //-------------------------------------
  HBStyle &getStyle() {
    IM_ASSERT(g_HBUICTX != nullptr && "Current Context is nullptr");

    return *g_HBUICTX->style;
  }

  //-------------------------------------
  // [SECTION] Fonts
  //-------------------------------------
  float getWindowSizeDpiScaleFactor() {
    return Backend::getWindowSizeDPIScaleFactor();
  }

  float getFontSizeIncreaseFactor() {
    return Backend::getFontSizeIncreaseFactor();
  }

  ImVec2 getWindowScaleFactor() {
    return Backend::getWindowScaleFactor();
  }

  //-------------------------------------
  //[SECTION] Helper functions
  //-------------------------------------
  std::string wchar32ToUtf8(const ImWchar &wchar) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.to_bytes(wchar);
  }

  bool aabb(const ImVec2 &min1, const ImVec2 &max1, const ImVec2 &min2, const ImVec2 &max2) {
    return (min1.x < max2.x && max1.x > min2.x) && (min1.y < max2.y && max1.y > min2.y);
  }

  /**
   * @brief Get the centered position for an element
   * @param size size of the element
   * @param parentSize size of the parent element
   * @return ImVec2 position
   */
  ImVec2 getCenteredwPosition(const ImVec2 &size,
                              const ImVec2 &parentSize) {
    return ((parentSize * 0.5f) - (size * 0.5f));
  }
}// namespace HBUI