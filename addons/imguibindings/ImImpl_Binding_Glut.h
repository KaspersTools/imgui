#ifndef IMIMPL_BINDING_H
#define IMIMPL_BINDING_H

#include "imguibindings.h"

/*
#ifndef IMIMPL_GLUT_HAS_MOUSE_WHEEL_CALLBACK
#   ifdef _WIN32
#       define IMIMPL_GLUT_HAS_MOUSE_WHEEL_CALLBACK
#   endif //_WIN32
#endif //IMIMPL_GLUT_HAS_MOUSE_WHEEL_CALLBACK
*/

static ImVec2 mousePosScale(1.0f, 1.0f);
//static const int specialCharMapAddend = 128;    // to prevent some special chars from clashing into ImGui normal chars

// NB: ImGui already provide OS clipboard support for Windows so this isn't needed if you are using Windows only.
#ifndef _WIN32
// If somebody implements these, in InitImGui(...) these callbacks MUST be set (they're currently detached).
// The default fallback on non-Windows OS is a "private" clipboard.
static const char* ImImpl_GetClipboardTextFn(void*)
{
    //fprintf(stderr,"ImImpl_GetClipboardTextFn()\n");
    static const char* txt = "copy and paste not implemented in the glut backend!";
    //return SDL_GetClipboardText();
    return txt;
}
static void ImImpl_SetClipboardTextFn(void*,const char* /*text*/)
{
    //fprintf(stderr,"ImImpl_SetClipboardTextFn()\n");
    //SDL_SetClipboardText(text);
}
#endif //_WIN32

// TODO: once we can extract the HWND from glut...
/*
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
*/

static void GlutReshapeFunc(int w,int h)    {
    int fb_w, fb_h;
    fb_w = w;
    fb_h = h;
    //glfwGetFramebufferSize(window, &fb_w, &fb_h);
    mousePosScale.x = 1.f;//(float)fb_w / w;                  // Some screens e.g. Retina display have framebuffer size != from window size, and mouse inputs are given in window/screen coordinates.
    mousePosScale.y = 1.f;//(float)fb_h / h;

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)fb_w, (float)fb_h);  // Display size, in pixels. For clamping windows positions.

    ResizeGL(w,h);
}
static void GlutEntryFunc(int a)   {
    //fprintf(stderr,"GlutEntryFunc %d\n",a);
    if (a==0){
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent(-FLT_MAX,-FLT_MAX);
    }
}
static bool gImGuiAppIconized = false;  // stays always false (I'm not able to detect when the user minimizes a window)
/*
// This method gets never called on my system:
static void GlutVisibilityFunc(int a)   {
    fprintf(stderr,"GlutVisibilityFunc %d\n",a);
    gImGuiAppIconized = a == GL_TRUE;
}
// This gets called, but does not provide the info I'm looking for
static void GlutWindowStatusFunc(int a)   {
    fprintf(stderr,"GlutWindowStatusFunc %d\n",a);
    //gImGuiAppIconized = a == GL_TRUE;
}*/

