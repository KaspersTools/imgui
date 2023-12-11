#ifndef IMIMPL_BINDING_H
#define IMIMPL_BINDING_H

#include "imguibindings.h"


/*static*/ SDL_Window* window = NULL;
static ImVec2 mousePosScale(1.0f, 1.0f);

static const SDL_SystemCursor sdlCursorIds[] = {
    SDL_SYSTEM_CURSOR_ARROW,
    SDL_SYSTEM_CURSOR_IBEAM,
    SDL_SYSTEM_CURSOR_HAND,//SDL_SYSTEM_CURSOR_SIZEALL,      //SDL_SYSTEM_CURSOR_HAND,    // or SDL_SYSTEM_CURSOR_SIZEALL  //ImGuiMouseCursor_ResizeAll,                  // Unused by ImGui
    SDL_SYSTEM_CURSOR_SIZENS,       //ImGuiMouseCursor_ResizeNS,              // Unused by ImGui
    SDL_SYSTEM_CURSOR_SIZEWE,       //ImGuiMouseCursor_ResizeEW,              // Unused by ImGui
    SDL_SYSTEM_CURSOR_SIZENESW,     //ImGuiMouseCursor_ResizeNESW,
    SDL_SYSTEM_CURSOR_SIZENWSE,     //ImGuiMouseCursor_ResizeNWSE,          // Unused by ImGui
    SDL_SYSTEM_CURSOR_HAND,
    SDL_SYSTEM_CURSOR_NO,
    SDL_SYSTEM_CURSOR_ARROW         // we use this as fallback ImGuiMouseCursor_Arrow
};
static SDL_Cursor* sdlCursors[ImGuiMouseCursor_COUNT+1];

// NB: ImGui already provide OS clipboard support for Windows so this isn't needed if you are using Windows only.
static const char* ImImpl_GetClipboardTextFn(void*)
{
    //return SDL_GetClipboardText();	// Wrong!!! Gets UTF-8 text from the clipboard, which must be freed with SDL_free(), otherwise leaks memory!
    const char* text = SDL_GetClipboardText();
    if (!text) return NULL;
    const int len = strlen(text);
    if (len==0) {SDL_free((void*)text);return "";}
    static ImVector<char> clipboardBuffer;
    // Optional branch to keep clipboardBuffer.capacity() low:
    if (len<=clipboardBuffer.capacity() && clipboardBuffer.capacity()>512)  {
        ImVector<char> emptyBuffer;clipboardBuffer.swap(emptyBuffer);
    }
    clipboardBuffer.resize(len+1);
    strcpy(&clipboardBuffer[0],text);
    SDL_free((void*)text);
    return (const char*) &clipboardBuffer[0];
}

static void ImImpl_SetClipboardTextFn(void*,const char* text)
{
    SDL_SetClipboardText(text);
}
// TODO: once we can find out how to get a HWND from SDL2
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


