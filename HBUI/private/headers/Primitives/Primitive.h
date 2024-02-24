//
// Created by Kasper de Bruin on 20/02/2024.
//

#ifndef IMGUI_PRIMITIVE_H
#define IMGUI_PRIMITIVE_H

enum HBPrimitveTypes_ {
  Quad = 0
};

template<typename TStart, typename TEnd>
struct HBUIPrimitive {
  HBUIPrimitive(const TStart &start, const TEnd &end,
                const ImColor &color = ImColor(255, 255, 255, 255),
                float thickness = 1,
                bool filled = false,
                bool visible = true,
                ImDrawFlags drawFlags = 0
  ) :
      start(start), end(end), color(color), thickness(thickness), filled(filled), visible(visible),
      drawFlags(drawFlags) {}

  HBUIPrimitive() {}

  virtual void draw(ImDrawList *drawList = ImGui::GetForegroundDrawList()) = 0;

  void setStart(TStart start) {
    this->start = start;
  }

  void setEnd(TEnd end) {
    this->end = end;
  }

  TStart getStart() {
    return start;
  }

  TEnd getEnd() {
    return end;
  }


  ImColor     color;
  float       thickness;
  bool        filled;
  bool        visible;
  ImDrawFlags drawFlags;

protected:
  TStart start;
  TEnd   end;

};


struct HBUIRect {
  inline static void draw(
      ImDrawList *drawList,
      const ImVec2 &start,
      const ImVec2 &end,
      const ImColor &color,
      float rounding = 0,
      float borderThickness = 1,
      bool filled = false,
      const ImDrawFlags drawFlags = 0,
      bool visible = true) {
    if (filled) {
      drawList->AddRectFilled(start, end, color, rounding);
    } else {
      drawList->AddRect(start, end, color, rounding, drawFlags, borderThickness);
    }
  }

  float rounding;
};

#endif //IMGUI_PRIMITIVE_H
