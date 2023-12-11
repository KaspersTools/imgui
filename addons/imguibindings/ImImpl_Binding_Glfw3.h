#ifndef IMIMPL_BINDING_H
#define IMIMPL_BINDING_H


#include "imguibindings.h"

#if (GLFW_VERSION_MAJOR>=3 && GLFW_VERSION_MINOR>0)
#define GLFW_HAS_MOUSE_CURSOR_SUPPORT
#endif //GLFW_VERSION


#if (defined(GLFW_HAS_MOUSE_CURSOR_SUPPORT) && defined(IMGUI_GLFW_NO_NATIVE_CURSORS))
#undef IMGUI_GLFW_NO_NATIVE_CURSORS
#warning IMGUI_GLFW_NO_NATIVE_CURSORS must be undefined globally, because your GLFW version is >= 3.1 and has embedded mouse cursor support.
#endif //IMGUI_GLFW_NO_NATIVE_CURSORS

#ifdef GLFW_HAS_MOUSE_CURSOR_SUPPORT

#   ifdef GLFW_RESIZE_NESW_CURSOR  // Let's be nice to people who pulled GLFW between 2019-04-16 (3.4 define) and 2019-11-29 (cursors defines) // FIXME: Remove when GLFW 3.4 is released?
#       define GLFW_HAS_NEW_CURSORS    (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3400) // 3.4+ GLFW_RESIZE_ALL_CURSOR, GLFW_RESIZE_NESW_CURSOR, GLFW_RESIZE_NWSE_CURSOR, GLFW_NOT_ALLOWED_CURSOR
#   else
#       define GLFW_HAS_NEW_CURSORS    (0)
#   endif

#   if GLFW_HAS_NEW_CURSORS==0
    static const int glfwCursorIds[] = {
        GLFW_ARROW_CURSOR,
        GLFW_IBEAM_CURSOR,
        GLFW_HAND_CURSOR,
        GLFW_VRESIZE_CURSOR,
        GLFW_HRESIZE_CURSOR,
        GLFW_CROSSHAIR_CURSOR,
        GLFW_CROSSHAIR_CURSOR,
        GLFW_HAND_CURSOR,
        GLFW_ARROW_CURSOR,
        GLFW_ARROW_CURSOR
    };
#   else
static const int glfwCursorIds[] = {
    GLFW_ARROW_CURSOR,          // ImGuiMouseCursor_Arrow
    GLFW_IBEAM_CURSOR,          // ImGuiMouseCursor_TextInput
    GLFW_RESIZE_ALL_CURSOR,     // ImGuiMouseCursor_ResizeAll
    GLFW_VRESIZE_CURSOR,        // ImGuiMouseCursor_ResizeNS
    GLFW_HRESIZE_CURSOR,        // ImGuiMouseCursor_ResizeEW
    GLFW_RESIZE_NESW_CURSOR,    // ImGuiMouseCursor_ResizeNESW
    GLFW_RESIZE_NWSE_CURSOR,    // ImGuiMouseCursor_ResizeNWSE
    GLFW_NOT_ALLOWED_CURSOR,    // ImGuiMouseCursor_Hand
    GLFW_ARROW_CURSOR,          // ImGuiMouseCursor_NotAllowed
    GLFW_ARROW_CURSOR           // -1: ImGuiMouseCursor_None
};
#   endif
    static GLFWcursor* glfwCursors[ImGuiMouseCursor_COUNT+1];

#else //GLFW_HAS_MOUSE_CURSOR_SUPPORT
#ifndef IMGUI_GLFW_NO_NATIVE_CURSORS
#   ifdef _WIN32
#       define IMGUI_USE_WIN32_CURSORS     // Optional, but needs at window creation: wc.hCursor = LoadCursor( NULL, NULL); // Now the window class is inside glfw3... Not sure how I can access it...
#       ifdef IMGUI_USE_WIN32_CURSORS
    static const LPCTSTR win32CursorIds[] = {
        IDC_ARROW,
        IDC_IBEAM,
        IDC_SIZEALL,      //SDL_SYSTEM_CURSOR_HAND,    // or SDL_SYSTEM_CURSOR_SIZEALL  //ImGuiMouseCursor_Move,                  // Unused by ImGui
        IDC_SIZENS,       //ImGuiMouseCursor_ResizeNS,              // Unused by ImGui
        IDC_SIZEWE,       //ImGuiMouseCursor_ResizeEW,              // Unused by ImGui
        IDC_SIZENESW,     //ImGuiMouseCursor_ResizeNESW,
        IDC_SIZENWSE,     //ImGuiMouseCursor_ResizeNWSE,          // Unused by ImGui
#       ifdef IDC_HAND    // winuser.h defines it if(WINVER >= 0x0500)
        IDC_HAND,         //ImGuiMouseCursor_Hand // Unused by ImGui
#       else
        IDC_ARROW,         //ImGuiMouseCursor_Hand // Unused by ImGui
#       endif
        IDC_ARROW,         //,ImGuiMouseCursor_Arrow
        IDC_ARROW
    };
    static HCURSOR win32Cursors[ImGuiMouseCursor_COUNT+1];
#       endif //IMGUI_USE_WIN32_CURSORS
#   else //_WIN32
#       define IMGUI_USE_X11_CURSORS      // Optional (feel free to comment it out if you don't have X11)
#       ifdef IMGUI_USE_X11_CURSORS
    // Before the inclusion of @ref glfw3native.h, you must define exactly one
    // window API macro and exactly one context API macro.
#           define GLFW_EXPOSE_NATIVE_X11
#           define GLFW_EXPOSE_NATIVE_GLX
#           include <GLFW/glfw3native.h>        // GLFWAPI Display* glfwGetX11Display(void); //GLFWAPI Window glfwGetX11Window(GLFWwindow* window);
#           include <X11/cursorfont.h>
    // 52 (closed hand)   58 (hand with forefinger) 124 (spray)   86  (pencil)  150 (wait)
    static const unsigned int x11CursorIds[] = {
        2,//SDL_SYSTEM_CURSOR_ARROW,
        152,//SDL_SYSTEM_CURSOR_IBEAM,
        30,//SDL_SYSTEM_CURSOR_SIZEALL,      //SDL_SYSTEM_CURSOR_HAND,    // or SDL_SYSTEM_CURSOR_SIZEALL  //ImGuiMouseCursor_Move,                  // Unused by ImGui
        116,//42,//SDL_SYSTEM_CURSOR_SIZENS,       //ImGuiMouseCursor_ResizeNS,              // Unused by ImGui
        108,//SDL_SYSTEM_CURSOR_SIZEWE,       //ImGuiMouseCursor_ResizeEW,              // Unused by ImGui
        13,//SDL_SYSTEM_CURSOR_SIZENESW,     //ImGuiMouseCursor_ResizeNESW,
        15,//SDL_SYSTEM_CURSOR_SIZENWSE,     //ImGuiMouseCursor_ResizeNWSE,          // Unused by ImGui
        58,                                 //ImGuiMouseCursor_Hand
        2,//SDL_SYSTEM_CURSOR_ARROW         //,ImGuiMouseCursor_Arrow
        2//SDL_SYSTEM_CURSOR_ARROW
    };
    static Cursor x11Cursors[ImGuiMouseCursor_COUNT+1];