static ImGuiKey ImplSDL2KeycodeToImGuiKey(int keycode) {
    // Cloned from imgui/backend/imgui_imp_sdl.cpp
    switch (keycode)    {
        case SDLK_TAB: return ImGuiKey_Tab;
        case SDLK_LEFT: return ImGuiKey_LeftArrow;
        case SDLK_RIGHT: return ImGuiKey_RightArrow;
        case SDLK_UP: return ImGuiKey_UpArrow;
        case SDLK_DOWN: return ImGuiKey_DownArrow;
        case SDLK_PAGEUP: return ImGuiKey_PageUp;
        case SDLK_PAGEDOWN: return ImGuiKey_PageDown;
        case SDLK_HOME: return ImGuiKey_Home;
        case SDLK_END: return ImGuiKey_End;
        case SDLK_INSERT: return ImGuiKey_Insert;
        case SDLK_DELETE: return ImGuiKey_Delete;
        case SDLK_BACKSPACE: return ImGuiKey_Backspace;
        case SDLK_SPACE: return ImGuiKey_Space;
        case SDLK_RETURN: return ImGuiKey_Enter;
        case SDLK_ESCAPE: return ImGuiKey_Escape;
        case SDLK_QUOTE: return ImGuiKey_Apostrophe;
        case SDLK_COMMA: return ImGuiKey_Comma;
        case SDLK_MINUS: return ImGuiKey_Minus;
        case SDLK_PERIOD: return ImGuiKey_Period;
        case SDLK_SLASH: return ImGuiKey_Slash;
        case SDLK_SEMICOLON: return ImGuiKey_Semicolon;
        case SDLK_EQUALS: return ImGuiKey_Equal;
        case SDLK_LEFTBRACKET: return ImGuiKey_LeftBracket;
        case SDLK_BACKSLASH: return ImGuiKey_Backslash;
        case SDLK_RIGHTBRACKET: return ImGuiKey_RightBracket;
        case SDLK_BACKQUOTE: return ImGuiKey_GraveAccent;
        case SDLK_CAPSLOCK: return ImGuiKey_CapsLock;
        case SDLK_SCROLLLOCK: return ImGuiKey_ScrollLock;
        case SDLK_NUMLOCKCLEAR: return ImGuiKey_NumLock;
        case SDLK_PRINTSCREEN: return ImGuiKey_PrintScreen;
        case SDLK_PAUSE: return ImGuiKey_Pause;
        case SDLK_KP_0: return ImGuiKey_Keypad0;
        case SDLK_KP_1: return ImGuiKey_Keypad1;
        case SDLK_KP_2: return ImGuiKey_Keypad2;
        case SDLK_KP_3: return ImGuiKey_Keypad3;
        case SDLK_KP_4: return ImGuiKey_Keypad4;
        case SDLK_KP_5: return ImGuiKey_Keypad5;
        case SDLK_KP_6: return ImGuiKey_Keypad6;
        case SDLK_KP_7: return ImGuiKey_Keypad7;
        case SDLK_KP_8: return ImGuiKey_Keypad8;
        case SDLK_KP_9: return ImGuiKey_Keypad9;
        case SDLK_KP_PERIOD: return ImGuiKey_KeypadDecimal;
        case SDLK_KP_DIVIDE: return ImGuiKey_KeypadDivide;
        case SDLK_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case SDLK_KP_MINUS: return ImGuiKey_KeypadSubtract;
        case SDLK_KP_PLUS: return ImGuiKey_KeypadAdd;
        case SDLK_KP_ENTER: return ImGuiKey_KeypadEnter;
        case SDLK_KP_EQUALS: return ImGuiKey_KeypadEqual;
        case SDLK_LSHIFT: return ImGuiKey_LeftShift;
        case SDLK_LCTRL: return ImGuiKey_LeftCtrl;
        case SDLK_LALT: return ImGuiKey_LeftAlt;
        case SDLK_LGUI: return ImGuiKey_LeftSuper;
        case SDLK_RSHIFT: return ImGuiKey_RightShift;
        case SDLK_RCTRL: return ImGuiKey_RightCtrl;
        case SDLK_RALT: return ImGuiKey_RightAlt;
        case SDLK_RGUI: return ImGuiKey_RightSuper;
        case SDLK_APPLICATION: return ImGuiKey_Menu;
        case SDLK_0: return ImGuiKey_0;
        case SDLK_1: return ImGuiKey_1;
        case SDLK_2: return ImGuiKey_2;
        case SDLK_3: return ImGuiKey_3;
        case SDLK_4: return ImGuiKey_4;
        case SDLK_5: return ImGuiKey_5;
        case SDLK_6: return ImGuiKey_6;
        case SDLK_7: return ImGuiKey_7;
        case SDLK_8: return ImGuiKey_8;
        case SDLK_9: return ImGuiKey_9;
        case SDLK_a: return ImGuiKey_A;
        case SDLK_b: return ImGuiKey_B;
        case SDLK_c: return ImGuiKey_C;
        case SDLK_d: return ImGuiKey_D;
        case SDLK_e: return ImGuiKey_E;
        case SDLK_f: return ImGuiKey_F;
        case SDLK_g: return ImGuiKey_G;
        case SDLK_h: return ImGuiKey_H;
        case SDLK_i: return ImGuiKey_I;
        case SDLK_j: return ImGuiKey_J;
        case SDLK_k: return ImGuiKey_K;
        case SDLK_l: return ImGuiKey_L;
        case SDLK_m: return ImGuiKey_M;
        case SDLK_n: return ImGuiKey_N;
        case SDLK_o: return ImGuiKey_O;
        case SDLK_p: return ImGuiKey_P;
        case SDLK_q: return ImGuiKey_Q;
        case SDLK_r: return ImGuiKey_R;
        case SDLK_s: return ImGuiKey_S;
        case SDLK_t: return ImGuiKey_T;
        case SDLK_u: return ImGuiKey_U;
        case SDLK_v: return ImGuiKey_V;
        case SDLK_w: return ImGuiKey_W;
        case SDLK_x: return ImGuiKey_X;
        case SDLK_y: return ImGuiKey_Y;
        case SDLK_z: return ImGuiKey_Z;
        case SDLK_F1: return ImGuiKey_F1;
        case SDLK_F2: return ImGuiKey_F2;
        case SDLK_F3: return ImGuiKey_F3;
        case SDLK_F4: return ImGuiKey_F4;
        case SDLK_F5: return ImGuiKey_F5;
        case SDLK_F6: return ImGuiKey_F6;
        case SDLK_F7: return ImGuiKey_F7;
        case SDLK_F8: return ImGuiKey_F8;
        case SDLK_F9: return ImGuiKey_F9;
        case SDLK_F10: return ImGuiKey_F10;
        case SDLK_F11: return ImGuiKey_F11;
        case SDLK_F12: return ImGuiKey_F12;
    }
    return ImGuiKey_None;
}


