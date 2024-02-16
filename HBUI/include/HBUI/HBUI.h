//
// Created by Kasper de Bruin on 06/02/2024.
//

#pragma once
#ifndef HBUI_API
#define HBUI_API
#endif

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <iostream>
#include <memory>
#include <vector>


//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations
//-----------------------------------------------------------------------------
typedef int MainWindowFlags; // -> enum HBMainWindowFlags_
typedef int DockspaceFlags;  // -> enum HBDockspaceFlags_
typedef int MainMenuBarFlags;// -> enum HBMainMenuBarFlags_

typedef int HBDrawType;      // -> enum HBDrawType_

struct HBContext;

struct HBTime;

struct HBDrawData;
struct HBUIItem;
struct HBCircle;
struct HBRect;

struct HBUpdatable;

//-----------------------------------------------------------------------------
// [SECTION] Drawables
//-----------------------------------------------------------------------------
enum HBDrawType_ {
  Normal = 0,
  Circle = 1,
  Square = 2
};

struct HBUpdatable {
  public:
  HBUpdatable() {
  };

  public:
  virtual void update(float deltaTime) = 0;
};

struct HBUIItem   : HBUpdatable {
  public:
  HBUIItem(const ImColor color) :
                                      Color(color){
  };
  ~HBUIItem() {
  };

  //ID
  std::string ID = "kasper123";

  //Color
  ImColor Color   = {};

  //Children
  std::vector<std::shared_ptr<HBUIItem>> Children = {};

  public:
  virtual bool draw(ImDrawList *drawList)                 = 0;
//  void update(float deltaTime)                          override;
};

struct HBRect     : HBUIItem {
  HBRect(const ImColor color) :
                                HBUIItem(color){
  }
  ~HBRect(){
  }

  //Position
  ImVec2 start = {};
  ImVec2 end   = {};

  bool  draw(ImDrawList *drawList) override;
  void  update(float deltaTime)    override;

  ImVec2 StartPos()                 const {
    return start;
  }

  ImVec2 EndPos()                   const {
    return end;
  }

  ImVec2 Size()                     const {
    return ImVec2(end.x - start.x, end.y - start.y);
  }
};


//-----------------------------------------------------------------------------
// [SECTION] Main menubar
//-----------------------------------------------------------------------------
enum HBMainMenuBarFlags_ {
  HB_MAIN_MENU_BAR_FLAG_NONE        = 0     ,
  HB_MAIN_MENU_BAR_FLAG_HORIZONTAL  = 1 << 0,
  HB_MAIN_MENU_BAR_FLAG_VERTICAL    = 1 << 1,
};

struct HBMenuItem : HBUIItem{
  public:
  HBMenuItem(const std::string& label, const HBDrawType drawType, const ImColor color)
      : HBUIItem(color){
    name = label;
    this->drawType = drawType;
  };
  ~HBMenuItem(){};

  public:
  std::string name          =         "";
  ImVec2      size          =         {};

  HBUIItem*   itemToDraw    =         {};
  HBDrawType  drawType      =         HBDrawType_::Normal;

  public:
  bool    draw(ImDrawList *drawList)                override;
  void    update(float deltaTime)                   override;
};

struct HBMainMenuBar : HBRect {
  HBMainMenuBar (MainMenuBarFlags flags, const ImColor color = ImColor(-1,-1,-1,-1)) :
                                                                flags(flags), HBRect(color){

  }
  ~HBMainMenuBar(){};

  MainMenuBarFlags                               flags   =   0;
  std::vector<std::shared_ptr<HBMenuItem>>       items   =   {};

  bool draw(ImDrawList *drawList)                         override;
  void update(float deltaTime)                            override;
};


//-----------------------------------------------------------------------------
// [SECTION] Draw Data
//-----------------------------------------------------------------------------
struct HBDrawData {
  DockspaceFlags                          dockspaceFlags            =   0   ; //the flags for the dockspace
  ImVec2                                  dockspaceSize             =   {}  ; //the size of the dockspace
  ImVec2                                  dockspacePos              =   {}  ; //the position of the dockspace

  std::shared_ptr<HBMainMenuBar>          currentAppendingMenuBar   =   NULL; //the current appending main menu bar
  std::shared_ptr<HBMainMenuBar>          mainMenuBarHorizontal     =   NULL; //the horizontal main menu bar
  std::shared_ptr<HBMainMenuBar>          mainMenuBarVertical       =   NULL; //the vertical main menu bar

  ImVec2 cursorPos                                                  =   ImVec2(-1,-1); //used to determine where for example the dockspace should startr
  ImVec2 savedScreenPos                                             =   ImVec2(-1,-1); //used to determine where for example the dockspace should startr
};

//-----------------------------------------------------------------------------
// [SECTION] Time
//-----------------------------------------------------------------------------
struct HBTime {
  float deltaTime = 0.0f;
  float lastTime  = 0.0f;
  float frameTime = 0.0f;
};