#       endif //IMGUI_USE_X11_CURSORS
#   endif //_WIN32
#endif //IMGUI_GLFW_NO_NATIVE_CURSORS
#endif //GLFW_HAS_MOUSE_CURSOR_SUPPORT




//static
GLFWwindow* window;
//static bool g_MousePressed[5] = {false, false, false, false, false };
static ImVec2 mousePosScale(1.0f, 1.0f);


// NB: ImGui already provide OS clipboard support for Windows so this isn't needed if you are using Windows only.
static const char* ImImpl_GetClipboardTextFn(void*)
{
    return glfwGetClipboardString(window);
}

static void ImImpl_SetClipboardTextFn(void*,const char* text)
{
    glfwSetClipboardString(window, text);
}

#ifdef _WIN32
// Notify OS Input Method Editor of text input position (e.g. when using Japanese/Chinese inputs, otherwise this isn't needed)
static void ImImpl_ImeSetInputScreenPosFn(int x, int y)
{
    HWND hwnd = glfwGetWin32Window(window);
    if (HIMC himc = ImmGetContext(hwnd))
    {
        COMPOSITIONFORM cf;
        cf.ptCurrentPos.x = x;
        cf.ptCurrentPos.y = y;
        cf.dwStyle = CFS_FORCE_POSITION;
        ImmSetCompositionWindow(himc, &cf);
    }
}
#endif

// Helper functions
static inline ImGuiKey glew_key_to_ImGuiKey(int key)  {
    // cloned from imgui/backends/imgui_impl_glfw.cpp
    switch (key)    {
        case GLFW_KEY_TAB: return ImGuiKey_Tab;
        case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
        case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
        case GLFW_KEY_UP: return ImGuiKey_UpArrow;
        case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
        case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
        case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
        case GLFW_KEY_HOME: return ImGuiKey_Home;
        case GLFW_KEY_END: return ImGuiKey_End;
        case GLFW_KEY_INSERT: return ImGuiKey_Insert;
        case GLFW_KEY_DELETE: return ImGuiKey_Delete;
        case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case GLFW_KEY_SPACE: return ImGuiKey_Space;
        case GLFW_KEY_ENTER: return ImGuiKey_Enter;
        case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
        case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
        case GLFW_KEY_COMMA: return ImGuiKey_Comma;
        case GLFW_KEY_MINUS: return ImGuiKey_Minus;
        case GLFW_KEY_PERIOD: return ImGuiKey_Period;
        case GLFW_KEY_SLASH: return ImGuiKey_Slash;
        case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
        case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
        case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
        case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
        case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
        case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
        case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
        case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
        case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
        case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
        case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
        case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
        case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
        case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
        case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
        case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
        case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
        case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
        case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
        case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
        case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
        case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
        case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
        case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
        case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
        case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
        case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
        case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
        case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
        case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
        case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
        case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
        case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
        case GLFW_KEY_MENU: return ImGuiKey_Menu;
        case GLFW_KEY_0: return ImGuiKey_0;
        case GLFW_KEY_1: return ImGuiKey_1;
        case GLFW_KEY_2: return ImGuiKey_2;
        case GLFW_KEY_3: return ImGuiKey_3;
        case GLFW_KEY_4: return ImGuiKey_4;
        case GLFW_KEY_5: return ImGuiKey_5;
        case GLFW_KEY_6: return ImGuiKey_6;
        case GLFW_KEY_7: return ImGuiKey_7;
        case GLFW_KEY_8: return ImGuiKey_8;
        case GLFW_KEY_9: return ImGuiKey_9;
        case GLFW_KEY_A: return ImGuiKey_A;
        case GLFW_KEY_B: return ImGuiKey_B;
        case GLFW_KEY_C: return ImGuiKey_C;
        case GLFW_KEY_D: return ImGuiKey_D;
        case GLFW_KEY_E: return ImGuiKey_E;
        case GLFW_KEY_F: return ImGuiKey_F;
        case GLFW_KEY_G: return ImGuiKey_G;
        case GLFW_KEY_H: return ImGuiKey_H;
        case GLFW_KEY_I: return ImGuiKey_I;
        case GLFW_KEY_J: return ImGuiKey_J;
        case GLFW_KEY_K: return ImGuiKey_K;
        case GLFW_KEY_L: return ImGuiKey_L;
        case GLFW_KEY_M: return ImGuiKey_M;
        case GLFW_KEY_N: return ImGuiKey_N;
        case GLFW_KEY_O: return ImGuiKey_O;
        case GLFW_KEY_P: return ImGuiKey_P;
        case GLFW_KEY_Q: return ImGuiKey_Q;
        case GLFW_KEY_R: return ImGuiKey_R;
        case GLFW_KEY_S: return ImGuiKey_S;
        case GLFW_KEY_T: return ImGuiKey_T;
        case GLFW_KEY_U: return ImGuiKey_U;
        case GLFW_KEY_V: return ImGuiKey_V;
        case GLFW_KEY_W: return ImGuiKey_W;
        case GLFW_KEY_X: return ImGuiKey_X;
        case GLFW_KEY_Y: return ImGuiKey_Y;
        case GLFW_KEY_Z: return ImGuiKey_Z;
        case GLFW_KEY_F1: return ImGuiKey_F1;
        case GLFW_KEY_F2: return ImGuiKey_F2;
        case GLFW_KEY_F3: return ImGuiKey_F3;
        case GLFW_KEY_F4: return ImGuiKey_F4;
        case GLFW_KEY_F5: return ImGuiKey_F5;
        case GLFW_KEY_F6: return ImGuiKey_F6;
        case GLFW_KEY_F7: return ImGuiKey_F7;
        case GLFW_KEY_F8: return ImGuiKey_F8;
        case GLFW_KEY_F9: return ImGuiKey_F9;
        case GLFW_KEY_F10: return ImGuiKey_F10;
        case GLFW_KEY_F11: return ImGuiKey_F11;
        case GLFW_KEY_F12: return ImGuiKey_F12;
        default: return ImGuiKey_None;
    }
}
/*static inline void glfw_window_update_ImGuiKeyModFlags(int mods)   {
    // cloned from imgui/backends/imgui_impl_glfw.cpp
    ImGuiIO& io = ImGui::GetIO();
    ImGuiKeyModFlags key_mods =
        ((mods & GLFW_MOD_CONTROL) ? ImGuiKeyModFlags_Ctrl : 0) |
        ((mods & GLFW_MOD_SHIFT) ? ImGuiKeyModFlags_Shift : 0) |
        ((mods & GLFW_MOD_ALT) ? ImGuiKeyModFlags_Alt : 0) |
        ((mods & GLFW_MOD_SUPER) ? ImGuiKeyModFlags_Super : 0);
    io.AddKeyModsEvent(key_mods);
}*/
static inline void glfw_window_update_ImGuiKeyModFlags()   {
    // cloned from imgui/backends/imgui_impl_glfw.cpp
    IM_ASSERT(window);
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiKey_ModCtrl,((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))?true:false);
    io.AddKeyEvent(ImGuiKey_ModShift,((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))?true:false);
    io.AddKeyEvent(ImGuiKey_ModAlt,((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS))?true:false);
    io.AddKeyEvent(ImGuiKey_ModSuper,((glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS))?true:false);
}
static inline int glfw_translate_untranslated_key(int key, int scancode)   {
// cloned from imgui/backends/imgui_impl_glfw.cpp
#define GLFW_HAS_GET_KEY_NAME   (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200)  // 3.2+ glfwGetKeyName()
#if GLFW_HAS_GET_KEY_NAME && !defined(__EMSCRIPTEN__)
    // GLFW 3.1+ attempts to "untranslate" keys, which goes the opposite of what every other framework does, making using lettered shortcuts difficult.
    // (It had reasons to do so: namely GLFW is/was more likely to be used for WASD-type game controls rather than lettered shortcuts, but IHMO the 3.1 change could have been done differently)
    // See https://github.com/glfw/glfw/issues/1502 for details.
    // Adding a workaround to undo this (so our keys are translated->untranslated->translated, likely a lossy process).
    // This won't cover edge cases but this is at least going to cover common cases.
    if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_EQUAL)   return key;
    const char* key_name = glfwGetKeyName(key, scancode);
    if (key_name && key_name[0] != 0 && key_name[1] == 0)    {
        const char char_names[] = "`-=[]\\,;\'./";
        const int char_keys[] = { GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_MINUS, GLFW_KEY_EQUAL, GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_COMMA, GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE, GLFW_KEY_PERIOD, GLFW_KEY_SLASH, 0 };
        IM_ASSERT(IM_ARRAYSIZE(char_names) == IM_ARRAYSIZE(char_keys));
        if (key_name[0] >= '0' && key_name[0] <= '9')               { key = GLFW_KEY_0 + (key_name[0] - '0'); }
        else if (key_name[0] >= 'A' && key_name[0] <= 'Z')          { key = GLFW_KEY_A + (key_name[0] - 'A'); }
        else if (const char* p = strchr(char_names, key_name[0]))   { key = char_keys[p - char_names]; }
    }
    // if (action == GLFW_PRESS) printf("key %d scancode %d name '%s'\n", key, scancode, key_name);