// Glut helper functions
static ImGuiKey GlutKeyToImGuiKey(int key)  {
    // cloned from backends/imgui_impl_glut.cpp

    // Glut has 1 function for characters and one for "special keys". We map the characters in the 0..255 range and the keys above.
    switch (key)    {
        case '\t':                      return ImGuiKey_Tab;
        case 256 + GLUT_KEY_LEFT:       return ImGuiKey_LeftArrow;
        case 256 + GLUT_KEY_RIGHT:      return ImGuiKey_RightArrow;
        case 256 + GLUT_KEY_UP:         return ImGuiKey_UpArrow;
        case 256 + GLUT_KEY_DOWN:       return ImGuiKey_DownArrow;
        case 256 + GLUT_KEY_PAGE_UP:    return ImGuiKey_PageUp;
        case 256 + GLUT_KEY_PAGE_DOWN:  return ImGuiKey_PageDown;
        case 256 + GLUT_KEY_HOME:       return ImGuiKey_Home;
        case 256 + GLUT_KEY_END:        return ImGuiKey_End;
        case 256 + GLUT_KEY_INSERT:     return ImGuiKey_Insert;
        case 127:                       return ImGuiKey_Delete;
        case 8:                         return ImGuiKey_Backspace;
        case ' ':                       return ImGuiKey_Space;
        case 13:                        return ImGuiKey_Enter;
        case 27:                        return ImGuiKey_Escape;
        case 39:                        return ImGuiKey_Apostrophe;
        case 44:                        return ImGuiKey_Comma;
        case 45:                        return ImGuiKey_Minus;
        case 46:                        return ImGuiKey_Period;
        case 47:                        return ImGuiKey_Slash;
        case 59:                        return ImGuiKey_Semicolon;
        case 61:                        return ImGuiKey_Equal;
        case 91:                        return ImGuiKey_LeftBracket;
        case 92:                        return ImGuiKey_Backslash;
        case 93:                        return ImGuiKey_RightBracket;
        case 96:                        return ImGuiKey_GraveAccent;
        //case 0:                         return ImGuiKey_CapsLock;
        //case 0:                         return ImGuiKey_ScrollLock;
        case 256 + 0x006D:              return ImGuiKey_NumLock;
        //case 0:                         return ImGuiKey_PrintScreen;
        //case 0:                         return ImGuiKey_Pause;
        //case '0':                       return ImGuiKey_Keypad0;
        //case '1':                       return ImGuiKey_Keypad1;
        //case '2':                       return ImGuiKey_Keypad2;
        //case '3':                       return ImGuiKey_Keypad3;
        //case '4':                       return ImGuiKey_Keypad4;
        //case '5':                       return ImGuiKey_Keypad5;
        //case '6':                       return ImGuiKey_Keypad6;
        //case '7':                       return ImGuiKey_Keypad7;
        //case '8':                       return ImGuiKey_Keypad8;
        //case '9':                       return ImGuiKey_Keypad9;
        //case 46:                        return ImGuiKey_KeypadDecimal;
        //case 47:                        return ImGuiKey_KeypadDivide;
        case 42:                        return ImGuiKey_KeypadMultiply;
        //case 45:                        return ImGuiKey_KeypadSubtract;
        case 43:                        return ImGuiKey_KeypadAdd;
        //case 13:                        return ImGuiKey_KeypadEnter;
        //case 0:                         return ImGuiKey_KeypadEqual;
        case 256 + 0x0070:              return ImGuiKey_LeftShift;
        case 256 + 0x0072:              return ImGuiKey_LeftCtrl;
        case 256 + 0x0074:              return ImGuiKey_LeftAlt;
        //case 0:                         return ImGuiKey_LeftSuper;
        case 256 + 0x0071:              return ImGuiKey_RightShift;
        case 256 + 0x0073:              return ImGuiKey_RightCtrl;
        case 256 + 0x0075:              return ImGuiKey_RightAlt;
        //case 0:                         return ImGuiKey_RightSuper;
        //case 0:                         return ImGuiKey_Menu;
        case '0':                       return ImGuiKey_0;
        case '1':                       return ImGuiKey_1;
        case '2':                       return ImGuiKey_2;
        case '3':                       return ImGuiKey_3;
        case '4':                       return ImGuiKey_4;
        case '5':                       return ImGuiKey_5;
        case '6':                       return ImGuiKey_6;
        case '7':                       return ImGuiKey_7;
        case '8':                       return ImGuiKey_8;
        case '9':                       return ImGuiKey_9;
        case 'A': case 'a':             return ImGuiKey_A;
        case 'B': case 'b':             return ImGuiKey_B;
        case 'C': case 'c':             return ImGuiKey_C;
        case 'D': case 'd':             return ImGuiKey_D;
        case 'E': case 'e':             return ImGuiKey_E;
        case 'F': case 'f':             return ImGuiKey_F;
        case 'G': case 'g':             return ImGuiKey_G;
        case 'H': case 'h':             return ImGuiKey_H;
        case 'I': case 'i':             return ImGuiKey_I;
        case 'J': case 'j':             return ImGuiKey_J;
        case 'K': case 'k':             return ImGuiKey_K;
        case 'L': case 'l':             return ImGuiKey_L;
        case 'M': case 'm':             return ImGuiKey_M;
        case 'N': case 'n':             return ImGuiKey_N;
        case 'O': case 'o':             return ImGuiKey_O;
        case 'P': case 'p':             return ImGuiKey_P;
        case 'Q': case 'q':             return ImGuiKey_Q;
        case 'R': case 'r':             return ImGuiKey_R;
        case 'S': case 's':             return ImGuiKey_S;
        case 'T': case 't':             return ImGuiKey_T;
        case 'U': case 'u':             return ImGuiKey_U;
        case 'V': case 'v':             return ImGuiKey_V;
        case 'W': case 'w':             return ImGuiKey_W;
        case 'X': case 'x':             return ImGuiKey_X;
        case 'Y': case 'y':             return ImGuiKey_Y;
        case 'Z': case 'z':             return ImGuiKey_Z;
        case 256 + GLUT_KEY_F1:         return ImGuiKey_F1;
        case 256 + GLUT_KEY_F2:         return ImGuiKey_F2;
        case 256 + GLUT_KEY_F3:         return ImGuiKey_F3;
        case 256 + GLUT_KEY_F4:         return ImGuiKey_F4;
        case 256 + GLUT_KEY_F5:         return ImGuiKey_F5;
        case 256 + GLUT_KEY_F6:         return ImGuiKey_F6;
        case 256 + GLUT_KEY_F7:         return ImGuiKey_F7;
        case 256 + GLUT_KEY_F8:         return ImGuiKey_F8;
        case 256 + GLUT_KEY_F9:         return ImGuiKey_F9;
        case 256 + GLUT_KEY_F10:        return ImGuiKey_F10;
        case 256 + GLUT_KEY_F11:        return ImGuiKey_F11;
        case 256 + GLUT_KEY_F12:        return ImGuiKey_F12;
        default:                        return ImGuiKey_None;
    }
}
/*static void GlutUpdateKeyModifiers(){
    ImGuiIO& io = ImGui::GetIO();
    int glut_key_mods = glutGetModifiers();
    io.AddKeyEvent(ImGuiKey_ModCtrl,(glut_key_mods & GLUT_ACTIVE_CTRL)?true:false);
    io.AddKeyEvent(ImGuiKey_ModShift,(glut_key_mods & GLUT_ACTIVE_SHIFT)?true:false);
    io.AddKeyEvent(ImGuiKey_ModAlt,(glut_key_mods & GLUT_ACTIVE_ALT)?true:false);
}*/
static void GlutAddKeyEvent(ImGuiKey key, bool down, int native_keycode)    {
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(key, down);

    // Legacy indexing does not work for me (sooner or later I get some assertions)
    //io.SetKeyEventNativeData(key, native_keycode, -1); // To support legacy indexing (<1.87 user code)
    (void) native_keycode; // Unused

    //GlutUpdateKeyModifiers();
    // This is the code I use to replace GlutUpdateKeyModifiers(). It seems to work better for me.
    ImGuiKeyModFlags mod = 0;
    if (key==ImGuiKey_LeftShift || key==ImGuiKey_RightShift)            mod = ImGuiKeyModFlags_Shift;
    else if (key==ImGuiKey_LeftCtrl || key==ImGuiKey_RightCtrl)         mod = ImGuiKeyModFlags_Ctrl;
    else if (key==ImGuiKey_LeftAlt || key==ImGuiKey_RightAlt)           mod = ImGuiKeyModFlags_Alt;
    else if (key==ImGuiKey_LeftSuper || key==ImGuiKey_RightSuper)       mod = ImGuiKeyModFlags_Super;
    if (mod)    {
        static ImGuiKeyModFlags keymods = io.KeyMods;   // or just 0.
        if (down) keymods|=mod;
        else keymods&=~mod;
        io.AddKeyEvent(ImGuiKey_ModShift,(keymods&ImGuiKeyModFlags_Shift)?true:false);
        io.AddKeyEvent(ImGuiKey_ModCtrl,(keymods&ImGuiKeyModFlags_Ctrl)?true:false);
        io.AddKeyEvent(ImGuiKey_ModAlt,(keymods&ImGuiKeyModFlags_Alt)?true:false);
        io.AddKeyEvent(ImGuiKey_ModSuper,(keymods&ImGuiKeyModFlags_Super)?true:false);
    }
    //----------------------------------------------------------------------------------------------
}

