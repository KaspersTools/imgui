#pragma once

#include <imgui.h>
#include "fontAwesome.hpp"
#include "fontAwesome_codes.hpp"

namespace icons {
 namespace Font {
  static const ImWchar ranges[] = { fontAwesome::Font_StartCode , fontAwesome::Font_EndCode, 0 };
  inline ImFont* Load(ImGuiIO& io, const float size, ImFontConfig* config) {
		config->RasterizerDensity  = 2.0f;
		config->RasterizerMultiply = 1.0f;

   void* data = const_cast<unsigned int*>(fontAwesome::data::DATA);
   if (config) {
    config->FontDataOwnedByAtlas = false;
    return io.Fonts->AddFontFromMemoryTTF(data, fontAwesome::data::SIZE, size, config, ranges);
   }
   else {
    ImFontConfig dconf;
    dconf.FontDataOwnedByAtlas = false;
    return io.Fonts->AddFontFromMemoryTTF(data, fontAwesome::data::SIZE, size, &dconf, ranges);
   }
  }
 }
}