//-----------------------------------------------------------------------------
// [SECTION] Flags & Enumerations
//-----------------------------------------------------------------------------
struct HBPadding {
  float top     = 0;
  float right   = 0;
  float bottom  = 0;
  float left    = 0;

  HBPadding(float top, float right, float bottom, float left) : top(top), right(right), bottom(bottom), left(left) {}

  HBPadding(float vertical, float horizontal)
      : top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}

  HBPadding(float all)
      : top(all), right(all), bottom(all), left(all) {}

  HBPadding() = default;
};

struct HBStyle {
  //Colors
  ImColor menuBarColor                  = ImColor(-1, -1, -1, 255);

  //Sizes
    //menus
      //vertical
      float       mainMenuVerticalWidth                 = 70; //The width of the vertical mainmenubar                         will be saved in draw data later and be will be determined by the items in the menubar
      ImVec2      mainMenuBarVerticalFirstItemOffset    = {}; //Extra position for the first item in the vertical mainmenubar

      //horizontal
      float       mainMenuHorizontalHeight              = 35; //The height of the horizontal mainmenubar                      will be saved in draw data later and be will be determined by the items in the menubar
      ImVec2      mainMenuBarHorizontalFirstItemOffset  = {}; //Extra position for the first item in the vertical mainmenubar

      //shared
      HBPadding   mainMenuItemsPadding                  = {-1}; //The padding for the mainmenu items (top, right, bottom, left). If not set use imgui frame padding
      HBPadding   mainMenuItemsSpacing                  = {-1}; //The spacing for the mainmenu items (top, right, bottom, left). If not set the imgui frame spacing
      HBPadding   getItemSpacing() const{
        if(mainMenuItemsSpacing.top == -1){
          auto imSpac = ImGui::GetStyle().ItemSpacing / 2;
          return HBPadding(imSpac.y, imSpac.x);
        }else{
          return mainMenuItemsSpacing;
        }
      }
};

//-----------------------------------------------------------------------------
// [SECTION] Dockspace
//-----------------------------------------------------------------------------
enum HBDockspaceFlags_ {
  HB_DOCKSPACE_FLAG_NONE      = 0,
  HB_DOCKSPACE_FLAG_MENUBAR   = 1 << 0,
};

//-----------------------------------------------------------------------------
// [SECTION] MAIN Window
//-----------------------------------------------------------------------------
enum HBMainWindowFlags_ {
  HBUI_MAIN_WINDOW_FLAG_NONE            = 0,
  HBUI_MAIN_WINDOW_FLAG_NO_DECORATION   = 1 << 1,
  HBUI_MAIN_WINDOW_FLAG_NO_RESIZE       = 1 << 2,
  HBUI_MAIN_WINDOW_FLAG_NO_MOVE         = 1 << 3,
  HBUI_MAIN_WINDOW_FLAG_NO_TITLEBAR     = 1 << 4
};

//-----------------------------------------------------------------------------
// [SECTION] HBIO
//-----------------------------------------------------------------------------
struct HBIO {
  std::string title = "ImVK";

  int width   = 1280;
  int height = 720;

  MainWindowFlags mainWindowFlags = HBUI_MAIN_WINDOW_FLAG_NONE;
};

//-----------------------------------------------------------------------------
// [SECTION] HBUIContext
//-----------------------------------------------------------------------------
struct HBContext {
  bool initialized  = false;
  HBIO io           = {};
  HBStyle style     = {};
  HBTime time       = {};

  //Draw Data
  std::shared_ptr<HBDrawData> drawData = std::make_shared<HBDrawData>();
};

//-----------------------------------------------------------------------------
// [SECTION] HBUI
//-----------------------------------------------------------------------------
namespace HBUI {
  //---------------------------------------------------------------------------------Q
  HBUI_API HBContext *
  initialize(const std::string &title, int width, int height, MainWindowFlags flags);

  HBUI_API void
  setCurrentContext(HBContext *ctx);

  HBUI_API void
  setCurrentImGuiContext(ImGuiContext *ctx);

  HBUI_API HBContext *
  getCurrentContext();

  HBUI_API void
  clearContext();

  HBUI_API HBStyle&
  getStyle();

  HBUI_API HBIO&
  getIO();

  HBUI_API HBDrawData&
  getDrawData();

  //---------------------------------------------------------------------------------
  HBUI_API ImVec2
  appendToCursor(const ImVec2 &size, const bool addSpacing = true);

  HBUI_API ImVec2
  getCursorViewportPos();

  HBUI_API ImVec2
  getWindowSize();

  HBUI_API ImVec2
  getContentRegionAvail();

  HBUI_API ImVec2
  getViewportPos();

  HBUI_API ImVec2
  getViewportSize();