// Key callbacks
static inline void GlutKeyboardUpDown(unsigned int c,int x, int y,bool down)  {
    // Send character to imgui
    ImGuiIO& io = ImGui::GetIO();
    if (c<32 /*&& io.KeyCtrl*/)   {
        // Theis is some strange input from CTRL+some key.
        // Tested only on my system (Ubuntu+freeglut: I'm not sure it's portable).
        // That's why I've not merged this into 'GlutKeyToImGuiKey(...)'.
        // Note that the CTRL modifier is already handled by 'GlutSpecialUpDown(...)':
        // what was missing was that the key pressed alongside was 'eaten' (without this workaround).
        switch (c)  {
        case  1: {c=(unsigned int)'a';break;}    // CTRL+A
        case  2: {c=(unsigned int)'b';break;}    // CTRL+B
        case  3: {c=(unsigned int)'c';break;}    // CTRL+C
        case  4: {c=(unsigned int)'d';break;}    // CTRL+D
        case  5: {c=(unsigned int)'e';break;}    // CTRL+E
        case  6: {c=(unsigned int)'f';break;}    // CTRL+F
        case  7: {c=(unsigned int)'g';break;}    // CTRL+F
        case  8: {c=(unsigned int)'h';break;}    // CTRL+H  [WARNING: without 'io.KeyCtrl', 8 is 'backspace'!!!]
        case  9: {c=(unsigned int)'i';break;}    // CTRL+I  [NOTE: without this line ImGui makes me switch through ImGui::windows with CTRL+I. Was this the expected behavior?]
        case 10: {c=(unsigned int)'j';break;}    // CTRL+J
        case 11: {c=(unsigned int)'k';break;}    // CTRL+K
        case 12: {c=(unsigned int)'l';break;}    // CTRL+L
        // 13 is ENTER (not eaten when CTRL is down AFAIK)
        case 14: {c=(unsigned int)'m';break;}    // CTRL+M
        case 15: {c=(unsigned int)'o';break;}    // CTRL+O
        case 16: {c=(unsigned int)'p';break;}    // CTRL+P
        case 17: {c=(unsigned int)'q';break;}    // CTRL+Q
        case 18: {c=(unsigned int)'r';break;}    // CTRL+R
        case 19: {c=(unsigned int)'s';break;}    // CTRL+S
        case 20: {c=(unsigned int)'t';break;}    // CTRL+T
        case 21: {c=(unsigned int)'u';break;}    // CTRL+U
        case 22: {c=(unsigned int)'v';break;}    // CTRL+V
        case 23: {c=(unsigned int)'w';break;}    // CTRL+W
        case 24: {c=(unsigned int)'x';break;}    // CTRL+X
        case 25: {c=(unsigned int)'y';break;}    // CTRL+Y
        case 26: {c=(unsigned int)'z';break;}    // CTRL+Z

        case 28: {c=(unsigned int)'\\';break;}   // CTRL+'\\'  [Not sure it's portable: it's the top-left key on my keyboard (not always a backslash)]
        //case 28: {c=(unsigned int)'ù';break;}    // CTRL+'ù' (it's the key next to 'enter', in the lower row, on my keyboard)
        //case 195: {c=(unsigned int)'ò';break;}  // SHIFT+'ò' => 'ç' on my keyboard
        default: break;
        }
    }
    const ImGuiKey key = GlutKeyToImGuiKey(c);
    //if (down && io.KeyCtrl && c<32) fprintf(stderr,"GlutKeyboardUpDown: '%i' (%s) %s\n",key,ImGui::GetKeyName(key),down?"[down]":"[up]");
    if (down && c >= 32)    {
        io.AddInputCharacter((unsigned int)c);
        //fprintf(stderr,"Added input character: '%c' (%u) %s\n",(char)c,c,ImGui::GetKeyName(key));
    }
    GlutAddKeyEvent(key, down, c);
    (void)x; (void)y; // Unused
}
static void GlutKeyboard(unsigned char c,int x,int y)   {GlutKeyboardUpDown(c,x,y,true);}
static void GlutKeyboardUp(unsigned char c,int x,int y)   {GlutKeyboardUpDown(c,x,y,false);}