#else
    IM_UNUSED(scancode);
#endif
    return key;
}

// GLFW callbacks to get events
static void glfw_error_callback(int /*error*/, const char* description)	{
    fputs(description, stderr);
}
static bool gImGuiAppIsIconified = false;
static void glfw_window_iconify_callback(GLFWwindow* /*window*/,int iconified)    {
    gImGuiAppIsIconified = iconified == GL_TRUE;
}
static void glfw_framebuffer_size_callback(GLFWwindow* /*window*/,int fb_w,int fb_h)  {
    int w, h;glfwGetWindowSize(window, &w, &h);
    mousePosScale.x = (float)fb_w / w;                  // Some screens e.g. Retina display have framebuffer size != from window size, and mouse inputs are given in window/screen coordinates.
    mousePosScale.y = (float)fb_h / h;

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)fb_w, (float)fb_h);
}
static void glfw_window_size_callback(GLFWwindow* /*window*/,int w,int h)  {
    int fb_w, fb_h;glfwGetFramebufferSize(window, &fb_w, &fb_h);
    mousePosScale.x = (float)fb_w / w;                  // Some screens e.g. Retina display have framebuffer size != from window size, and mouse inputs are given in window/screen coordinates.
    mousePosScale.y = (float)fb_h / h;
    ResizeGL(w,h);
}
static void glfw_mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int mods)	{
    glfw_window_update_ImGuiKeyModFlags(/*mods*/);  // on my system, if I pass 'mods' here, I get wrong results...
                                                    // (I've commented out the overload that takes 'mods' in the code above)
    ImGuiIO& io = ImGui::GetIO();
    if (button >= 0 && button < ImGuiMouseButton_COUNT)
        io.AddMouseButtonEvent(button, action == GLFW_PRESS);
    // Single clicks are handled once per frame in 'ImImplMainLoopFrame(...)'
    // Handling double-clicks manually is deprecated AFAICS
    /* ImGuiIO& io = ImGui::GetIO();
        if (button >= 0 && button < 5) {
        //io.MouseDown[button] = (action == GLFW_PRESS);
        // Manual double click handling:
        static double dblClickTimes[6]={-FLT_MAX,-FLT_MAX,-FLT_MAX,-FLT_MAX,-FLT_MAX,-FLT_MAX};  // seconds
        if (action == GLFW_PRESS)   {
            //g_MousePressed[button] = true;
            double time = glfwGetTime();
            double& oldTime = dblClickTimes[button];
            bool& mouseDoubleClicked = gImGuiBindingMouseDblClicked[button];
            if (time - oldTime < io.MouseDoubleClickTime) {
                mouseDoubleClicked = true;
                oldTime = -FLT_MAX;
                //fprintf(stderr,"Double Clicked button %d\n",button);
            }
            else {
                //fprintf(stderr,"Not Double Clicked button %d (%1.4f < %1.4f)\n",button,(float)time-(float)oldTime,io.MouseDoubleClickTime);
                mouseDoubleClicked = false;
                oldTime = time;
            }
        }
    }*/
    IM_UNUSED(button);IM_UNUSED(action);IM_UNUSED(mods);
}
static void glfw_scroll_callback(GLFWwindow* /*window*/, double xoffset, double yoffset)	{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent((float)xoffset, (float)yoffset);
}

