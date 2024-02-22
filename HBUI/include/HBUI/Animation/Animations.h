//
// Created by Kasper de Bruin on 19/02/2024.
//

#ifndef IMGUI_ANIMATIONS_H
#define IMGUI_ANIMATIONS_H

template<typename T>
static inline T Clamp(T v, T mn, T mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }

static inline ImVec2 Clamp(const ImVec2 &v, const ImVec2 &mn, const ImVec2 &mx) {
  return ImVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x,
                (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y);
}
static inline ImVec2 Lerp(const ImVec2 &a, const ImVec2 &b, float t) {
  return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}

static inline ImVec2 Lerp(const ImVec2 &a, const ImVec2 &b, const ImVec2 &t) {
  return ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y);
}

#endif //IMGUI_ANIMATIONS_H
