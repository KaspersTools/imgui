//
// Created by Kasper de Bruin on 06/02/2024.
//

#include "../headers/Backend.h"
#include <HBUI/HBUI.h>
#ifndef g_HBUICTX
HBUIContext *g_HBUICTX = NULL;
#endif

namespace HBUI {
  HBUIContext *
  initialize() {
    if (g_HBUICTX == NULL) {
      g_HBUICTX = new HBUIContext();
    }
    auto *settings = new MainWindowSettings("HBUI Example", 1280, 720, HBUI_MAIN_WINDOW_FLAG_NONE | HBUI_MAIN_WINDOW_FLAG_DEFAULT_DOCKSPACE);
    g_HBUICTX->mainWindowSettings = settings;
    if (!initPlatformBackend(g_HBUICTX)) {
      std::cerr << "Failed to initialize platform backend" << std::endl;
    }
    if (!initGraphicsBackend(g_HBUICTX)) {
      std::cerr << "Failed to initialize graphics backend" << std::endl;
    }

    return g_HBUICTX;
  }

  void
  setCurrentContext(HBUIContext *ctx) {
    g_HBUICTX = ctx;
  }

  HBUIContext *
  getCurrentContext() {
    return g_HBUICTX;
  }

  void
  clearContext() {
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
    for (auto updatable: g_HBUICTX->updatables) {
      updatable->update(deltatime);
    }
  }

  HBUI_API void
  addUpdatable(std::shared_ptr<Updatable> updatable) {
    g_HBUICTX->updatables.push_back(updatable);
  }

  /*********************************************
    * Rendering
    * *********************************************/
  HBUI_API void
  startFrame() {
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
    return (mousePos.x - center.x) * (mousePos.x - center.x) + (mousePos.y - center.y) * (mousePos.y - center.y) <
           radius * radius;
  }
}// namespace HBUI