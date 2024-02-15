//
// Created by Kasper de Bruin on 06/02/2024.
//

#include "../headers/Backend.h"
#include <HBUI/HBUI.h>
#ifndef g_HBUICTX
HBContext *g_HBUICTX = NULL;
#endif

namespace HBUI {
  HBContext *
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

    return g_HBUICTX;
  }

  void
  setCurrentContext(HBContext *ctx) {
    g_HBUICTX = ctx;
  }

  HBContext *
  getCurrentContext() {
    return g_HBUICTX;
  }

  void
  clearContext() {
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

  /*********************************************
    * Updating
    * *********************************************/
  HBUI_API float
  getDeltaTime() {
    return g_HBUICTX->time.deltaTime;
  }

  HBUI_API float
  getFrameTime() {
    return g_HBUICTX->time.frameTime;
  }

  HBUI_API float
  getTime() {
    ImGui::GetTime();
  }

  void
  update(float deltatime) {
    //    for (auto updatable: g_HBUICTX->updatables) {
    //      updatable->update(deltatime);
    //    }
  }

  HBUI_API void
  addUpdatable(std::shared_ptr<HBUpdatable> updatable) {
    //    g_HBUICTX->updatables.push_back(updatable);
  }

  /*********************************************
    * Rendering
    * *********************************************/
  HBUI_API void
  startFrame() {
    g_HBUICTX->drawData = std::make_shared<HBDrawData>();

    float deltaTime = getTime() - g_HBUICTX->time.lastTime;
    g_HBUICTX->time.deltaTime = deltaTime;
    g_HBUICTX->time.lastTime = getTime();
    update(deltaTime);

    startRenderBackend();
  }

  HBUI_API void
  endFrame() {
    endRenderBackend();

    g_HBUICTX->time.frameTime = getTime() - g_HBUICTX->time.lastTime;
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