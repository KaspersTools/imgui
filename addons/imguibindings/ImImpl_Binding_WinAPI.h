#ifndef IMIMPL_BINDING_H
#define IMIMPL_BINDING_H

#include <tchar.h>


#include "imguibindings.h"



static HWND window = NULL;
static ImVec2 mousePosScale(1.0f, 1.0f);

static const LPCTSTR win32CursorIds[] = {
    IDC_ARROW,
    IDC_IBEAM,
    IDC_SIZEALL,      //SDL_SYSTEM_CURSOR_HAND,    // or SDL_SYSTEM_CURSOR_SIZEALL  //ImGuiMouseCursor_ResizeAll,                  // Unused by ImGui
    IDC_SIZENS,       //ImGuiMouseCursor_ResizeNS,              // Unused by ImGui
    IDC_SIZEWE,       //ImGuiMouseCursor_ResizeEW,              // Unused by ImGui
    IDC_SIZENESW,     //ImGuiMouseCursor_ResizeNESW,
    IDC_SIZENWSE,     //ImGuiMouseCursor_ResizeNWSE,          // Unused by ImGui
#   ifdef IDC_HAND    // winuser.h defines it if(WINVER >= 0x0500)
    IDC_HAND,         //ImGuiMouseCursor_Hand // Unused by ImGui
#   else
    IDC_ARROW,         //ImGuiMouseCursor_Hand // Unused by ImGui
#   endif
    IDC_ARROW,        //ImGuiMouseCursor_NotAllowed
    IDC_ARROW         //ImGuiMouseCursor_Arrow (we use this as fallback)
};
static HCURSOR win32Cursors[ImGuiMouseCursor_COUNT+1];


// Notify OS Input Method Editor of text input position (e.g. when using Japanese/Chinese inputs, otherwise this isn't needed)
static void ImImpl_ImeSetInputScreenPosFn(int x, int y)
{
    HWND hwnd = window;
    if (HIMC himc = ImmGetContext(hwnd))
    {
        COMPOSITIONFORM cf;
        cf.ptCurrentPos.x = x;
        cf.ptCurrentPos.y = y;
        cf.dwStyle = CFS_FORCE_POSITION;
        ImmSetCompositionWindow(himc, &cf);
    }
}