static void InitImGui(const ImImpl_InitParams* pOptionalInitParams=NULL)	{
    //int w, h;
    int fb_w, fb_h;
    SDL_GetWindowSize(window, &fb_w, &fb_h);
    //glfwGetFramebufferSize(window, &fb_w, &fb_h);
    mousePosScale.x = 1.f;//(float)fb_w / w;                  // Some screens e.g. Retina display have framebuffer size != from window size, and mouse inputs are given in window/screen coordinates.
    mousePosScale.y = 1.f;//(float)fb_h / h;

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)fb_w, (float)fb_h);  // Display size, in pixels. For clamping windows positions.
    io.DeltaTime = 1.0f/60.0f;                          // Time elapsed since last frame, in seconds (in this sample app we'll override this every frame because our timestep is variable)
    //io.PixelCenterOffset = 0.0f;                        // Align OpenGL texels

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;   // We can honor GetMouseCursor() values
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;    // We can honor io.WantSetMousePos requests (optional, rarely used)

    io.BackendUsingLegacyKeyArrays = 0; // Not sure if user must set this

    //io.RenderDrawListsFn = ImImpl_RenderDrawLists;
    io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
    io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;
#ifdef _MSC_VER
    //io.ImeSetInputScreenPosFn = ImImpl_ImeSetInputScreenPosFn;
#endif

    // 3 common init steps
    InitImGuiFontTexture(pOptionalInitParams);
    InitImGuiProgram();
    InitImGuiBuffer();
}

