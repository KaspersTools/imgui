//
// Created by Kasper de Bruin on 17/02/2024.
//

#ifndef IMGUI_HBTIME_H
#define IMGUI_HBTIME_H
#include <imgui.h>
//-----------------------------------------------------------------------------
// [SECTION] Time
//-----------------------------------------------------------------------------
struct HBTime {
  inline static float deltaTime = 0.0f;
  inline static float lastTime  = 0.0f;
  inline static float frameTime = 0.0f;

  inline static void init() {
    lastTime = ImGui::GetTime();
  }

  inline static void startFrame() {
    float time = ImGui::GetTime();
    deltaTime = time - lastTime;
    lastTime  = time;
  }

  inline static void endFrame() {
    frameTime = ImGui::GetTime() - lastTime;
  }
};

#endif //IMGUI_HBTIME_H