// Special key callbacks
static inline void GlutSpecialUpDown(int key,int x, int y,bool down)  {
    //printf("GlutSpecialUpDown %d [%s]\n", key, down?"down":"up");
    const ImGuiKey imgui_key = GlutKeyToImGuiKey(key + 256);
    //fprintf(stderr,"GlutSpecialUpDown: '%i' (%s) %s\n",imgui_key,ImGui::GetKeyName(imgui_key),down?"[down]":"[up]");
    GlutAddKeyEvent(imgui_key, down, key + 256);
    (void)x; (void)y; // Unused
}
static void GlutSpecial(int key,int x,int y)   {GlutSpecialUpDown(key,x,y,true);}
static void GlutSpecialUp(int key,int x,int y)   {GlutSpecialUpDown(key,x,y,false);}

// Mouse callbacks
static void GlutMouse(int b,int s,int x,int y)  {
    //fprintf(stderr,"GlutMouse(%d,%d,%d,%d);\n",b,s,x,y);
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)x, (float)y);
    //GlutUpdateKeyboardMods();
    if (b>=0 && b<5)    {
        const int d = (b==1 ? 2 : b==2 ? 1 : b);
#       ifndef IMIMPL_GLUT_HAS_MOUSE_WHEEL_CALLBACK
        if (s==GLUT_DOWN && d>2)    {
            //io.MouseWheel = d==3 ? 1 : d==4 ? -1 : 0;   // old
            io.AddMouseWheelEvent(0.0f, d==3 ? 1.0f : (d==4 ? -1.0f : 0.0f));   // new
        }
        else
#       endif //IMIMPL_GLUT_HAS_MOUSE_WHEEL_CALLBACK
        {
            io.AddMouseButtonEvent(d,s==GLUT_DOWN); // new
            // Manual double click handling:
            /*static double dblClickTimes[6]={-FLT_MAX,-FLT_MAX,-FLT_MAX,-FLT_MAX,-FLT_MAX,-FLT_MAX};  // seconds
            if (s == 0)   {
                double time = glutGet(GLUT_ELAPSED_TIME);
                double& oldTime = dblClickTimes[d];
                bool& mouseDoubleClicked = gImGuiBindingMouseDblClicked[b];
                if (time - oldTime < io.MouseDoubleClickTime*1000.f) {
                    mouseDoubleClicked = true;
                    oldTime = -FLT_MAX;
                    //fprintf(stderr,"Double Clicked button %d\n",button);
                }
                else {
                    //fprintf(stderr,"Not Double Clicked button %d (%1.4f < %1.4f)\n",button,(float)time-(float)oldTime,io.MouseDoubleClickTime);
                    mouseDoubleClicked = false;
                    oldTime = time;
                }
            }*/
        }
    }
}
#ifdef IMIMPL_GLUT_HAS_MOUSE_WHEEL_CALLBACK
static void GlutMouseWheel(int button,int dir,int x,int y)  {
    //fprintf(stderr,"GlutMouseWheel(%d,%d,%d,%d);\n",button,dir,x,y);
    ImGuiIO& io = ImGui::GetIO();
    //GlutUpdateKeyboardMods();
    io.AddMousePosEvent((float)x, (float)y);
    if (dir != 0)   io.AddMouseWheelEvent(0.0f, dir > 0 ? 1.0f : -1.0f);
    (void)button; // Unused
}
#endif //IMIMPL_GLUT_HAS_MOUSE_WHEEL_CALLBACK