static void glfw_key_callback(GLFWwindow* /*window*/, int key, int scancode, int action, int /*mods*/)	{
    if (action != GLFW_PRESS && action != GLFW_RELEASE) return;
    glfw_window_update_ImGuiKeyModFlags(/*mods*/);  // on my system, if I pass 'mods' here, I get wrong results...
                                                    // (I've commented out the overload that takes 'mods' in the code above)

    const bool down = (action==GLFW_PRESS);
    key = glfw_translate_untranslated_key(key,scancode);

    ImGuiIO& io = ImGui::GetIO();
    ImGuiKey imgui_key = glew_key_to_ImGuiKey(key);
    io.AddKeyEvent(imgui_key, down);

    // We don't support this
    //io.SetKeyEventNativeData(imgui_key, key, scancode); // To support legacy indexing (<1.87 user code)

    //fprintf(stderr,"glfw_key_callback: '%i' (%s) %s\n",imgui_key,ImGui::GetKeyName(imgui_key),down?"[down]":"[up]");
}
static void glfw_char_callback(GLFWwindow* /*window*/, unsigned int c)	{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(c);
    //if (c > 0 /*&& c < 0x10000*/ && !ImGui::GetIO().KeyCtrl) ImGui::GetIO().AddInputCharacter((unsigned int)c);
}
static void glfw_mouse_enter_leave_callback(GLFWwindow* /*window*/, int entered)	{
    if (entered==GL_FALSE) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
    }
}
static void glfw_mouse_move_callback(GLFWwindow* /*window*/, double x,double y)	{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)x * mousePosScale.x, (float)y * mousePosScale.y);
}
static void InitImGui(const ImImpl_InitParams* pOptionalInitParams=NULL)	{
    int w, h;
    int fb_w, fb_h;
    glfwGetWindowSize(window, &w, &h);
    glfwGetFramebufferSize(window, &fb_w, &fb_h);
    mousePosScale.x = (float)fb_w / w;                  // Some screens e.g. Retina display have framebuffer size != from window size, and mouse inputs are given in window/screen coordinates.
    mousePosScale.y = (float)fb_h / h;

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)fb_w, (float)fb_h);  // Display size, in pixels. For clamping windows positions.
    io.DeltaTime = 1.0f/60.0f;                          // Time elapsed since last frame, in seconds (in this sample app we'll override this every frame because our timestep is variable)
    //io.PixelCenterOffset = 0.0f;                        // Align OpenGL texels

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;   // We can honor GetMouseCursor() values
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;    // We can honor io.WantSetMousePos requests (optional, rarely used)

    // Set up ImGui

    // Transition Guide:
    // IsKeyPressed(MY_NATIVE_KEY_XXX) -> use IsKeyPressed(ImGuiKey_XXX)
    // IsKeyPressed(GetKeyIndex(ImGuiKey_XXX)) -> use IsKeyPressed(ImGuiKey_XXX)
    // Backend writing to io.KeyMap[],KeysDown[] -> backend should call io.AddKeyEvent(), if legacy indexing is desired, call io.SetKeyEventNativeData()

    io.BackendUsingLegacyKeyArrays = 0; // Not sure if user must set this...

    //io.RenderDrawListsFn = ImImpl_RenderDrawLists;
    io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
    io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;
#ifdef _WIN32
    io.ImeSetInputScreenPosFn = ImImpl_ImeSetInputScreenPosFn;
#endif

    // 3 common init steps
    InitImGuiFontTexture(pOptionalInitParams);
    InitImGuiProgram();
    InitImGuiBuffer();
}


static bool InitBinding(const ImImpl_InitParams* pOptionalInitParams=NULL,int argc=0, char** argv=NULL)	{
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())    {
        fprintf(stderr, "Could not call glfwInit(...) successfully.\n");
        return false;
    }

//-ENABLE-OPENGLES COMPATIBILITY PROFILES----------------------------------
#ifndef IMIMPL_SHADER_NONE
#ifdef IMIMPL_SHADER_GLES
#   ifndef IMIMPL_SHADER_GL3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);          // 1 => ES1.1   2 => ES2.0  3 => ES3.0
#   else //IMIMPL_SHADER_GL3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
#   endif //MIMPL_SHADER_GL3
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#endif // IMIMPL_SHADER_GLES
#endif //IMIMPL_SHADER_NONE
//--------------------------------------------------------------------------
    //glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

#ifdef IMIMPL_FORCE_DEBUG_CONTEXT
glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GL_TRUE);
#endif //IMIMPL_FORCE_DEBUG_CONTEXT