// --Helper functions cloned by imgui/backend/imgui_impl_win32.cpp----------------
#define IS_VK_DOWN(VK)  ((::GetKeyState(VK) & 0x8000) != 0)
static void ImplWin32AddKeyEvent(ImGuiKey key, bool down, int native_keycode, int native_scancode = -1)  {
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(key, down);
    //io.SetKeyEventNativeData(key, native_keycode, native_scancode); // To support legacy indexing (<1.87 user code)
    IM_UNUSED(native_scancode);
}
static void ImplWin32ProcessKeyEventsWorkarounds()   {
    // Left & right Shift keys: when both are pressed together, Windows tend to not generate the WM_KEYUP event for the first released one.
    if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && !IS_VK_DOWN(VK_LSHIFT))
        ImplWin32AddKeyEvent(ImGuiKey_LeftShift, false, VK_LSHIFT);
    if (ImGui::IsKeyDown(ImGuiKey_RightShift) && !IS_VK_DOWN(VK_RSHIFT))
        ImplWin32AddKeyEvent(ImGuiKey_RightShift, false, VK_RSHIFT);

    // Sometimes WM_KEYUP for Win key is not passed down to the app (e.g. for Win+V on some setups, according to GLFW).
    if (ImGui::IsKeyDown(ImGuiKey_LeftSuper) && !IS_VK_DOWN(VK_LWIN))
        ImplWin32AddKeyEvent(ImGuiKey_LeftSuper, false, VK_LWIN);
    if (ImGui::IsKeyDown(ImGuiKey_RightSuper) && !IS_VK_DOWN(VK_RWIN))
        ImplWin32AddKeyEvent(ImGuiKey_RightSuper, false, VK_RWIN);
}
static void ImplWin32UpdateKeyModifiers()   {
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiKey_ModCtrl,(IS_VK_DOWN(VK_LCONTROL) || IS_VK_DOWN(VK_RCONTROL)) ? true : false);
    io.AddKeyEvent(ImGuiKey_ModShift,(IS_VK_DOWN(VK_LSHIFT) || IS_VK_DOWN(VK_RSHIFT)) ? true : false);
    io.AddKeyEvent(ImGuiKey_ModAlt,(IS_VK_DOWN(VK_LMENU) || IS_VK_DOWN(VK_RMENU)) ? true : false);
    io.AddKeyEvent(ImGuiKey_ModSuper,(IS_VK_DOWN(VK_LWIN) || IS_VK_DOWN(VK_RWIN)) ? true : false);
}
#define IM_VK_KEYPAD_ENTER      (VK_RETURN + 256)   // There is no distinct VK_xxx for keypad enter, instead it is VK_RETURN + KF_EXTENDED, we assign it an arbitrary value to make code more readable (VK_ codes go up to 255)
static ImGuiKey ImplWin32VirtualKeyToImGuiKey(WPARAM wParam)    {
    // Map VK_xxx to ImGuiKey_xxx.
    switch (wParam) {
        case VK_TAB: return ImGuiKey_Tab;
        case VK_LEFT: return ImGuiKey_LeftArrow;
        case VK_RIGHT: return ImGuiKey_RightArrow;
        case VK_UP: return ImGuiKey_UpArrow;
        case VK_DOWN: return ImGuiKey_DownArrow;
        case VK_PRIOR: return ImGuiKey_PageUp;
        case VK_NEXT: return ImGuiKey_PageDown;
        case VK_HOME: return ImGuiKey_Home;
        case VK_END: return ImGuiKey_End;
        case VK_INSERT: return ImGuiKey_Insert;
        case VK_DELETE: return ImGuiKey_Delete;
        case VK_BACK: return ImGuiKey_Backspace;
        case VK_SPACE: return ImGuiKey_Space;
        case VK_RETURN: return ImGuiKey_Enter;
        case VK_ESCAPE: return ImGuiKey_Escape;
        case VK_OEM_7: return ImGuiKey_Apostrophe;
        case VK_OEM_COMMA: return ImGuiKey_Comma;
        case VK_OEM_MINUS: return ImGuiKey_Minus;
        case VK_OEM_PERIOD: return ImGuiKey_Period;
        case VK_OEM_2: return ImGuiKey_Slash;
        case VK_OEM_1: return ImGuiKey_Semicolon;
        case VK_OEM_PLUS: return ImGuiKey_Equal;
        case VK_OEM_4: return ImGuiKey_LeftBracket;
        case VK_OEM_5: return ImGuiKey_Backslash;
        case VK_OEM_6: return ImGuiKey_RightBracket;
        case VK_OEM_3: return ImGuiKey_GraveAccent;
        case VK_CAPITAL: return ImGuiKey_CapsLock;
        case VK_SCROLL: return ImGuiKey_ScrollLock;
        case VK_NUMLOCK: return ImGuiKey_NumLock;
        case VK_SNAPSHOT: return ImGuiKey_PrintScreen;
        case VK_PAUSE: return ImGuiKey_Pause;
        case VK_NUMPAD0: return ImGuiKey_Keypad0;
        case VK_NUMPAD1: return ImGuiKey_Keypad1;
        case VK_NUMPAD2: return ImGuiKey_Keypad2;
        case VK_NUMPAD3: return ImGuiKey_Keypad3;
        case VK_NUMPAD4: return ImGuiKey_Keypad4;
        case VK_NUMPAD5: return ImGuiKey_Keypad5;
        case VK_NUMPAD6: return ImGuiKey_Keypad6;
        case VK_NUMPAD7: return ImGuiKey_Keypad7;
        case VK_NUMPAD8: return ImGuiKey_Keypad8;
        case VK_NUMPAD9: return ImGuiKey_Keypad9;
        case VK_DECIMAL: return ImGuiKey_KeypadDecimal;
        case VK_DIVIDE: return ImGuiKey_KeypadDivide;
        case VK_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case VK_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case VK_ADD: return ImGuiKey_KeypadAdd;
        case IM_VK_KEYPAD_ENTER: return ImGuiKey_KeypadEnter;
        case VK_LSHIFT: return ImGuiKey_LeftShift;
        case VK_LCONTROL: return ImGuiKey_LeftCtrl;
        case VK_LMENU: return ImGuiKey_LeftAlt;
        case VK_LWIN: return ImGuiKey_LeftSuper;
        case VK_RSHIFT: return ImGuiKey_RightShift;
        case VK_RCONTROL: return ImGuiKey_RightCtrl;
        case VK_RMENU: return ImGuiKey_RightAlt;
        case VK_RWIN: return ImGuiKey_RightSuper;
        case VK_APPS: return ImGuiKey_Menu;
        case '0': return ImGuiKey_0;
        case '1': return ImGuiKey_1;
        case '2': return ImGuiKey_2;
        case '3': return ImGuiKey_3;
        case '4': return ImGuiKey_4;
        case '5': return ImGuiKey_5;
        case '6': return ImGuiKey_6;
        case '7': return ImGuiKey_7;
        case '8': return ImGuiKey_8;
        case '9': return ImGuiKey_9;
        case 'A': return ImGuiKey_A;
        case 'B': return ImGuiKey_B;
        case 'C': return ImGuiKey_C;
        case 'D': return ImGuiKey_D;
        case 'E': return ImGuiKey_E;
        case 'F': return ImGuiKey_F;
        case 'G': return ImGuiKey_G;
        case 'H': return ImGuiKey_H;
        case 'I': return ImGuiKey_I;
        case 'J': return ImGuiKey_J;
        case 'K': return ImGuiKey_K;
        case 'L': return ImGuiKey_L;
        case 'M': return ImGuiKey_M;
        case 'N': return ImGuiKey_N;
        case 'O': return ImGuiKey_O;
        case 'P': return ImGuiKey_P;
        case 'Q': return ImGuiKey_Q;
        case 'R': return ImGuiKey_R;
        case 'S': return ImGuiKey_S;
        case 'T': return ImGuiKey_T;
        case 'U': return ImGuiKey_U;
        case 'V': return ImGuiKey_V;
        case 'W': return ImGuiKey_W;
        case 'X': return ImGuiKey_X;
        case 'Y': return ImGuiKey_Y;
        case 'Z': return ImGuiKey_Z;
        case VK_F1: return ImGuiKey_F1;
        case VK_F2: return ImGuiKey_F2;
        case VK_F3: return ImGuiKey_F3;
        case VK_F4: return ImGuiKey_F4;
        case VK_F5: return ImGuiKey_F5;
        case VK_F6: return ImGuiKey_F6;
        case VK_F7: return ImGuiKey_F7;
        case VK_F8: return ImGuiKey_F8;
        case VK_F9: return ImGuiKey_F9;
        case VK_F10: return ImGuiKey_F10;
        case VK_F11: return ImGuiKey_F11;
        case VK_F12: return ImGuiKey_F12;
        default: return ImGuiKey_None;
    }
}
// Allow compilation with old Windows SDK. MinGW doesn't have default _WIN32_WINNT/WINVER versions.
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef DBT_DEVNODES_CHANGED
#define DBT_DEVNODES_CHANGED 0x0007
#endif
// -------------------------------------------------------------------------------