static void GlutMotion(int x,int y)  {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)x,(float)y);
}
static void GlutPassiveMotion(int x,int y)  {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)x,(float)y);
}
static void GlutDrawGL()    {
    ImGuiIO& io = ImGui::GetIO();
    if (gImGuiAppIconized) WaitFor(1000);

    // Setup timestep
    static double time = 0.0;
    const double current_time =  (double) glutGet(GLUT_ELAPSED_TIME)*0.001;
    static float deltaTime = (float)(current_time -time);
    deltaTime = (float) (current_time - time);
    time = current_time;
    if (deltaTime<=0) deltaTime = (1.0f/60.0f);

    // Start the frame
    io.DeltaTime = deltaTime;
    if (!gImGuiPaused)	{
        static ImGuiMouseCursor oldCursor = ImGuiMouseCursor_Arrow;
        static bool oldMustHideCursor = io.MouseDrawCursor;
        if (oldMustHideCursor!=io.MouseDrawCursor) {
            glutSetCursor(GLUT_CURSOR_NONE);
            //glfwSetInputMode(window, GLFW_CURSOR, io.MouseDrawCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
            oldMustHideCursor = io.MouseDrawCursor;
            oldCursor = ImGuiMouseCursor_COUNT;
        }
        if (!io.MouseDrawCursor) {
            if (oldCursor!=ImGui::GetMouseCursor()) {
                oldCursor=ImGui::GetMouseCursor();
                static const int glutCursors[] = {
                    //GLUT_CURSOR_NONE = -1,
                    GLUT_CURSOR_INHERIT,
                    GLUT_CURSOR_TEXT,
                    GLUT_CURSOR_CROSSHAIR,      //ImGuiMouseCursor_ResizeAll,                  // Unused by ImGui
                    GLUT_CURSOR_UP_DOWN,    //ImGuiMouseCursor_ResizeNS,              // Unused by ImGui
                    GLUT_CURSOR_LEFT_RIGHT, //ImGuiMouseCursor_ResizeEW,              // Unused by ImGui
                    GLUT_CURSOR_TOP_RIGHT_CORNER,//ImGuiMouseCursor_ResizeNESW,
                    GLUT_CURSOR_BOTTOM_RIGHT_CORNER, //ImGuiMouseCursor_ResizeNWSE,          // Unused by ImGui
                    GLUT_CURSOR_INFO,        // ImGuiMouseCursor_Hand
                    GLUT_CURSOR_DESTROY,
                    GLUT_CURSOR_INHERIT,
                };
                IM_ASSERT(sizeof(glutCursors)/sizeof(glutCursors[0])==ImGuiMouseCursor_COUNT+1);    // asserts
                glutSetCursor(glutCursors[oldCursor]);
            }
        }

        if (io.WantSetMousePos)  {
            // Set mouse position if requested by io.WantSetMousePos flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
#           ifndef __EMSCRIPTEN__     // error: undefined symbol: glutWarpPointer (referenced by top-level compiled C/C++ code)
            glutWarpPointer((int)io.MousePos.x, (int)io.MousePos.y);
#           endif
        }
        /*else    {
            // Get mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
            double mouse_x, mouse_y;
            glfwGetCursorPos(window, &mouse_x, &mouse_y);
            io.AddMousePosEvent((float)mouse_x, (float)mouse_y));
        }*/

        ImGui::NewFrame();
    }
    else {
        ImImpl_NewFramePaused();    // Enables some ImGui queries regardless ImGui::NewFrame() not being called.
        gImGuiCapturesInput = false;
    }
    //for (size_t i = 0; i < 5; i++) io.MouseDoubleClicked[i]=gImGuiBindingMouseDblClicked[i];   // We manually set it (otherwise it won't work with low frame rates)

    if (gImGuiPreDrawGLCallback) gImGuiPreDrawGLCallback();
    DrawGL();

    static const int numFramesDelay = 12;
    static int curFramesDelay = -1;
    if (!gImGuiPaused)	{
        gImGuiWereOutsideImGui = !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemActive();
        const bool imguiNeedsInputNow = !gImGuiWereOutsideImGui && (io.WantTextInput || io.MouseDelta.x!=0 || io.MouseDelta.y!=0 || io.MouseWheel!=0);// || io.MouseDownOwned[0] || io.MouseDownOwned[1] || io.MouseDownOwned[2]);
        if (gImGuiCapturesInput != imguiNeedsInputNow) {
            gImGuiCapturesInput = imguiNeedsInputNow;
            //fprintf(stderr,"gImGuiCapturesInput=%s\n",gImGuiCapturesInput?"true":"false");
            if (gImGuiDynamicFPSInsideImGui) {
                if (!gImGuiCapturesInput && !gImGuiWereOutsideImGui) curFramesDelay = 0;
                else curFramesDelay = -1;
            }
        }
        if (gImGuiWereOutsideImGui) curFramesDelay = -1;

        // Rendering
        ImGui::Render();
        ImImpl_RenderDrawLists(ImGui::GetDrawData());
    }
    else {gImGuiWereOutsideImGui=true;curFramesDelay = -1;}

    if (gImGuiPreDrawGLSwapBuffersCallback) gImGuiPreDrawGLSwapBuffersCallback();
    glutSwapBuffers();
    if (gImGuiPostDrawGLSwapBuffersCallback) gImGuiPostDrawGLSwapBuffersCallback();

    //if (!gImGuiPaused)	for (size_t i = 0; i < 5; i++) gImGuiBindingMouseDblClicked[i] = false;   // We manually set it (otherwise it won't work with low frame rates)

    // Handle clamped FPS:
    if (curFramesDelay>=0 && ++curFramesDelay>numFramesDelay) WaitFor(200);     // 200 = 5 FPS - frame rate when ImGui is inactive
    else {
        const float& inverseFPSClamp = gImGuiWereOutsideImGui ? gImGuiInverseFPSClampOutsideImGui : gImGuiInverseFPSClampInsideImGui;
        if (inverseFPSClamp==0.f) WaitFor(500);
        // If needed we must wait (gImGuiInverseFPSClamp-deltaTime) seconds (=> honestly I shouldn't add the * 2.0f factor at the end, but ImGui tells me the wrong FPS otherwise... why? <=)
        else if (inverseFPSClamp>0 && deltaTime < inverseFPSClamp)  WaitFor((unsigned int) ((inverseFPSClamp-deltaTime)*1000.f * 2.0f) );
    }

}
static void GlutIdle(void)  {
   glutPostRedisplay(); // TODO: Well, we could sleep a bit here if we detect that the window is minimized...
}
static void GlutFakeDrawGL() {
   glutDisplayFunc(GlutDrawGL);
}