/*
//-ENABLE-OPENGLES COMPATIBILITY PROFILES----------------------------------
#ifndef IMIMPL_SHADER_NONE
#ifdef IMIMPL_SHADER_GLES
#   ifndef IMIMPL_SHADER_GL3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);          // 1 => ES1.1   2 => ES2.0  3 => ES3.0
#   ifdef IMIMPL_SHADER_GLES_MINOR_VERSION
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, IMIMPL_SHADER_GLES_MINOR_VERSION);
#   endif //IMIMPL_SHADER_GLES_MINOR_VERSION
#   else //IMIMPL_SHADER_GL3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
#   ifdef IMIMPL_SHADER_GLES_MINOR_VERSION
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, IMIMPL_SHADER_GLES_MINOR_VERSION);
#   endif //IMIMPL_SHADER_GLES_MINOR_VERSION
#   endif //MIMPL_SHADER_GL3
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#endif // IMIMPL_SHADER_GLES
#endif //IMIMPL_SHADER_NONE

#ifndef IMIMPL_SHADER_GLES
#ifdef IMIMPL_FORWARD_COMPAT
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif
#ifdef IMIMPL_CORE_PROFILE
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#endif
#endif

#ifdef IMIMPL_SRGB_CAPABLE
    glfwWindowHint(GLFW_SRGB_CAPABLE,GL_TRUE);
#endif
if (pOptionalInitParams && pOptionalInitParams->useOpenGLDebugContext) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GL_TRUE);
//--------------------------------------------------------------------------
*/

    if (pOptionalInitParams && pOptionalInitParams->gWindowTitle[0]!='\0')  window = glfwCreateWindow(pOptionalInitParams ? pOptionalInitParams->gWindowSize.x : 1270, pOptionalInitParams ? pOptionalInitParams->gWindowSize.y : 720,(const char*) &pOptionalInitParams->gWindowTitle[0], NULL, NULL);
    else		window = glfwCreateWindow(pOptionalInitParams ? pOptionalInitParams->gWindowSize.x : 1270, pOptionalInitParams ? pOptionalInitParams->gWindowSize.y : 720, "Dear ImGui Glfw3 OpenGL example", NULL, NULL);
    if (!window)    {
        fprintf(stderr, "Could not call glfwCreateWindow(...) successfully.\n");
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
    //glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
    glfwSetScrollCallback(window, glfw_scroll_callback);
    glfwSetCharCallback(window, glfw_char_callback);
    glfwSetCursorPosCallback(window, glfw_mouse_move_callback);
    glfwSetCursorEnterCallback(window, glfw_mouse_enter_leave_callback);

    glfwSetWindowSizeCallback(window, glfw_window_size_callback);
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
    glfwSetWindowIconifyCallback(window, glfw_window_iconify_callback);

#ifdef IMGUI_USE_GLAD
   if(!gladLoadGL()) {
        fprintf(stderr,"Error initializing GLAD!\n");
        return false;
    }
    // gladLoadGLLoader(&glfwGetProcAddress);
#endif //IMGUI_USE_GLAD
#ifdef IMGUI_USE_GL3W
   if (gl3wInit()) {
       fprintf(stderr, "Error initializing GL3W!\n");
       return false;
   }
#endif //IMGUI_USE_GL3W

        //OpenGL info
    {
        printf("GLFW Version: %d.%d.%d\n",GLFW_VERSION_MAJOR,GLFW_VERSION_MINOR,GLFW_VERSION_REVISION);
        printf("GL Vendor: %s\n", glGetString( GL_VENDOR ));
        printf("GL Renderer : %s\n", glGetString( GL_RENDERER ));
        printf("GL Version (string) : %s\n",  glGetString( GL_VERSION ));
#       ifndef IMIMPL_SHADER_NONE
        printf("GLSL Version : %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ));
#       endif //IMIMPL_SHADER_NONE
        //printf("GL Extensions:\n%s\n",(char *) glGetString(GL_EXTENSIONS));
    }

#ifdef IMGUI_USE_GLEW
    GLenum err = glewInit();
    if( GLEW_OK != err )
    {
        fprintf(stderr, "Error initializing GLEW: %s\n",
                glewGetErrorString(err) );
        return false;
    }
#endif //IMGUI_USE_GLEW

#ifdef IMIMPL_FORCE_DEBUG_CONTEXT
    glDebugMessageCallback(GLDebugMessageCallback,NULL);    // last is userParam
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);   // use glDisable(GL_DEBUG_OUTPUT); at runtime to disable it

    // Test:
    //glEnable(GL_DEPTH); // instead of glEnable(GL_DEPTH_TEST); => Produces: GL_INVALID_ENUM error generated. <cap> enum is invalid; expected GL_ALPHA_TEST, GL_BLEND, GL_COLOR_MATERIAL, GL_CULL_FACE, GL_DEPTH_TEST, GL_DITHER, GL_FOG, etc. (136 others).
#endif //IMIMPL_FORCE_DEBUG_CONTEXT

	return true;
}

struct ImImplMainLoopFrameStruct {
int done;
#ifndef GLFW_HAS_MOUSE_CURSOR_SUPPORT
#ifndef IMGUI_GLFW_NO_NATIVE_CURSORS
#if (!defined(IMGUI_USE_WIN32_CURSORS) && defined(IMGUI_USE_X11_CURSORS))
Display* x11Display;
Window x11Window;
#endif //IMGUI_USE_CURSORS
#endif //IMGUI_GLFW_NO_NATIVE_CURSORS
#endif //GLFW_HAS_MOUSE_CURSOR_SUPPORT
ImImplMainLoopFrameStruct() : done(false) {}
};


// Update gamepad inputs
static void glfw_Update_gamepads()   {
#   define GLFW_HAS_GAMEPAD_API    (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetGamepadState() new api
#   define IMIMPL_SATURATE(V)   ((V)<0.0f?0.0f:(V)>1.0f?1.0f:(V))
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)  return;

    io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;

    static bool gEmulateGamepadWithKeyboard =   // Intended mainly for __EMSCRIPTEN__ to replace NavEnableKeyboard, that does not work on browsers
            false;
            //true;

    if (!gEmulateGamepadWithKeyboard)   {
#       if GLFW_HAS_GAMEPAD_API
            GLFWgamepadstate gamepad;
            if (!glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepad))    return;
#           define MAP_BUTTON(KEY_NO, BUTTON_NO, _UNUSED)          do { io.AddKeyEvent(KEY_NO, gamepad.buttons[BUTTON_NO] != 0); } while (0)
#           define MAP_ANALOG(KEY_NO, AXIS_NO, _UNUSED, V0, V1)    do { float v = gamepad.axes[AXIS_NO]; v = (v - V0) / (V1 - V0); io.AddKeyAnalogEvent(KEY_NO, v > 0.10f, IMIMPL_SATURATE(v)); } while (0)
#       else
            int axes_count = 0, buttons_count = 0;
            const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
            const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
            if (axes_count == 0 || buttons_count == 0)  return;
