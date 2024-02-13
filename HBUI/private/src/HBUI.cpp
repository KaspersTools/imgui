//
// Created by Kasper de Bruin on 06/02/2024.
//

#include <HBUI/HBUI.h>
#include "../headers/Backend.h"

#ifndef g_HBUICTX
HBUIContext *g_HBUICTX = NULL;
#endif

namespace HBUI {
  HBUIContext *
  initialize() {
    if (g_HBUICTX == NULL) {
      g_HBUICTX = new HBUIContext();
    }
    auto *settings = new MainWindowSettings();
    settings->width = 1280;
    settings->height = 720;
    settings->flags =
            HBUI_MAIN_WINDOW_FLAG_RESIZABLE |
            HBUI_MAIN_WINDOW_FLAG_TRANSPARENT |
            HBUI_MAIN_WINDOW_FLAG_CENTER_WINDOW;

    settings->title = "ImVK";
    g_HBUICTX->mainWindowSettings = settings;
    if (!initPlatformBackend(g_HBUICTX)) {
      std::cerr << "Failed to initialize platform backend" << std::endl;
    }
    if (!initGraphicsBackend(g_HBUICTX)){
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
	* Rendering
	*********************************************/
  HBUI_API void
  startFrame() {
    startRenderBackend();
  }

  HBUI_API void
  endFrame() {
    endRenderBackend();
  }

  HBUI_API bool
  wantToClose() {
    return windowShouldCloseBackend();
  }

  HBUI_API void
  shutdown() {
    shutdownBackend();

    delete g_HBUICTX;
    g_HBUICTX = NULL;
  }


  /*********************************************
	* INPUT
	*********************************************/
  bool
  mouseOverRect(const ImVec2 &start, const ImVec2 &end) {
    ImVec2 mousePos = ImGui::GetMousePos();
    return mousePos.x > start.x && mousePos.x < end.x && mousePos.y > start.y && mousePos.y < end.y;
  }

  bool
  mouseOverCircle(const ImVec2 &center, float radius) {
    ImVec2 mousePos = ImGui::GetMousePos();
    return (mousePos.x - center.x) * (mousePos.x - center.x) + (mousePos.y - center.y) * (mousePos.y - center.y) <
           radius * radius;
  }
}// namespace HBUI