static void InitImGui(const ImImpl_InitParams* pOptionalInitParams=NULL)    {
    //fprintf(stderr,"InitImGui(%d,%d);\n",w,h);
    //int w, h;
    int fb_w, fb_h;
    fb_w = glutGet(GLUT_WINDOW_WIDTH);
    fb_h = glutGet(GLUT_WINDOW_HEIGHT);
    //glfwGetFramebufferSize(window, &fb_w, &fb_h);
    mousePosScale.x = 1.f;//(float)fb_w / w;                  // Some screens e.g. Retina display have framebuffer size != from window size, and mouse inputs are given in window/screen coordinates.
    mousePosScale.y = 1.f;//(float)fb_h / h;

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
#ifndef _WIN32
    //io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
    //io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;
#endif
#ifdef _WIN32
    //io.ImeSetInputScreenPosFn = ImImpl_ImeSetInputScreenPosFn;
#endif

    // 3 common init steps
    InitImGuiFontTexture(pOptionalInitParams);
    InitImGuiProgram();
    InitImGuiBuffer();
}

static bool InitBinding(const ImImpl_InitParams* pOptionalInitParams=NULL,int argc=0, char** argv=NULL)	{

//-ENABLE-OPENGLES COMPATIBILITY PROFILES----------------------------------
/* // Don't know how to make this work... for sure,
 * // "custom" freeglut compiled with -legl -lglesv2 can do it,
 * // but otherwise it seems that the required profile definition is missing ATM.
 * // Moreover, I'm not sure that the glutInitContextMethods (defined in freeglut_ext.h)
 * // can be called at this point...
*/
/*
#ifndef IMIMPL_SHADER_NONE
#ifdef IMIMPL_SHADER_GLES
#   ifndef IMIMPL_SHADER_GL3
    glutInitContextVersion( 2, 0);
#   else //IMIMPL_SHADER_GL3
    glutInitContextVersion( 3, 0);
#   endif //MIMPL_SHADER_GL3
    //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);                          // GLFW3 can do it!
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_ES);   // SDL2 can do it too!

    // What about freeglut?
    glutInitContextProfile( GLUT_ES_PROFILE ); //GLUT_CORE_PROFILE GLUT_COMPATIBILITY_PROFILE
    //glutInitContextVersion( int majorVersion, int minorVersion );
    //glutInitContextFlags( int flags );  //GLUT_DEBUG  GLUT_FORWARD_COMPATIBLE
    //glutInitContextProfile( int profile ); //GLUT_CORE_PROFILE GLUT_COMPATIBILITY_PROFILE
#endif // IMIMPL_SHADER_GLES
#endif //IMIMPL_SHADER_NONE
*/
//--------------------------------------------------------------------------


    glutInitDisplayMode(GLUT_RGB | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE);// | GLUT_MULTISAMPLE | GLUT_SRGB); // The latter is defined in freeglut_ext.h
    glutInitWindowSize(pOptionalInitParams ? pOptionalInitParams->gWindowSize.x : 1270, pOptionalInitParams ? pOptionalInitParams->gWindowSize.y : 720);
    //int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    //glutInitWindowPosition(5 * screenWidth/ 12, 0);
    glutInit(&argc, argv);
    if (!glutCreateWindow((pOptionalInitParams && pOptionalInitParams->gWindowTitle[0]!='\0') ? (const char*) &pOptionalInitParams->gWindowTitle[0] : "Dear ImGui Glut OpenGL example"))
    {
        fprintf(stderr, "Could not call glutCreateWindow(...) successfully.\n");
        return false;
    }

#ifdef IMGUI_USE_GLAD
   if(!gladLoadGL()) {
        fprintf(stderr,"Error initializing GLAD!\n");
        return false;
    }
    // gladLoadGLLoader(&glutGetProcAddress);
#endif //IMGUI_USE_GLAD
#ifdef IMGUI_USE_GL3W
   if (gl3wInit()) {
       fprintf(stderr, "Error initializing GL3W!\n");
       return false;
   }
#endif //IMGUI_USE_GL3W

    //OpenGL info
    {
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


    // Set up glut callbacks
    glutIdleFunc(GlutIdle);
    glutReshapeFunc(GlutReshapeFunc);
    //glutDisplayFunc(GlutDrawGL);

    //glutInitContextFunc(InitGL);   // freeglut3 only: call order:  ResizeGL() - InitGL() - DrawGL()
    //InitGL();                      // call order:                  InitGL() - DrawGL() - ResizeGL()

    //But I prefer call order: InitGL() - ResizeGL() - DrawGL():
    // this can be achieved by skipping the first DrawGL call this way:
    glutDisplayFunc(GlutFakeDrawGL);

    glutKeyboardFunc(GlutKeyboard);
    glutKeyboardUpFunc(GlutKeyboardUp);
    glutSpecialFunc(GlutSpecial);
    glutSpecialUpFunc(GlutSpecialUp);
    glutMouseFunc(GlutMouse);               // This handles mouse wheel as well on non-Windows systems
    glutMotionFunc(GlutMotion);
    glutPassiveMotionFunc(GlutPassiveMotion);

#ifndef __EMSCRIPTEN__
    glutEntryFunc(GlutEntryFunc);
#endif //__EMSCRIPTEN__

    // Apparently, there's no way to detect when the user minimizes a window using glut (at least on my system these callbacks don't provide this info)
    //glutVisibilityFunc(GlutVisibilityFunc);       // never called
    //glutWindowStatusFunc(GlutWindowStatusFunc);   // called on resizing too

#ifdef IMIMPL_GLUT_HAS_MOUSE_WHEEL_CALLBACK
    glutMouseWheelFunc(GlutMouseWheel);
#endif //IMIMPL_GLUT_HAS_MOUSE_WHEEL_CALLBACK

	return true;
}

// Application code
int ImImpl_Main(const ImImpl_InitParams* pOptionalInitParams,int argc, char** argv)
{
    if (!InitBinding(pOptionalInitParams,argc,argv)) return -1;	
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    InitImGui(pOptionalInitParams);
    ImGuiIO& io = ImGui::GetIO();        

    gImGuiInverseFPSClampInsideImGui = pOptionalInitParams ? ((pOptionalInitParams->gFpsClampInsideImGui!=0) ? (1.0f/pOptionalInitParams->gFpsClampInsideImGui) : 1.0f) : -1.0f;
    gImGuiInverseFPSClampOutsideImGui = pOptionalInitParams ? ((pOptionalInitParams->gFpsClampOutsideImGui!=0) ? (1.0f/pOptionalInitParams->gFpsClampOutsideImGui) : 1.0f) : -1.0f;
    gImGuiDynamicFPSInsideImGui = pOptionalInitParams ? pOptionalInitParams->gFpsDynamicInsideImGui : false;

    InitGL();
    if (gImGuiPostInitGLCallback) gImGuiPostInitGLCallback();
    ResizeGL((int)io.DisplaySize.x,(int)io.DisplaySize.y);
	
#   ifdef __FREEGLUT_EXT_H__
    glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION ) ;
#   endif //__FREEGLUT_STD_H__

//#	ifdef __EMSCRIPTEN__
//    emscripten_set_main_loop(glutMainLoop, 0, 1);
//#	else
    glutMainLoop();     // GLUT has its own main loop, which calls display();
//#   endif //__EMSCRIPTEN__



    ImGui::DestroyContext();
    DestroyGL();
    DestroyImGuiFontTexture();
    DestroyImGuiProgram();
    DestroyImGuiBuffer();

    return 0;
}

#endif //#ifndef IMIMPL_BINDING_H