#           define MAP_BUTTON(KEY_NO, _UNUSED, BUTTON_NO)          do { io.AddKeyEvent(KEY_NO, (buttons_count > BUTTON_NO && buttons[BUTTON_NO] == GLFW_PRESS)); } while (0)
#           define MAP_ANALOG(KEY_NO, _UNUSED, AXIS_NO, V0, V1)    do { float v = (axes_count > AXIS_NO) ? axes[AXIS_NO] : V0; v = (v - V0) / (V1 - V0); io.AddKeyAnalogEvent(KEY_NO, v > 0.10f, IMIMPL_SATURATE(v)); } while (0)
#       endif
        io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
        MAP_BUTTON(ImGuiKey_GamepadStart,       GLFW_GAMEPAD_BUTTON_START,          7);
        MAP_BUTTON(ImGuiKey_GamepadBack,        GLFW_GAMEPAD_BUTTON_BACK,           6);
        MAP_BUTTON(ImGuiKey_GamepadFaceDown,    GLFW_GAMEPAD_BUTTON_A,              0);     // Xbox A, PS Cross
        MAP_BUTTON(ImGuiKey_GamepadFaceRight,   GLFW_GAMEPAD_BUTTON_B,              1);     // Xbox B, PS Circle
        MAP_BUTTON(ImGuiKey_GamepadFaceLeft,    GLFW_GAMEPAD_BUTTON_X,              2);     // Xbox X, PS Square
        MAP_BUTTON(ImGuiKey_GamepadFaceUp,      GLFW_GAMEPAD_BUTTON_Y,              3);     // Xbox Y, PS Triangle
        MAP_BUTTON(ImGuiKey_GamepadDpadLeft,    GLFW_GAMEPAD_BUTTON_DPAD_LEFT,      13);
        MAP_BUTTON(ImGuiKey_GamepadDpadRight,   GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,     11);
        MAP_BUTTON(ImGuiKey_GamepadDpadUp,      GLFW_GAMEPAD_BUTTON_DPAD_UP,        10);
        MAP_BUTTON(ImGuiKey_GamepadDpadDown,    GLFW_GAMEPAD_BUTTON_DPAD_DOWN,      12);
        MAP_BUTTON(ImGuiKey_GamepadL1,          GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,    4);
        MAP_BUTTON(ImGuiKey_GamepadR1,          GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,   5);
        MAP_ANALOG(ImGuiKey_GamepadL2,          GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,     4,      -0.75f,  +1.0f);
        MAP_ANALOG(ImGuiKey_GamepadR2,          GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,    5,      -0.75f,  +1.0f);
        MAP_BUTTON(ImGuiKey_GamepadL3,          GLFW_GAMEPAD_BUTTON_LEFT_THUMB,     8);
        MAP_BUTTON(ImGuiKey_GamepadR3,          GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,    9);
        MAP_ANALOG(ImGuiKey_GamepadLStickLeft,  GLFW_GAMEPAD_AXIS_LEFT_X,           0,      -0.25f,  -1.0f);
        MAP_ANALOG(ImGuiKey_GamepadLStickRight, GLFW_GAMEPAD_AXIS_LEFT_X,           0,      +0.25f,  +1.0f);
        MAP_ANALOG(ImGuiKey_GamepadLStickUp,    GLFW_GAMEPAD_AXIS_LEFT_Y,           1,      -0.25f,  -1.0f);
        MAP_ANALOG(ImGuiKey_GamepadLStickDown,  GLFW_GAMEPAD_AXIS_LEFT_Y,           1,      +0.25f,  +1.0f);
        MAP_ANALOG(ImGuiKey_GamepadRStickLeft,  GLFW_GAMEPAD_AXIS_RIGHT_X,          2,      -0.25f,  -1.0f);
        MAP_ANALOG(ImGuiKey_GamepadRStickRight, GLFW_GAMEPAD_AXIS_RIGHT_X,          2,      +0.25f,  +1.0f);
        MAP_ANALOG(ImGuiKey_GamepadRStickUp,    GLFW_GAMEPAD_AXIS_RIGHT_Y,          3,      -0.25f,  -1.0f);
        MAP_ANALOG(ImGuiKey_GamepadRStickDown,  GLFW_GAMEPAD_AXIS_RIGHT_Y,          3,      +0.25f,  +1.0f);
#          undef MAP_BUTTON
#          undef MAP_ANALOG
    }
    else // gEmulateGamepadWithKeyboard
    {
        // gEmulateGamepadWithKeyboard
        float analogStep = 0.1f; // Bad stuff
        if (io.Framerate>65.f) analogStep*= 60.f/io.Framerate;    // Never tested
#       define MAP_BUTTON_PRESSED(NAV_NO, IMGUI_KEY_NO, MOD_BOOL)       do { io.AddKeyEvent(NAV_NO, (MOD_BOOL && ImGui::IsKeyPressed(IMGUI_KEY_NO)) ? 1.0f : 0.f); } while (0)
#       define MAP_BUTTON_DOWN(NAV_NO, GLFW_KEY_NO, MOD_BOOL)           do { io.AddKeyEvent(NAV_NO, (MOD_BOOL && glfwGetKey(window,GLFW_KEY_NO)==GLFW_PRESS) ? 1.0f : 0.f); } while (0)
#       define MAP_ANALOG(NAV_NO, GLFW_KEY_NO,MOD_BOOL,MOD_BOOL_HALF)   do { float v =((MOD_BOOL && glfwGetKey(window,GLFW_KEY_NO)==GLFW_PRESS) ? analogStep : 0.f)*(MOD_BOOL_HALF?0.5f:1.f); io.AddKeyAnalogEvent(NAV_NO, v > 0.10f, IMIMPL_SATURATE(v)); } while (0)
//#     define MAP_ANALOG(NAV_NO, GLFW_KEY_NO,MOD_BOOL,MOD_BOOL_HALF) {float v =((MOD_BOOL && glfwGetKey(window,GLFW_KEY_NO)==GLFW_PRESS) ? analogStep : 0.f)*(MOD_BOOL_HALF?0.5f:1.f); if (v > 1.0f) v = 1.0f; if (io.NavInputs[NAV_NO] < v) io.NavInputs[NAV_NO] = v;}

        const bool noMods = (!io.KeyCtrl && !io.KeyShift);

        // These work.
        MAP_BUTTON_PRESSED(ImGuiNavInput_Activate,  ImGuiKey_Space,      noMods);
        MAP_BUTTON_PRESSED(ImGuiNavInput_Cancel,    ImGuiKey_Delete,     noMods);
        MAP_BUTTON_PRESSED(ImGuiNavInput_Input,     ImGuiKey_Enter,      noMods);
        MAP_BUTTON_DOWN(ImGuiNavInput_Menu,      GLFW_KEY_KP_0,    noMods);

        MAP_BUTTON_DOWN(ImGuiNavInput_DpadLeft, GLFW_KEY_LEFT,   noMods);
        MAP_BUTTON_DOWN(ImGuiNavInput_DpadRight,GLFW_KEY_RIGHT,  noMods);
        MAP_BUTTON_DOWN(ImGuiNavInput_DpadUp,   GLFW_KEY_UP,     noMods);
        MAP_BUTTON_DOWN(ImGuiNavInput_DpadDown, GLFW_KEY_DOWN,   noMods);

        MAP_BUTTON_DOWN(ImGuiNavInput_FocusPrev, GLFW_KEY_KP_7,     noMods);
        MAP_BUTTON_DOWN(ImGuiNavInput_FocusNext, GLFW_KEY_KP_9,     noMods);
        MAP_BUTTON_DOWN(ImGuiNavInput_TweakSlow, GLFW_KEY_KP_7,     noMods);
        MAP_BUTTON_DOWN(ImGuiNavInput_TweakFast, GLFW_KEY_KP_9,     noMods);

        // These work (partially: io.KeyCtrl does nothing and there's the intimidating 'analogStep' above).
        MAP_ANALOG(ImGuiNavInput_LStickLeft,    GLFW_KEY_KP_4,  noMods,  io.KeyCtrl);
        MAP_ANALOG(ImGuiNavInput_LStickRight,   GLFW_KEY_KP_6,  noMods,  io.KeyCtrl);
        MAP_ANALOG(ImGuiNavInput_LStickUp,      GLFW_KEY_KP_8,  noMods,  io.KeyCtrl);
        MAP_ANALOG(ImGuiNavInput_LStickDown,    GLFW_KEY_KP_2,  noMods,  io.KeyCtrl);

#       undef MAP_BUTTON_PRESSED
#       undef MAP_BUTTON_DOWN
#       undef MAP_ANALOG
        io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    }