static bool InitBinding(const ImImpl_InitParams* pOptionalInitParams=NULL,int argc=0, char** argv=NULL)	{
/*  // From the web:
SDL_INIT_EVERYTHING & ~(SDL_INIT_TIMER | SDL_INIT_HAPTIC) succeeds in Firefox. Timer doesn't work because it requires threads, and haptic support also isn't included. SDL_INIT_AUDIO requires Web Audio, and causes failure in IE. There is no flag for threads; they are simply initialized by default if they are enabled.
In the SDL2 version of my DOSBox port, I simply don't use SDL_INIT_TIMER and call SDL_GetTicks() anyways. Yes, that is wrong according to SDL documentation, but it works. Availability of that function is probably important for games. For audio, I use SDL_InitSubSystem(SDL_INIT_AUDIO), and run in no audio mode if that fails.
I think failing in SDL_Init() when a requested subsystem doesn't work properly is reasonable. When porting it's a lot easier to understand that failure than than failures later on when trying to use it.
*/
#ifndef IMIMPL_SDL2_INIT_FLAGS
#   ifndef __EMSCRIPTEN__
#		define IMIMPL_SDL2_INIT_FLAGS (SDL_INIT_EVERYTHING)
#	else //__EMSCRIPTEN__
#		if (defined(WITH_SDL) || defined(WITH_SDL_STATIC) || defined(WITH_SDL2) || defined(WITH_SDL2_STATIC)) // YES_IMGUISOLOUD
#			define IMIMPL_SDL2_INIT_FLAGS (SDL_INIT_VIDEO|SDL_INIT_AUDIO)
#		else // WITH_SDL2_STATIC // YES_IMGUISOLOUD
#			define IMIMPL_SDL2_INIT_FLAGS (SDL_INIT_VIDEO)
#		endif // WITH_SDL2_STATIC // YES_IMGUISOLOUD
#	endif //__EMSCRIPTEN__
#endif //IMIMPL_SDL2_INIT_FLAGS

    if (SDL_Init(IMIMPL_SDL2_INIT_FLAGS) < 0)	{
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    // Init OpenGL
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

//-ENABLE-OPENGLES COMPATIBILITY PROFILES----------------------------------
#ifndef IMIMPL_SHADER_NONE
#ifdef IMIMPL_SHADER_GLES
#   ifndef IMIMPL_SHADER_GL3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);          // 1 => ES1.1   2 => ES2.0  3 => ES3.0
#   else //IMIMPL_SHADER_GL3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
#   endif //MIMPL_SHADER_GL3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_ES);
#endif // IMIMPL_SHADER_GLES
#endif //IMIMPL_SHADER_NONE
//--------------------------------------------------------------------------

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,);
    //SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE,1);

    window = SDL_CreateWindow((pOptionalInitParams && pOptionalInitParams->gWindowTitle[0]!='\0') ? (const char*) &pOptionalInitParams->gWindowTitle[0] : "Dear ImGui SDL2 OpenGL example",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          pOptionalInitParams ? pOptionalInitParams->gWindowSize.x : 1270,
                          pOptionalInitParams ? pOptionalInitParams->gWindowSize.y : 720,
                          SDL_WINDOW_RESIZABLE
                          | SDL_WINDOW_OPENGL
                          //| SDL_WINDOW_ALLOW_HIGHDPI
                          );
    if (!window)
    {
        fprintf(stderr, "Could not call SDL_CreateWindow(...) successfully.\n");
        return false;
    }

    SDL_GL_CreateContext(window);


    // 0 => immediate; 1 => vsync; -1 => vsync if possible, but late swaps happen immediately instead of waiting for the next retrace.
    if (SDL_GL_SetSwapInterval(-1)==-1) {
        //fprintf(stderr,"Warning SwapInterval(-1) not supported\n");
        // It should fall back to 0 if unsupported
        SDL_GL_SetSwapInterval(1);  // vsync
    }

#ifdef IMGUI_USE_GLAD
   if(!gladLoadGL()) {
        fprintf(stderr,"Error initializing GLAD!\n");
        return false;
    }
    // gladLoadGLLoader(&SDL_GL_GetProcAddress);
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
#       endif // IMIMPL_SHADER_NONE
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

	return true;
}