static bool gImGuiAppIconized = false;

// Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();

    switch (message)
    {
        case WM_SIZE:

            switch (wParam)
            {
                case SIZE_MINIMIZED:
                //fprintf(stderr,"SIZE_MINIMIZED %d %d\n",wParam,lParam);
                return 0;
            }
        ResizeGL(LOWORD (lParam),HIWORD (lParam));
        break;

    case WM_CREATE:
        return 0;

    case WM_CLOSE:
        PostQuitMessage( 0 );
        return 0;

    case WM_DESTROY:
        return 0;
    case WM_ACTIVATE:
        gImGuiAppIconized = (HIWORD(wParam) == SIZE_MINIMIZED); // SIZE_MINIMIZED == 1
        //fprintf(stderr,"WM_ACTIVATE %d %d %d gImGuiAppIconized=%d\n",wParam,LOWORD(wParam),HIWORD(wParam),gImGuiAppIconized);
        return 0;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    {
        const bool is_key_down = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
        if (wParam < 256)   {
            // Obtain virtual key code
            // (keypad enter doesn't have its own... VK_RETURN with KF_EXTENDED flag means keypad enter, see IM_VK_KEYPAD_ENTER definition for details, it is mapped to ImGuiKey_KeyPadEnter.)
            int vk = (int)wParam;
            if ((wParam == VK_RETURN) && (HIWORD(lParam) & KF_EXTENDED))    vk = IM_VK_KEYPAD_ENTER;
            // Submit key event
            const ImGuiKey key = ImplWin32VirtualKeyToImGuiKey(vk);
            const int scancode = (int)LOBYTE(HIWORD(lParam));
            if (key != ImGuiKey_None) ImplWin32AddKeyEvent(key, is_key_down, vk, scancode);
            // Submit individual left/right modifier events
            if (vk == VK_SHIFT) {
                // Important: Shift keys tend to get stuck when pressed together, missing key-up events are corrected in ImGui_ImplWin32_ProcessKeyEventsWorkarounds()
                if (IS_VK_DOWN(VK_LSHIFT) == is_key_down) { ImplWin32AddKeyEvent(ImGuiKey_LeftShift, is_key_down, VK_LSHIFT, scancode); }
                if (IS_VK_DOWN(VK_RSHIFT) == is_key_down) { ImplWin32AddKeyEvent(ImGuiKey_RightShift, is_key_down, VK_RSHIFT, scancode); }
            }
            else if (vk == VK_CONTROL)  {
                if (IS_VK_DOWN(VK_LCONTROL) == is_key_down) { ImplWin32AddKeyEvent(ImGuiKey_LeftCtrl, is_key_down, VK_LCONTROL, scancode); }
                if (IS_VK_DOWN(VK_RCONTROL) == is_key_down) { ImplWin32AddKeyEvent(ImGuiKey_RightCtrl, is_key_down, VK_RCONTROL, scancode); }
            }
            else if (vk == VK_MENU) {
                if (IS_VK_DOWN(VK_LMENU) == is_key_down) { ImplWin32AddKeyEvent(ImGuiKey_LeftAlt, is_key_down, VK_LMENU, scancode); }
                if (IS_VK_DOWN(VK_RMENU) == is_key_down) { ImplWin32AddKeyEvent(ImGuiKey_RightAlt, is_key_down, VK_RMENU, scancode); }
            }
        }
        return 0;
    }
    case WM_CHAR:
        // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
        //if (wParam>=0 /*&& wParam<0x10000*/) io.((unsigned int)wParam);
        if (wParam > 0 && wParam < 0x10000) io.AddInputCharacterUTF16((unsigned short)wParam);
        return 0;
    case WM_MOUSEMOVE:
        io.AddMousePosEvent((float)LOWORD(lParam) * mousePosScale.x, (float)HIWORD(lParam) * mousePosScale.y);
        return 0;
    case WM_MOUSELEAVE:
        io.AddMousePosEvent(-FLT_MAX,-FLT_MAX);
        return 0;
    case WM_MOUSEWHEEL:
        /*io.MouseDown[0] = (wParam&MK_LBUTTON);
        io.MouseDown[2] = (wParam&MK_MBUTTON);
        io.MouseDown[1] = (wParam&MK_RBUTTON);*/
        //io.MouseWheel = GET_WHEEL_DELTA_WPARAM(wParam) > 0  ? 1 : -1;   // it's 120 or -120
        io.AddMouseWheelEvent(0.0f, (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
        return 0;
    case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
    case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
    {
        int button = 0;
        if (message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK) { button = 0; }
        if (message == WM_RBUTTONDOWN || message == WM_RBUTTONDBLCLK) { button = 1; }
        if (message == WM_MBUTTONDOWN || message == WM_MBUTTONDBLCLK) { button = 2; }
        if (message == WM_XBUTTONDOWN || message == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
        io.AddMouseButtonEvent(button,true);
        io.AddMousePosEvent((float)LOWORD(lParam) * mousePosScale.x, (float)HIWORD(lParam) * mousePosScale.y);
        return 0;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
    {
        int button = 0;
        if (message == WM_LBUTTONUP) { button = 0; }
        if (message == WM_RBUTTONUP) { button = 1; }
        if (message == WM_MBUTTONUP) { button = 2; }
        if (message == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
        io.AddMouseButtonEvent(button,false);
        io.AddMousePosEvent((float)LOWORD(lParam) * mousePosScale.x, (float)HIWORD(lParam) * mousePosScale.y);
        return 0;
    }
    default:
        return DefWindowProc( hWnd, message, wParam, lParam );

    }
    return 0;
}
#undef IS_VK_DOWN



static void InitImGui(const ImImpl_InitParams* pOptionalInitParams=NULL)	{
    //int w, h;
    int fb_w, fb_h;
    //glfwGetWindowSize(window, &w, &h);
    //glfwGetFramebufferSize(window, &fb_w, &fb_h);
    mousePosScale.x = 1.f;//(float)fb_w / w;                  // Some screens e.g. Retina display have framebuffer size != from window size, and mouse inputs are given in window/screen coordinates.
    mousePosScale.y = 1.f;//(float)fb_h / h;
    RECT rect = {0};::GetClientRect(window,&rect);
    fb_w = rect.right - rect.left;
    fb_h = rect.bottom - rect.top;


    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)fb_w, (float)fb_h);  // Display size, in pixels. For clamping windows positions.
    io.DeltaTime = 1.0f/60.0f;                          // Time elapsed since last frame, in seconds (in this sample app we'll override this every frame because our timestep is variable)
    //io.PixelCenterOffset = 0.0f;                        // Align OpenGL texels

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;   // We can honor GetMouseCursor() values
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;    // We can honor io.WantSetMousePos requests (optional, rarely used)

    io.BackendUsingLegacyKeyArrays = 0;

    //io.RenderDrawListsFn = ImImpl_RenderDrawLists;
    //io.ImeSetInputScreenPosFn = ImImpl_ImeSetInputScreenPosFn;    
    io.ImeWindowHandle = window;   // Is this correct?

    // 3 common init steps
    InitImGuiFontTexture(pOptionalInitParams);
    InitImGuiProgram();
    InitImGuiBuffer();
}




// Application code
int ImImpl_WinMain(const ImImpl_InitParams* pOptionalInitParams,HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int iCmdShow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL quit = FALSE;


    // register window class
    wc.style = CS_OWNDC | CS_DBLCLKS;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, NULL);//IDC_ARROW );
    wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = _T("ImGuiApp");
    RegisterClass( &wc );

    const int width = pOptionalInitParams ? pOptionalInitParams->gWindowSize.x : 1270;
    const int height = pOptionalInitParams ? pOptionalInitParams->gWindowSize.y : 720;

    // create main window
    window = hWnd = CreateWindow(
        _T("ImGuiApp"),
        (pOptionalInitParams && pOptionalInitParams->gWindowTitle[0]!='\0') ? (TCHAR*) &pOptionalInitParams->gWindowTitle[0] : _T("Dear ImGui WinApi OpenGL Example"),
        WS_CAPTION | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        0, 0,
        width,
        height,
        NULL, NULL, hInstance, NULL );

    // enable OpenGL for the window-------------------------------------------------
    PIXELFORMATDESCRIPTOR pfd;
    int format;

    // get the device context (DC)
    hDC = GetDC( hWnd );

    // set the pixel format for the DC
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.cStencilBits = 1;
    pfd.iLayerType = PFD_MAIN_PLANE;
    format = ChoosePixelFormat( hDC, &pfd );
    SetPixelFormat( hDC, format, &pfd );

    // create and enable the render context (RC)
    hRC = wglCreateContext( hDC );
    wglMakeCurrent( hDC, hRC );
    //----------------------------------------------------------------------------------

    // New: create cursors-------------------------------------------
    IM_ASSERT(sizeof(win32CursorIds)/sizeof(win32CursorIds[0])==ImGuiMouseCursor_COUNT+1);
    for (int i=0,isz=ImGuiMouseCursor_COUNT+1;i<isz;i++) {
        win32Cursors[i] = LoadCursor(NULL,(LPCTSTR) win32CursorIds[i]);
        if (i==0) SetCursor(win32Cursors[i]);
    }
    //---------------------------------------------------------------

#ifdef IMGUI_USE_GLAD
   if(!gladLoadGL()) {
        fprintf(stderr,"Error initializing GLAD!\n");
        return false;
    }
    // gladLoadGLLoader(&GetProcAddress);
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
   }
#endif //IMGUI_USE_GLEW

    static double time = 0.0;
    gImGuiInverseFPSClampInsideImGui = pOptionalInitParams ? ((pOptionalInitParams->gFpsClampInsideImGui!=0) ? (1.0f/pOptionalInitParams->gFpsClampInsideImGui) : 1.0f) : -1.0f;
    gImGuiInverseFPSClampOutsideImGui = pOptionalInitParams ? ((pOptionalInitParams->gFpsClampOutsideImGui!=0) ? (1.0f/pOptionalInitParams->gFpsClampOutsideImGui) : 1.0f) : -1.0f;
    gImGuiDynamicFPSInsideImGui = pOptionalInitParams ? pOptionalInitParams->gFpsDynamicInsideImGui : false;


    InitImGui(pOptionalInitParams);
    InitGL();
    if (gImGuiPostInitGLCallback) gImGuiPostInitGLCallback();
    ResizeGL(width,height);

    ImGuiIO& io = ImGui::GetIO();
    // program main loop
    while ( !quit )
    {
        // check for messages
        if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
        {

            // handle or dispatch messages
            if ( msg.message == WM_QUIT )
            {
                quit = TRUE;
            }
            else
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }


        };


        // Setup timestep
        const double current_time =  (double) ::GetTickCount();
        static float deltaTime = (float)(0.001*(current_time - time));
        deltaTime = (float)(0.001*(current_time - time));
        time = current_time;
        if (deltaTime<=0) deltaTime=1.0f/60.0f;


        // Start the frame
        {
            io.DeltaTime = deltaTime;

            // Process workarounds for known Windows key handling issues
            ImplWin32ProcessKeyEventsWorkarounds();

            // Update key modifiers
            ImplWin32UpdateKeyModifiers();

            if (!gImGuiPaused)	{
                // Set OS mouse position if requested last frame by io.WantSetMousePos flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
                if (io.WantSetMousePos)   {
                    POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
                    ClientToScreen(hWnd, &pos);
                    SetCursorPos(pos.x, pos.y);
                }

                static ImGuiMouseCursor oldCursor = ImGuiMouseCursor_Arrow;
                static bool oldMustHideCursor = io.MouseDrawCursor;
                if (oldMustHideCursor!=io.MouseDrawCursor) {
                    ShowCursor(!io.MouseDrawCursor);
                    oldMustHideCursor = io.MouseDrawCursor;
                    oldCursor = ImGuiMouseCursor_COUNT;
                }
                if (!io.MouseDrawCursor) {
                    if (oldCursor!=ImGui::GetMouseCursor()) {
                        oldCursor=ImGui::GetMouseCursor();
                        SetCursor(win32Cursors[oldCursor]);
                        //fprintf(stderr,"SetCursor(%d);\n",oldCursor);
                    }
                }
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
        SwapBuffers( hDC );
        if (gImGuiPostDrawGLSwapBuffersCallback) gImGuiPostDrawGLSwapBuffersCallback();

        // Handle clamped FPS:
        if (gImGuiAppIconized) WaitFor(500);
        else if (curFramesDelay>=0 && ++curFramesDelay>numFramesDelay) WaitFor(200);     // 200 = 5 FPS - frame rate when ImGui is inactive
        else {
            const float& inverseFPSClamp = gImGuiWereOutsideImGui ? gImGuiInverseFPSClampOutsideImGui : gImGuiInverseFPSClampInsideImGui;
            if (inverseFPSClamp==0.f) WaitFor(500);
            // If needed we must wait (gImGuiInverseFPSClamp-deltaTime) seconds (=> honestly I shouldn't add the * 2.0f factor at the end, but ImGui tells me the wrong FPS otherwise... why? <=)
            else if (inverseFPSClamp>0.f && deltaTime < inverseFPSClamp)  WaitFor((unsigned int) ((inverseFPSClamp-deltaTime)*1000.f * 2.0f) );
        }

    }

    DestroyGL();
    DestroyImGuiFontTexture();
    DestroyImGuiProgram();
    DestroyImGuiBuffer();

    // New: delete cursors-------------------------------------------
    for (int i=0,isz=ImGuiMouseCursor_COUNT+1;i<isz;i++) {
        //DestroyCursor(win32Cursors[i]);   // Nope: LoadCursor() loads SHARED cursors that should not be destroyed
    }
    //---------------------------------------------------------------

    // shutdown OpenGL
    wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hRC );
    ReleaseDC( hWnd, hDC );

    // destroy the window explicitly
    DestroyWindow( hWnd );

    ImGui::DestroyContext();

    return msg.wParam;
}


#endif //#ifndef IMIMPL_BINDING_H

