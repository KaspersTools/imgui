#pragma once

struct ImVec2;
namespace icons {
 struct uv { float x; float y; inline operator ImVec2 const& () const { return *reinterpret_cast<const ImVec2*>(this); } };
 struct frame { uv uv0; uv uv1; };
 struct sz16 {
   static constexpr frame java                     {{0.2812500000,0.0000000000},{0.3125000000,0.2500000000}};
 };
 struct sz32 {
   static constexpr frame java                     {{0.2187500000,0.0000000000},{0.2812500000,0.5000000000}};
 };
 struct sz48 {
   static constexpr frame java                     {{0.1250000000,0.0000000000},{0.2187500000,0.7500000000}};
 };
 struct sz64 {
   static constexpr frame java                     {{0.0000000000,0.0000000000},{0.1250000000,1.0000000000}};
 };
}