static void ImplSDL2UpdateGamepads()    {
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)  return;

    // Get gamepad
    io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
    SDL_GameController* game_controller = SDL_GameControllerOpen(0);
    if (!game_controller)   return;
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

    // Update gamepad inputs
    #define IM_SATURATE(V)                      (V < 0.0f ? 0.0f : V > 1.0f ? 1.0f : V)
    #define MAP_BUTTON(KEY_NO, BUTTON_NO)       { io.AddKeyEvent(KEY_NO, SDL_GameControllerGetButton(game_controller, BUTTON_NO) != 0); }
    #define MAP_ANALOG(KEY_NO, AXIS_NO, V0, V1) { float vn = (float)(SDL_GameControllerGetAxis(game_controller, AXIS_NO) - V0) / (float)(V1 - V0); vn = IM_SATURATE(vn); io.AddKeyAnalogEvent(KEY_NO, vn > 0.1f, vn); }
    const int thumb_dead_zone = 8000;           // SDL_gamecontroller.h suggests using this value.
    MAP_BUTTON(ImGuiKey_GamepadStart,           SDL_CONTROLLER_BUTTON_START);
    MAP_BUTTON(ImGuiKey_GamepadBack,            SDL_CONTROLLER_BUTTON_BACK);
    MAP_BUTTON(ImGuiKey_GamepadFaceDown,        SDL_CONTROLLER_BUTTON_A);              // Xbox A, PS Cross
    MAP_BUTTON(ImGuiKey_GamepadFaceRight,       SDL_CONTROLLER_BUTTON_B);              // Xbox B, PS Circle
    MAP_BUTTON(ImGuiKey_GamepadFaceLeft,        SDL_CONTROLLER_BUTTON_X);              // Xbox X, PS Square
    MAP_BUTTON(ImGuiKey_GamepadFaceUp,          SDL_CONTROLLER_BUTTON_Y);              // Xbox Y, PS Triangle
    MAP_BUTTON(ImGuiKey_GamepadDpadLeft,        SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    MAP_BUTTON(ImGuiKey_GamepadDpadRight,       SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    MAP_BUTTON(ImGuiKey_GamepadDpadUp,          SDL_CONTROLLER_BUTTON_DPAD_UP);
    MAP_BUTTON(ImGuiKey_GamepadDpadDown,        SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    MAP_BUTTON(ImGuiKey_GamepadL1,              SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    MAP_BUTTON(ImGuiKey_GamepadR1,              SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    MAP_ANALOG(ImGuiKey_GamepadL2,              SDL_CONTROLLER_AXIS_TRIGGERLEFT,  0.0f, 32767);
    MAP_ANALOG(ImGuiKey_GamepadR2,              SDL_CONTROLLER_AXIS_TRIGGERRIGHT, 0.0f, 32767);
    MAP_BUTTON(ImGuiKey_GamepadL3,              SDL_CONTROLLER_BUTTON_LEFTSTICK);
    MAP_BUTTON(ImGuiKey_GamepadR3,              SDL_CONTROLLER_BUTTON_RIGHTSTICK);
    MAP_ANALOG(ImGuiKey_GamepadLStickLeft,      SDL_CONTROLLER_AXIS_LEFTX,  -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiKey_GamepadLStickRight,     SDL_CONTROLLER_AXIS_LEFTX,  +thumb_dead_zone, +32767);
    MAP_ANALOG(ImGuiKey_GamepadLStickUp,        SDL_CONTROLLER_AXIS_LEFTY,  -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiKey_GamepadLStickDown,      SDL_CONTROLLER_AXIS_LEFTY,  +thumb_dead_zone, +32767);
    MAP_ANALOG(ImGuiKey_GamepadRStickLeft,      SDL_CONTROLLER_AXIS_RIGHTX, -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiKey_GamepadRStickRight,     SDL_CONTROLLER_AXIS_RIGHTX, +thumb_dead_zone, +32767);
    MAP_ANALOG(ImGuiKey_GamepadRStickUp,        SDL_CONTROLLER_AXIS_RIGHTY, -thumb_dead_zone, -32768);
    MAP_ANALOG(ImGuiKey_GamepadRStickDown,      SDL_CONTROLLER_AXIS_RIGHTY, +thumb_dead_zone, +32767);
    #undef MAP_BUTTON
    #undef MAP_ANALOG
    #undef IM_SATURATE
}


static void ImImplMainLoopFrame(void* pDone)	{
    ImGuiIO& io = ImGui::GetIO();
    int& done = *((int*) pDone);
    static SDL_Event event;

    //for (size_t i = 0; i < 5; i++) gImGuiBindingMouseDblClicked[i] = false;   // We manually set it (otherwise it won't work with low frame rates)
    if (!gImGuiPaused) {
        static ImGuiMouseCursor oldCursor = ImGuiMouseCursor_Arrow;
        static bool oldMustHideCursor = io.MouseDrawCursor;
        if (oldMustHideCursor!=io.MouseDrawCursor) {
            SDL_ShowCursor(io.MouseDrawCursor?0:1);
            oldMustHideCursor = io.MouseDrawCursor;
            oldCursor = ImGuiMouseCursor_COUNT;
        }
        if (!io.MouseDrawCursor) {
            if (oldCursor!=ImGui::GetMouseCursor()) {
                oldCursor=ImGui::GetMouseCursor();
                SDL_SetCursor(sdlCursors[oldCursor]);
            }
        }
    }

    static bool gImGuiAppIsIconified = false;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT: {
            if (event.window.event<=SDL_WINDOWEVENT_RESTORED) gImGuiAppIsIconified = (event.window.event==SDL_WINDOWEVENT_MINIMIZED || event.window.event==SDL_WINDOWEVENT_HIDDEN);
            //printf("%d\n",event.window.event);fflush(stdout);
            switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
            {
                int width = event.window.data1;
                int height = event.window.data2;
                int fb_w, fb_h;
                fb_w = width;fb_h = height;
                mousePosScale.x = 1.f;//(float)fb_w / w;              // Some screens e.g. Retina display have framebuffer size != from window size, and mouse inputs are given in window/screen coordinates.
                mousePosScale.y = 1.f;//(float)fb_h / h;
                io.DisplaySize = ImVec2((float)fb_w, (float)fb_h);    // Display size, in pixels. For clamping windows positions.

                ResizeGL(width,height);
            }
                break;
            }
        }
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            SDL_Keymod mod = SDL_GetModState();
            io.AddKeyEvent(ImGuiKey_ModCtrl,((mod & (KMOD_LCTRL|KMOD_RCTRL)) != 0)?true:false);
            io.AddKeyEvent(ImGuiKey_ModShift,((mod & (KMOD_LSHIFT|KMOD_RSHIFT)) != 0)?true:false);
            io.AddKeyEvent(ImGuiKey_ModAlt,((mod & (KMOD_LALT|KMOD_RALT)) != 0)?true:false);
            io.AddKeyEvent(ImGuiKey_ModSuper,((mod & (KMOD_LGUI|KMOD_RGUI)) != 0)?true:false);

            const bool down = (event.type == SDL_KEYDOWN);
            ImGuiKey key = ImplSDL2KeycodeToImGuiKey(event.key.keysym.sym);
            io.AddKeyEvent(key, down);

            //io.SetKeyEventNativeData(key, event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.scancode); // To support legacy indexing (<1.87 user code). Legacy backend uses SDLK_*** as indices to IsKeyXXX() functions.
        }
            break;
            //case SDL_TEXTEDITING:   break;
        case SDL_TEXTINPUT:
        {
            io.AddInputCharactersUTF8(event.text.text);
        }
            break;
        case SDL_MOUSEBUTTONDOWN:        /**< Mouse button pressed */
        case SDL_MOUSEBUTTONUP: {
            SDL_Keymod mod = SDL_GetModState();
            io.AddKeyEvent(ImGuiKey_ModCtrl,((mod & (KMOD_LCTRL|KMOD_RCTRL)) != 0)?true:false);
            io.AddKeyEvent(ImGuiKey_ModShift,((mod & (KMOD_LSHIFT|KMOD_RSHIFT)) != 0)?true:false);
            io.AddKeyEvent(ImGuiKey_ModAlt,((mod & (KMOD_LALT|KMOD_RALT)) != 0)?true:false);
            io.AddKeyEvent(ImGuiKey_ModSuper,((mod & (KMOD_LGUI|KMOD_RGUI)) != 0)?true:false);
            if (event.button.button>0 && event.button.button<6) {
                static const int evBtnMap[5]={0,2,1,3,4};
                //io.MouseDown[ evBtnMap[event.button.button-1] ] = (event.button.type == SDL_MOUSEBUTTONDOWN);   // Old
                io.AddMouseButtonEvent(evBtnMap[event.button.button-1],event.button.type==SDL_MOUSEBUTTONDOWN); // New
                //if (event.button.clicks==2 && event.button.type == SDL_MOUSEBUTTONDOWN) gImGuiBindingMouseDblClicked[evBtnMap[event.button.button-1]] = true;
           }
            //fprintf(stderr,"mousePressed[%d] = %s\n",event.button.button-1,(event.button.type == SDL_MOUSEBUTTONDOWN)?"true":"false");
        }
            break;
        case SDL_MOUSEWHEEL:
            // positive away from the user and negative toward the user
            //fprintf(stderr,"io.MouseWheel = %d (%d,%d)\n",io.MouseWheel,event.wheel.x,event.wheel.y);
            io.AddMouseWheelEvent(event.wheel.x>0?1.0f:(event.wheel.x<0?-1.0f:0.0f),event.wheel.y>0?1.0f:(event.wheel.y<0?-1.0f:0.0f));
            break;
        case SDL_MOUSEMOTION:
            io.AddMousePosEvent((float)event.motion.x * mousePosScale.x, (float)event.motion.y * mousePosScale.y);
            //fprintf(stderr,"io.MousePos (%1.2f,%1.2f)\n",io.MousePos.x,io.MousePos.y);
            break;
        case SDL_QUIT:
            done = 1;
#				ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#				endif //
            break;
        case SDL_USEREVENT:
           if (gImGuiUserEventCallback) gImGuiUserEventCallback((void*)&event);
           break;
        default:
            break;
        }
    }

    // Setup io.DeltaTime
    /*static Uint32  time = SDL_GetTicks();
    const Uint32  current_time =  SDL_GetTicks();
    static float deltaTime = (float)(0.001*(double)(current_time - time));
    deltaTime = (float)(0.001*(double)(current_time - time));*/

    static const Uint64 frequency = SDL_GetPerformanceFrequency();
    static Uint64 time = SDL_GetPerformanceCounter();
    const Uint64 current_time = SDL_GetPerformanceCounter();
    static float deltaTime = 0.f;
    deltaTime = (float)((double)(current_time - time) / frequency);

    if (deltaTime<=0) deltaTime=1.0f/60.0f;
    time = current_time;

    // Start the frame
    {
        io.DeltaTime = deltaTime;
        if (!gImGuiPaused) {
            if (io.WantSetMousePos)  {
                // Set mouse position if requested by io.WantSetMousePos flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
                // SDL_WarpMouseInWindow() and SDL_WarpMouseGlobal()
                //SDL_WarpMouseGlobal((int)io.MousePos.x, (int)io.MousePos.y);  // Wrong!
                SDL_WarpMouseInWindow(window,(int)io.MousePos.x, (int)io.MousePos.y);
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
    }

    if (gImGuiPreDrawGLCallback) gImGuiPreDrawGLCallback();
    DrawGL();


    static const int numFramesDelay = 12;
    static int curFramesDelay = -1;
    if (!gImGuiPaused)	{
        gImGuiWereOutsideImGui = !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemActive();
        const bool imguiNeedsInputNow = !gImGuiWereOutsideImGui && (io.WantTextInput || io.MouseDelta.x!=0 || io.MouseDelta.y!=0 || io.MouseWheel!=0 || io.MouseWheelH!=0);// || io.MouseDownOwned[0] || io.MouseDownOwned[1] || io.MouseDownOwned[2]);
        if (gImGuiCapturesInput != imguiNeedsInputNow) {
            gImGuiCapturesInput = imguiNeedsInputNow;
            //fprintf(stderr,"gImGuiCapturesInput=%s\n",gImGuiCapturesInput?"true":"false");
            if (gImGuiDynamicFPSInsideImGui) {
                if (!gImGuiCapturesInput && !gImGuiWereOutsideImGui) curFramesDelay = 0;
                else curFramesDelay = -1;
            }
        }
        if (gImGuiWereOutsideImGui) curFramesDelay = -1;

        ImGui::Render();
        ImImpl_RenderDrawLists(ImGui::GetDrawData());
    }
    else {gImGuiWereOutsideImGui=true;curFramesDelay = -1;}

    if (gImGuiPreDrawGLSwapBuffersCallback) gImGuiPreDrawGLSwapBuffersCallback();
    SDL_GL_SwapWindow(window);
    if (gImGuiPostDrawGLSwapBuffersCallback) gImGuiPostDrawGLSwapBuffersCallback();

    // Handle clamped FPS:
    if (gImGuiAppIsIconified) {WaitFor(500);/*printf("Minimized\n");fflush(stdout);*/}
    else if (curFramesDelay>=0 && ++curFramesDelay>numFramesDelay) WaitFor(200);     // 200 = 5 FPS - frame rate when ImGui is inactive
    else {
        const float& inverseFPSClamp = gImGuiWereOutsideImGui ? gImGuiInverseFPSClampOutsideImGui : gImGuiInverseFPSClampInsideImGui;
        if (inverseFPSClamp==0.f) WaitFor(500);
        // If needed we must wait (gImGuiInverseFPSClamp-deltaTime) seconds (=> honestly I shouldn't add the * 2.0f factor at the end, but ImGui tells me the wrong FPS otherwise... why? <=)
        else if (inverseFPSClamp>0.f && deltaTime < inverseFPSClamp)  WaitFor((unsigned int) ((inverseFPSClamp-deltaTime)*1000.f * 2.0f) );
    }

    // Update game controllers (if enabled and available)
    ImplSDL2UpdateGamepads();
}

// Application code
int ImImpl_Main(const ImImpl_InitParams* pOptionalInitParams,int argc, char** argv)
{
    if (!InitBinding(pOptionalInitParams,argc,argv)) return -1;
    // New: create cursors-------------------------------------------
    IM_ASSERT(sizeof(sdlCursorIds)/sizeof(sdlCursorIds[0])==ImGuiMouseCursor_COUNT+1);
    for (int i=0,isz=ImGuiMouseCursor_COUNT+1;i<isz;i++) {
        sdlCursors[i] = SDL_CreateSystemCursor(sdlCursorIds[i]);
    }
    //---------------------------------------------------------------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    InitImGui(pOptionalInitParams);
    ImGuiIO& io = ImGui::GetIO();           
    
    InitGL();
    if (gImGuiPostInitGLCallback) gImGuiPostInitGLCallback();
    ResizeGL((int) io.DisplaySize.x,(int) io.DisplaySize.y);
	
    gImGuiInverseFPSClampInsideImGui = pOptionalInitParams ? ((pOptionalInitParams->gFpsClampInsideImGui!=0) ? (1.0f/pOptionalInitParams->gFpsClampInsideImGui) : 1.0f) : -1.0f;
    gImGuiInverseFPSClampOutsideImGui = pOptionalInitParams ? ((pOptionalInitParams->gFpsClampOutsideImGui!=0) ? (1.0f/pOptionalInitParams->gFpsClampOutsideImGui) : 1.0f) : -1.0f;
    gImGuiDynamicFPSInsideImGui = pOptionalInitParams ? pOptionalInitParams->gFpsDynamicInsideImGui : false;

	int done = 0;
#	ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(ImImplMainLoopFrame,&done, 0, 1);
#	else
	while (!done)	{
		ImImplMainLoopFrame((void*)&done);
	}
#	endif //__EMSCRIPTEN__

    DestroyGL();
    ImGui::DestroyContext();
    DestroyImGuiFontTexture();
    DestroyImGuiProgram();
    DestroyImGuiBuffer();

    // New: delete cursors-------------------------------------------
    for (int i=0,isz=ImGuiMouseCursor_COUNT+1;i<isz;i++) {
        SDL_FreeCursor(sdlCursors[i]);
    }
    //---------------------------------------------------------------
    if (window) {SDL_DestroyWindow(window);window = NULL;}
    SDL_Quit();
    return 0;
}

#endif //#ifndef IMIMPL_BINDING_H

