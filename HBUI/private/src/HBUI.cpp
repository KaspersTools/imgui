//
// Created by Kasper de Bruin on 06/02/2024.
//todo: cleanup with sections

#include "../headers/Backend.h"
#include <HBUI/HBUI.h>
#ifndef g_HBUICTX
HBContext *g_HBUICTX = NULL;
#endif

namespace HBUI {
  HBUI_API HBContext *
  initialize(const std::string &title, int width, int height, MainWindowFlags flags) {
    if (g_HBUICTX == NULL) {
      g_HBUICTX = new HBContext();
    }
    g_HBUICTX->initialized = true;//fixme delete old context first

    auto io = HBIO{
            .title = title,
            .width = width,
            .height = height,
            .mainWindowFlags = flags};
    g_HBUICTX->io = io;


    if (!initPlatformBackend(g_HBUICTX)) {
      std::cerr << "Failed to initialize platform backend" << std::endl;
    }
    if (!initGraphicsBackend(g_HBUICTX)) {
      std::cerr << "Failed to initialize graphics backend" << std::endl;
    }

    HBTime::init();

    return g_HBUICTX;
  }

  HBUI_API void
  setCurrentContext(HBContext *ctx) {
    g_HBUICTX = ctx;
  }

  HBUI_API HBContext *
  getCurrentContext() {
    return g_HBUICTX;
  }

  HBUI_API void
  clearContext() {//todo: implement
  }

  HBUI_API HBStyle &
  getStyle() {
    return getCurrentContext()->style;
  }

  HBUI_API HBIO &
  getIO() {
    return getCurrentContext()->io;
  }

  HBUI_API HBDrawData &
  getDrawData() {
    return *getCurrentContext()->drawData;
  }

  HBUI_API ImVec2
  getCursorViewportPos(){
    auto windowPos  = HBUI::getDrawData().cursorPos;
    auto vp         = ImGui::GetMainViewport();

    return ImVec2(windowPos.x + vp->Pos.x,
                  windowPos.y + vp->Pos.y);
  }

  HBUI_API ImVec2
  getContentRegionAvail(){
    auto windowPos = HBUI::getDrawData().cursorPos;
    auto vp = ImGui::GetMainViewport();

    return ImVec2(vp->Size.x - windowPos.x,
                  vp->Size.y - windowPos.y);
  }

  HBUI_API ImVec2
  getWindowSize(){
    return ImGui::GetIO().DisplaySize;
  }

  HBUI_API ImVec2
  getViewportPos(){
    return ImGui::GetMainViewport()->Pos;
  }

  HBUI_API ImVec2
  getViewportSize(){
    return ImGui::GetMainViewport()->Size;
  }

  /*********************************************
    * MainWin
    * *********************************************/
  HBUI_API bool
  isMaximized() {
    ImVec2 size = getWindowSize();
    ImVec2 frameSize = getWindowFrameSize();

    return (size.x + frameSize.x == getMonitorWidth() &&
            size.y + frameSize.y == getMonitorHeight());
  }

  HBUI_API void update(float deltatime){
  }
  /*********************************************
    * Rendering
    * *********************************************/

  void fullScreenemptyWindow() {
  }
  HBUI_API void
  startFrame() {
    HBTime::startFrame();
    update(g_HBUICTX->time.deltaTime);
    g_HBUICTX->drawData = std::make_shared<HBDrawData>();
    startRenderBackend();
  }

  HBUI_API void
  endFrame() {
    endRenderBackend();
    HBTime::endFrame();
  }

  HBUI_API bool
  wantToClose() {
    return getWindowShouldCloseBackend();
  }

  HBUI_API void
  shutdown() {
    shutdownBackend();
    delete g_HBUICTX;
    g_HBUICTX = NULL;
  }


  /*********************************************
    * Rendering
    * *********************************************/
  HBUI_API bool
  mouseOverRect(const ImVec2 &start, const ImVec2 &end) {
    ImVec2 mousePos = ImGui::GetMousePos();
    return mousePos.x > start.x && mousePos.x < end.x && mousePos.y > start.y && mousePos.y < end.y;
  }

  HBUI_API bool
  mouseOverCircle(const ImVec2 &center, float radius) {
    ImVec2 mousePos = ImGui::GetMousePos();
    return (mousePos.x - center.x) * (mousePos.x - center.x) + (mousePos.y - center.y) * (mousePos.y - center.y) < radius * radius;
  }

  /*********************************************
      * Flags
      * *********************************************/
  HBUI_API void toggleFlag(int flag) {
    g_HBUICTX->io.mainWindowFlags ^= flag;
    setBackendWindowFlags(*g_HBUICTX);
  }

  HBUI_API bool isFlagSet(int *flags, int flag) {
    return (*flags & flag) == flag;
  }

}// namespace HBUI