#   undef IMIMPL_SATURATE
}



static void ImImplMainLoopFrame(void* userPtr)	{
    ImImplMainLoopFrameStruct& mainLoopFrameStruct = *((ImImplMainLoopFrameStruct*) userPtr);

    static double time = 0.0;
    ImGuiIO& io = ImGui::GetIO();

    //for (size_t i = 0; i < 5; i++) gImGuiBindingMouseDblClicked[i] = false;   // We manually set it (otherwise it won't work with low frame rates)
    memset(io.NavInputs, 0, sizeof(io.NavInputs));
    if (!gImGuiPaused)	{
        static ImGuiMouseCursor oldCursor = ImGuiMouseCursor_Arrow;
        static bool oldMustHideCursor = io.MouseDrawCursor;
        if (oldMustHideCursor!=io.MouseDrawCursor) {
            glfwSetInputMode(window, GLFW_CURSOR, io.MouseDrawCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
            oldMustHideCursor = io.MouseDrawCursor;
            oldCursor = ImGuiMouseCursor_COUNT;
        }
        if (!io.MouseDrawCursor) {
            if (oldCursor!=ImGui::GetMouseCursor()) {
                oldCursor=ImGui::GetMouseCursor();
#               ifdef GLFW_HAS_MOUSE_CURSOR_SUPPORT
                glfwSetCursor(window,glfwCursors[oldCursor]);
#               else //GLFW_HAS_MOUSE_CURSOR_SUPPORT
#                   ifndef IMGUI_GLFW_NO_NATIVE_CURSORS
#                       ifdef IMGUI_USE_WIN32_CURSORS
                            SetCursor(win32Cursors[oldCursor]);           // If this does not work, it's bacause the native Window must be created with a NULL cursor (but how to tell glfw about it?)
#                       elif defined IMGUI_USE_X11_CURSORS
                            XDefineCursor(mainLoopFrameStruct.x11Display,mainLoopFrameStruct.x11Window,x11Cursors[oldCursor]);
#                       endif
#                   endif //IMGUI_GLFW_NO_NATIVE_CURSORS
#               endif //GLFW_HAS_MOUSE_CURSOR_SUPPORT
            }
        }
    }

    static const int numFramesDelay = 12;
    static int curFramesDelay = -1;
    // What about using: if (gImGuiAppIsIconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED)) instead of listening to glfw_window_iconify_callback ?
    // (some users suggest using !glfwGetWindowAttrib(window, GLFW_FOCUSED) instead..., some other: !glfwGetWindowAttrib(window, GLFW_VISIBLE) )
    if (gImGuiAppIsIconified || (gImGuiWereOutsideImGui ? (gImGuiInverseFPSClampOutsideImGui==0) : (gImGuiInverseFPSClampInsideImGui==0))) {
        //fprintf(stderr,"glfwWaitEvents() Start %1.4f\n",glfwGetTime());
        glfwWaitEvents();
        //fprintf(stderr,"glfwWaitEvents() End %1.4f\n",glfwGetTime());
    }
    else {
        glfwPollEvents();
        //----------------------------------------------------------------------
        // Update game controllers (if enabled and available)
        glfw_Update_gamepads();
        //------------------------------------------------------------------------
    }

    // Setup timestep
    const double current_time =  glfwGetTime();
    static float deltaTime = (float)(current_time -time);
    deltaTime = (float) (current_time - time);
    time = current_time;

    // Start the frame
    {
        io.DeltaTime = deltaTime;
        /*for (size_t i = 0; i < 5; i++) {
            io.MouseDown[i]= g_MousePressed[i] || glfwGetMouseButton(window, i); // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
            g_MousePressed[i] = false;
        }*/
        if (!gImGuiPaused) {
            if (io.WantSetMousePos)  {
                // Set mouse position if requested by io.WantSetMousePos flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
                glfwSetCursorPos(window, (double)io.MousePos.x, (double)io.MousePos.y);
            }
            /*else    {
                // Get mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
                double mouse_x, mouse_y;
                glfwGetCursorPos(window, &mouse_x, &mouse_y);
                io.AddMousePosEvent((float)mouse_x, (float)mouse_y);
            }*/

            ImGui::NewFrame();
        }
        else {
            ImImpl_NewFramePaused();    // Enables some ImGui queries regardless ImGui::NewFrame() not being called.
            gImGuiCapturesInput = false;
        }
        //for (size_t i = 0; i < 5; i++) io.MouseDoubleClicked[i]=gImGuiBindingMouseDblClicked[i];   // We manually set it (otherwise it won't work with low frame rates)
    }

    if (gImGuiPreDrawGLCallback) gImGuiPreDrawGLCallback();
    DrawGL();


    if (!gImGuiPaused)	{
        gImGuiWereOutsideImGui = !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemActive();
        const bool imguiNeedsInputNow = !gImGuiWereOutsideImGui && (io.WantTextInput || io.MouseDelta.x!=0 || io.MouseDelta.y!=0 || io.MouseWheel!=0  || io.MouseWheelH!=0);// || io.MouseDownOwned[0] || io.MouseDownOwned[1] || io.MouseDownOwned[2]);
        if (gImGuiCapturesInput != imguiNeedsInputNow) {
            gImGuiCapturesInput = imguiNeedsInputNow;
             if (gImGuiDynamicFPSInsideImGui) {
                if (!gImGuiCapturesInput && !gImGuiWereOutsideImGui) curFramesDelay = 0;
                else curFramesDelay = -1;                
            }            
        }
        if (gImGuiWereOutsideImGui) curFramesDelay = -1;
        //fprintf(stderr,"gImGuiCapturesInput=%s curFramesDelay=%d wereOutsideImGui=%s\n",gImGuiCapturesInput?"true":"false",curFramesDelay,wereOutsideImGui?"true":"false");

        // Rendering
        ImGui::Render();
        ImImpl_RenderDrawLists(ImGui::GetDrawData());
    }
    else {gImGuiWereOutsideImGui=true;curFramesDelay = -1;}

    if (gImGuiPreDrawGLSwapBuffersCallback) gImGuiPreDrawGLSwapBuffersCallback();
    glfwSwapBuffers(window);
    if (gImGuiPostDrawGLSwapBuffersCallback) gImGuiPostDrawGLSwapBuffersCallback();

    // Handle clamped FPS:
    if (curFramesDelay>=0 && ++curFramesDelay>numFramesDelay) WaitFor(200);     // 200 = 5 FPS - frame rate when ImGui is inactive
    else {
        const float& inverseFPSClamp = gImGuiWereOutsideImGui ? gImGuiInverseFPSClampOutsideImGui : gImGuiInverseFPSClampInsideImGui;
        if (inverseFPSClamp==0.f) WaitFor(500);
        // If needed we must wait (inverseFPSClamp-deltaTime) seconds (=> honestly I shouldn't add the * 2.0f factor at the end, but ImGui tells me the wrong FPS otherwise... why? <=)
        else if (inverseFPSClamp>0 && deltaTime < inverseFPSClamp)  WaitFor((unsigned int) ((inverseFPSClamp-deltaTime)*1000.f * 2.0f) );
    }

#	ifdef __EMSCRIPTEN__
    if ((mainLoopFrameStruct.done=!glfwWindowShouldClose(window))==0) emscripten_cancel_main_loop();
#   endif //__EMSCRIPTEN__
}


// Application code
int ImImpl_Main(const ImImpl_InitParams* pOptionalInitParams,int argc, char** argv)
{
    if (!InitBinding(pOptionalInitParams,argc,argv)) return -1;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    InitImGui(pOptionalInitParams);
    ImGuiIO& io = ImGui::GetIO();

    ImImplMainLoopFrameStruct mainLoopFrameStruct;
    // New: create cursors-------------------------------------------
#ifdef GLFW_HAS_MOUSE_CURSOR_SUPPORT
        IM_ASSERT(sizeof(glfwCursorIds)/sizeof(glfwCursorIds[0])==ImGuiMouseCursor_COUNT+1);
        int num_failures = 0;
        for (int i=0,isz=ImGuiMouseCursor_COUNT+1;i<isz;i++) {
            glfwCursors[i] = glfwCreateStandardCursor(glfwCursorIds[i]);
            if (glfwCursors[i]==NULL) {/*printf("Sorry, but your glfw version does not have standard cursor support for index: %d\n",i);fflush(stdout);*/++num_failures;}
            if (i==0) glfwSetCursor(window,glfwCursors[i]);
        }
        if (num_failures==ImGuiMouseCursor_COUNT+1) {
            printf("Sorry, but your glfw version has a broken standard cursor support [glfwCreateStandardCursor(...) always returns NULL].\nSkipping cursors.\n");
            fflush(stdout);
        }
#else //GLFW_HAS_MOUSE_CURSOR_SUPPORT
#   ifndef IMGUI_GLFW_NO_NATIVE_CURSORS
#       ifdef IMGUI_USE_WIN32_CURSORS
        IM_ASSERT(sizeof(win32CursorIds)/sizeof(win32CursorIds[0])==ImGuiMouseCursor_COUNT+1);
        for (int i=0,isz=ImGuiMouseCursor_COUNT+1;i<isz;i++) {
            win32Cursors[i] = LoadCursor(NULL,(LPCTSTR) win32CursorIds[i]);
            if (i==0) SetCursor(win32Cursors[i]);
        }
#       elif defined IMGUI_USE_X11_CURSORS
        IM_ASSERT(sizeof(x11CursorIds)/sizeof(x11CursorIds[0])==ImGuiMouseCursor_COUNT+1);
        mainLoopFrameStruct.x11Display = glfwGetX11Display();
        mainLoopFrameStruct.x11Window = glfwGetX11Window(window);
        //XColor white;white.red=white.green=white.blue=255;
        //XColor black;black.red=black.green=black.blue=0;
        for (int i=0,isz=ImGuiMouseCursor_COUNT+1;i<isz;i++) {
            x11Cursors[i] = XCreateFontCursor(mainLoopFrameStruct.x11Display,x11CursorIds[i]);
            //XRecolorCursor(x11Display, x11Cursors[i], &white,&black);
            if (i==0) XDefineCursor(mainLoopFrameStruct.x11Display,mainLoopFrameStruct.x11Window,x11Cursors[i]);
        }
#       endif
#   endif //IMGUI_GLFW_NO_NATIVE_CURSORS
#endif //GLFW_HAS_MOUSE_CURSOR_SUPPORT
    //---------------------------------------------------------------

    InitGL();
    if (gImGuiPostInitGLCallback) gImGuiPostInitGLCallback();
 	ResizeGL(io.DisplaySize.x,io.DisplaySize.y);
	
    gImGuiInverseFPSClampInsideImGui = pOptionalInitParams ? ((pOptionalInitParams->gFpsClampInsideImGui!=0) ? (1.0f/pOptionalInitParams->gFpsClampInsideImGui) : 1.0f) : -1.0f;
    gImGuiInverseFPSClampOutsideImGui = pOptionalInitParams ? ((pOptionalInitParams->gFpsClampOutsideImGui!=0) ? (1.0f/pOptionalInitParams->gFpsClampOutsideImGui) : 1.0f) : -1.0f;
    gImGuiDynamicFPSInsideImGui = pOptionalInitParams ? pOptionalInitParams->gFpsDynamicInsideImGui : false;

    mainLoopFrameStruct.done = 0;
#	ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(ImImplMainLoopFrame,&mainLoopFrameStruct, 0, 1);
#	else
    while ((mainLoopFrameStruct.done=!glfwWindowShouldClose(window)))   {
        ImImplMainLoopFrame((void*)&mainLoopFrameStruct);
    }
#	endif //__EMSCRIPTEN__


    DestroyGL();
    ImGui::DestroyContext();
    DestroyImGuiFontTexture();
    DestroyImGuiProgram();
    DestroyImGuiBuffer();

    // New: delete cursors-------------------------------------------
#   ifdef  GLFW_HAS_MOUSE_CURSOR_SUPPORT
    for (int i=0,isz=ImGuiMouseCursor_COUNT+1;i<isz;i++) {
        glfwDestroyCursor(glfwCursors[i]);
    }
#   else   //GLFW_HAS_MOUSE_CURSOR_SUPPORT
#       ifndef IMGUI_GLFW_NO_NATIVE_CURSORS
#           ifdef IMGUI_USE_WIN32_CURSORS
                // Nothing to do
#           elif defined IMGUI_USE_X11_CURSORS
                XUndefineCursor(mainLoopFrameStruct.x11Display,mainLoopFrameStruct.x11Window);
                for (int i=0,isz=ImGuiMouseCursor_COUNT+1;i<isz;i++) {
                    XFreeCursor(mainLoopFrameStruct.x11Display,x11Cursors[i]);
                }
#           endif
#       endif //IMGUI_GLFW_NO_NATIVE_CURSORS
#   endif //GLFW_HAS_MOUSE_CURSOR_SUPPORT
    //---------------------------------------------------------------

    glfwTerminate();
    return 0;
}

#endif //#ifndef IMIMPL_BINDING_H