  //---------------------------------------------------------------------------------
  // [SECTION] Main Window
  //---------------------------------------------------------------------------------
  HBUI_API void
  setMainWindowFlags(MainWindowFlags flags);

  HBUI_API bool
  isMaximized();

  //---------------------------------------------------------------------------------
  // [SECTION] Input
  //---------------------------------------------------------------------------------
  HBUI_API bool
  mouseOverRect(const ImVec2 &start, const ImVec2 &end);

  HBUI_API bool
  mouseOverCircle(const ImVec2 &center, float radius);

  //---------------------------------------------------------------------------------
  // [SECTION] Widgets & Drawing
  //---------------------------------------------------------------------------------
  HBUI_API HBCircle &
  drawCircle(const HBCircle &circle, ImDrawList *drawList, const ImColor &color);

  HBUI_API HBRect &
  drawRect(const HBRect &rect, ImDrawList *drawList, const ImColor &color, bool drawFilled);

  HBUI_API bool
  drawCircleButton(const HBCircle &circle, ImDrawList *drawList, const ImColor &color);

  HBUI_API bool
  drawRectButton(const HBRect &rect, ImDrawList *drawList, const ImColor &color);

  HBUI_API bool
  drawCircleImageButton(const HBCircle &circle, const ImTextureID &texture, const ImColor &color,
                        ImDrawList *drawList);

  HBUI_API void
  addUpdatable(std::shared_ptr<HBUpdatable> updatable);

  //---------------------------------------------------------------------------------
  // [SECTION] Dockspaces
  //---------------------------------------------------------------------------------
  /*
   * @brief begin a full screen dockspace
   * @param flags - the flags for the dockspace, see DockspaceFlags_
   *                  HB_DOCKSPACE_FLAG_NONE, HB_DOCKSPACE_FLAG_MENUBAR
   */
  HBUI_API bool
  beginFullScreenDockspace(HBDockspaceFlags_ flags = HB_DOCKSPACE_FLAG_NONE);

  HBUI_API bool
  beginFullScreenDockspace(const bool isMaximized,
                           const bool mainWindowNoTitlebar,
                           const bool hasMenuBar);
//
//  /*
//   * @brief end the full screen dockspace
//   *      this function should be called after beginFullScreenDockspace(..flags..)
//   */
//  HBUI_API void
//  endFullScreenDockspace();

  //---------------------------------------------------------------------------------
  // [SECTION] Menu Bars
  //---------------------------------------------------------------------------------

//  HBUI_API bool
//  beginMainMenuBar(HBMainMenuBar flags);// create and append to a full screen menu-bar.

  /*
   * @brief create a full screen menu-bar.
   * @param flags - the flags for the main menu bar, see MainMenuBarFlags_
   *                  HB_MAIN_MENU_BAR_FLAG_NONE, HB_MAIN_MENU_BAR_FLAG_HORIZONTAL, HB_MAIN_MENU_BAR_FLAG_VERTICAL //@at 15-04-2024
   * @return true if the main menu bar is active
   */
  HBUI_API bool
  beginMainMenuBar(MainMenuBarFlags flags);// create and append to a full screen menu-bar.

  /*
   * @brief end the main menu bar
   *      this function should be called after beginMainMenuBar, but only if beginMainMenuBar() is true
   */
  IMGUI_API void
  endMainMenuBar();//

  /*
   * @param id    - the id of the menu item
   * @param type  - the type of the menu item,  if the type is not set the default type (imgui main menu bar button) will be used
   * @param size  - if the size is not set the default size will be used
   * @return true if the menu item is active
   */
  IMGUI_API bool
  beginMainMenuItem(const std::string &id, HBDrawType type = 0, ImVec2 size = {});

  IMGUI_API void
  EndMainMenuItem();

  //---------------------------------------------------------------------------------
  // [SECTION] Sample/Debug Windows
  //---------------------------------------------------------------------------------
  HBUI_API void
  showDebugWindow(bool *p_open);

  //---------------------------------------------------------------------------------
  // [SECTION] Updating
  //---------------------------------------------------------------------------------
  HBUI_API float getDeltaTime();
  HBUI_API float getFrameTime();
  HBUI_API float getTime();

  //---------------------------------------------------------------------------------
  // [SECTION] Rendering
  //---------------------------------------------------------------------------------
  HBUI_API void
  startFrame();

  HBUI_API void
  endFrame();

  HBUI_API bool
  wantToClose();

  HBUI_API void
  shutdown();

  //---------------------------------------------------------------------------------
  // [SECTION] Input
  //---------------------------------------------------------------------------------s
  HBUI_API bool
  mouseOverRect(const ImVec2 &start, const ImVec2 &end);

  HBUI_API bool
  mouseOverCircle(const ImVec2 &center, float radius);

  HBUI_API bool
  isFlagSet(int *flags, int flag);

  HBUI_API void
  toggleFlag(int flag);


}// namespace HBUI
