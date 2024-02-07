//
// Created by Kasper de Bruin on 06/02/2024.
//

#ifndef HUMMINGBIRD_HBUI_H
#define HUMMINGBIRD_HBUI_H
#include <iostream>
#include <chrono>
#include <map>
#include "../../../imgui.h"

//#include <KDB_ImGui/Extension.h>

//
//}
//
//// Forward declarations
//struct ImDrawChannel;               // Temporary storage to output draw commands out of order, used by ImDrawListSplitter and ImDrawList::ChannelsSplit()
//struct ImDrawCmd;                   // A single draw command within a parent ImDrawList (generally maps to 1 GPU draw call, unless it is a callback)
//struct ImDrawData;                  // All draw command lists required to render the frame + pos/size coordinates to use for the projection matrix.
//struct ImDrawList;                  // A single draw command list (generally one per window, conceptually you may see this as a dynamic "mesh" builder)
//struct ImDrawListSharedData;        // Data shared among multiple draw lists (typically owned by parent ImGui context, but you may create one yourself)
//struct ImDrawListSplitter;          // Helper to split a draw list into different layers which can be drawn into out of order, then flattened back.
//struct ImDrawVert;                  // A single vertex (pos + uv + col = 20 bytes by default. Override layout with IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT)
//struct ImFont;                      // Runtime data for a single font within a parent ImFontAtlas
//struct ImFontAtlas;                 // Runtime data for multiple fonts, bake multiple fonts into a single texture, TTF/OTF font loader
//struct ImFontBuilderIO;             // Opaque interface to a font builder (stb_truetype or FreeType).
//struct ImFontConfig;                // Configuration data when adding a font or merging fonts
//struct ImFontGlyph;                 // A single font glyph (code point + coordinates within in ImFontAtlas + offset)
//struct ImFontGlyphRangesBuilder;    // Helper to build glyph ranges from text/string data
//struct ImColor;                     // Helper functions to create a color that can be converted to either u32 or float4 (*OBSOLETE* please avoid using)
//struct ImGuiContext;                // Dear ImGui context (opaque structure, unless including imgui_internal.h)
//struct ImGuiIO;                     // Main configuration and I/O between your application and ImGui
//struct ImGuiInputTextCallbackData;  // Shared state of InputText() when using custom ImGuiInputTextCallback (rare/advanced use)
//struct ImGuiKeyData;                // Storage for ImGuiIO and IsKeyDown(), IsKeyPressed() etc functions.
//struct ImGuiListClipper;            // Helper to manually clip large list of items
//struct ImGuiOnceUponAFrame;         // Helper for running a block of code not more than once a frame
//struct ImGuiPayload;                // User data payload for drag and drop operations
//struct ImGuiPlatformIO;             // Multi-viewport support: interface for Platform/Renderer backends + viewports to render
//struct ImGuiPlatformMonitor;        // Multi-viewport support: user-provided bounds for each connected monitor/display. Used when positioning popups and tooltips to avoid them straddling monitors
//struct ImGuiPlatformImeData;        // Platform IME data for io.SetPlatformImeDataFn() function.
//struct ImGuiSizeCallbackData;       // Callback data when using SetNextWindowSizeConstraints() (rare/advanced use)
//struct ImGuiStorage;                // Helper for key->value storage
//struct ImGuiStyle;                  // Runtime data for styling/colors
//struct ImGuiTableSortSpecs;         // Sorting specifications for a table (often handling sort specs for a single column, occasionally more)
//struct ImGuiTableColumnSortSpecs;   // Sorting specification for one column of a table
//struct ImGuiTextBuffer;             // Helper to hold and append into a text buffer (~string builder)
//struct ImGuiTextFilter;             // Helper to parse and apply text filters (e.g. "aaaaa[,bbbbb][,ccccc]")
//struct ImGuiViewport;               // A Platform Window (always 1 unless multi-viewport are enabled. One per platform window to output to). In the future may represent Platform Monitor
//struct ImGuiWindowClass;            // Window class (rare/advanced uses: provide hints to the platform backend via altered viewport flags and parent/child info)
//
//// Enumerations
//// - We don't use strongly typed enums much because they add constraints (can't extend in private code, can't store typed in bit fields, extra casting on iteration)
//// - Tip: Use your programming IDE navigation facilities on the names in the _central column_ below to find the actual flags/enum lists!
////   In Visual Studio IDE: CTRL+comma ("Edit.GoToAll") can follow symbols in comments, whereas CTRL+F12 ("Edit.GoToImplementation") cannot.
////   With Visual Assist installed: ALT+G ("VAssistX.GoToImplementation") can also follow symbols in comments.
//enum ImGuiKey
//		: int;                // -> enum ImGuiKey              // Enum: A key identifier (ImGuiKey_XXX or ImGuiMod_XXX value)
//enum ImGuiMouseSource
//		: int;        // -> enum ImGuiMouseSource      // Enum; A mouse input source identifier (Mouse, TouchScreen, Pen)
//typedef int ImGuiCol;               // -> enum ImGuiCol_             // Enum: A color identifier for styling
//typedef int ImGuiCond;              // -> enum ImGuiCond_            // Enum: A condition for many Set*() functions
//typedef int ImGuiDataType;          // -> enum ImGuiDataType_        // Enum: A primary data type
//typedef int ImGuiDir;               // -> enum ImGuiDir_             // Enum: A cardinal direction
//typedef int ImGuiMouseButton;       // -> enum ImGuiMouseButton_     // Enum: A mouse button identifier (0=left, 1=right, 2=middle)
//typedef int ImGuiMouseCursor;       // -> enum ImGuiMouseCursor_     // Enum: A mouse cursor shape
//typedef int ImGuiSortDirection;     // -> enum ImGuiSortDirection_   // Enum: A sorting direction (ascending or descending)
//typedef int ImGuiStyleVar;          // -> enum ImGuiStyleVar_        // Enum: A variable identifier for styling
//typedef int ImGuiTableBgTarget;     // -> enum ImGuiTableBgTarget_   // Enum: A color target for TableSetBgColor()
//
//// Flags (declared as int for compatibility with old C++, to allow using as flags without overhead, and to not pollute the top of this file)
//// - Tip: Use your programming IDE navigation facilities on the names in the _central column_ below to find the actual flags/enum lists!
////   In Visual Studio IDE: CTRL+comma ("Edit.GoToAll") can follow symbols in comments, whereas CTRL+F12 ("Edit.GoToImplementation") cannot.
////   With Visual Assist installed: ALT+G ("VAssistX.GoToImplementation") can also follow symbols in comments.
//typedef int ImDrawFlags;            // -> enum ImDrawFlags_          // Flags: for ImDrawList functions
//typedef int ImDrawListFlags;        // -> enum ImDrawListFlags_      // Flags: for ImDrawList instance
//typedef int ImFontAtlasFlags;       // -> enum ImFontAtlasFlags_     // Flags: for ImFontAtlas build
//typedef int ImGuiBackendFlags;      // -> enum ImGuiBackendFlags_    // Flags: for io.BackendFlags
//typedef int ImGuiButtonFlags;       // -> enum ImGuiButtonFlags_     // Flags: for InvisibleButton()
//typedef int ImGuiChildFlags;        // -> enum ImGuiChildFlags_      // Flags: for BeginChild()
//typedef int ImGuiColorEditFlags;    // -> enum ImGuiColorEditFlags_  // Flags: for ColorEdit4(), ColorPicker4() etc.
//typedef int ImGuiConfigFlags;       // -> enum ImGuiConfigFlags_     // Flags: for io.ConfigFlags
//typedef int ImGuiComboFlags;        // -> enum ImGuiComboFlags_      // Flags: for BeginCombo()
//typedef int ImGuiDockNodeFlags;     // -> enum ImGuiDockNodeFlags_   // Flags: for DockSpace()
//typedef int ImGuiDragDropFlags;     // -> enum ImGuiDragDropFlags_   // Flags: for BeginDragDropSource(), AcceptDragDropPayload()
//typedef int ImGuiFocusedFlags;      // -> enum ImGuiFocusedFlags_    // Flags: for IsWindowFocused()
//typedef int ImGuiHoveredFlags;      // -> enum ImGuiHoveredFlags_    // Flags: for IsItemHovered(), IsWindowHovered() etc.
//typedef int ImGuiInputTextFlags;    // -> enum ImGuiInputTextFlags_  // Flags: for InputText(), InputTextMultiline()
//typedef int ImGuiKeyChord;          // -> ImGuiKey | ImGuiMod_XXX    // Flags: for storage only for now: an ImGuiKey optionally OR-ed with one or more ImGuiMod_XXX values.
//typedef int ImGuiPopupFlags;        // -> enum ImGuiPopupFlags_      // Flags: for OpenPopup*(), BeginPopupContext*(), IsPopupOpen()
//typedef int ImGuiSelectableFlags;   // -> enum ImGuiSelectableFlags_ // Flags: for Selectable()
//typedef int ImGuiSliderFlags;       // -> enum ImGuiSliderFlags_     // Flags: for DragFloat(), DragInt(), SliderFloat(), SliderInt() etc.
//typedef int ImGuiTabBarFlags;       // -> enum ImGuiTabBarFlags_     // Flags: for BeginTabBar()
//typedef int ImGuiTabItemFlags;      // -> enum ImGuiTabItemFlags_    // Flags: for BeginTabItem()
//typedef int ImGuiTableFlags;        // -> enum ImGuiTableFlags_      // Flags: For BeginTable()
//typedef int ImGuiTableColumnFlags;  // -> enum ImGuiTableColumnFlags_// Flags: For TableSetupColumn()
//typedef int ImGuiTableRowFlags;     // -> enum ImGuiTableRowFlags_   // Flags: For TableNextRow()
//typedef int ImGuiTreeNodeFlags;     // -> enum ImGuiTreeNodeFlags_   // Flags: for TreeNode(), TreeNodeEx(), CollapsingHeader()
//typedef int ImGuiViewportFlags;     // -> enum ImGuiViewportFlags_   // Flags: for ImGuiViewport
//typedef int ImGuiWindowFlags;       // -> enum ImGuiWindowFlags_     // Flags: for Begin(), BeginChild()
//
//// ImTexture: user data for renderer backend to identify a texture [Compile-time configurable type]
//// - To use something else than an opaque void* pointer: override with e.g. '#define ImTextureID MyTextureType*' in your imconfig.h file.
//// - This can be whatever to you want it to be! read the FAQ about ImTextureID for details.
//#ifndef ImTextureID
//typedef void *ImTextureID;          // Default: store a pointer or an integer fitting in a pointer (most renderer backends are ok with that)
//#endif
//
//// ImDrawIdx: vertex index. [Compile-time configurable type]
//// - To use 16-bit indices + allow large meshes: backend need to set 'io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset' and handle ImDrawCmd::VtxOffset (recommended).
//// - To use 32-bit indices: override with '#define ImDrawIdx unsigned int' in your imconfig.h file.
//#ifndef ImDrawIdx
//typedef unsigned short ImDrawIdx;   // Default: 16-bit (for maximum compatibility with renderer backends)
//#endif
//
//// Scalar data types
//typedef unsigned int ImGuiID;// A unique ID used by widgets (typically the result of hashing a stack of string)
//typedef signed char ImS8;   // 8-bit signed integer
//typedef unsigned char ImU8;   // 8-bit unsigned integer
//typedef signed short ImS16;  // 16-bit signed integer
//typedef unsigned short ImU16;  // 16-bit unsigned integer
//typedef signed int ImS32;  // 32-bit signed integer == int
//typedef unsigned int ImU32;  // 32-bit unsigned integer (often used to store packed colors)
//typedef signed long long ImS64;  // 64-bit signed integer
//typedef unsigned long long ImU64;  // 64-bit unsigned integer
//
//// Character types
//// (we generally use UTF-8 encoded string in the API. This is storage specifically for a decoded character used for keyboard input and display)
//typedef unsigned int ImWchar32;     // A single decoded U32 character/code point. We encode them as multi bytes UTF-8 when used in strings.
//typedef unsigned short ImWchar16;   // A single decoded U16 character/code point. We encode them as multi bytes UTF-8 when used in strings.

namespace HBUI {
  [[maybe_unused]]void initialize();
}// namespace HBUI
#endif//HUMMINGBIRD_HBUI_H
