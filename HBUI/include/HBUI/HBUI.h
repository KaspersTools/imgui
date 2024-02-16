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
// [SECTION] Flags & Enumerations
//-----------------------------------------------------------------------------
//struct HBPadding {
//  float top     = 0;
//  float right   = 0;
//  float bottom  = 0;
//  float left    = 0;
//
//  HBPadding(float top, float right, float bottom, float left) : top(top), right(right), bottom(bottom), left(left) {}
//
//  HBPadding(float vertical, float horizontal)
//      : top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}
//
//  HBPadding(float all)
//      : top(all), right(all), bottom(all), left(all) {}
//
//  HBPadding() = default;
//};


struct HBStyle {
  //Colors
  ImColor menuBarColor                  = ImColor(163, 255, 0, 255);

  //Sizes
    //menus
      //vertical  menubar
        ImVec2      mainMenuBarVerticalFirstItemOffset    = {0,0}; //Extra offset for the first item in the vertical mainmenubar

      //horizontal menubar
        ImVec2      mainMenuBarHorizontalFirstItemOffset  = {0,0}; //Extra offset for the first item in the vertical mainmenubar

    //shared
      //items
      ImVec2      mainMenuItemSize                          = {32, 32}; //The size of the mainmenu items (width, height) \\ for circles x is used for the radius and y is ignored
      ImColor     mainMenuBarItemColor                      = ImColor(255,0,266,255);

      ImVec2      mainMenuItemsPadding                      = {0, 0}; //The padding for the mainmenu items (top, right, bottom, left). If not set use imgui frame padding
      ImVec2      mainMenuItemsSpacing                      = {5, 9}; //The spacing for the mainmenu items (top, right, bottom, left). If not set the imgui frame spacing

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
// [SECTION] Drawables
//-----------------------------------------------------------------------------
enum HBDrawType_ {
  Square = 0,
  Circle = 1,
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
  HBUIItem(){
  };

  ~HBUIItem() {
  };

  //ID
  std::string ID = "kasper123";

  public:
    virtual bool draw(ImDrawList *drawList, ImColor color, bool drawFilled)  = 0;
//  void update(float deltaTime)                          override;
};

struct HBRect     : HBUIItem {
  HBRect() : HBUIItem() {
  }

  ~HBRect(){
  }

  //Position
  ImVec2 start = {};
  ImVec2 end   = {};

  bool  draw(ImDrawList *drawList, ImColor color, bool drawFilled) override;

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
  HB_MAIN_MENU_BAR_FLAG_NONE              = 0     ,
  HB_MAIN_MENU_BAR_FLAG_HORIZONTAL        = 1 << 0,
  HB_MAIN_MENU_BAR_FLAG_VERTICAL          = 1 << 1,
  HB_MAIN_MENU_BAR_FLAG_USE_HBUI_STYLE    = 1 << 2
};

struct HBMenuItem : HBUIItem{
  public:
  HBMenuItem(const std::string& label, const HBDrawType drawType, const ImVec2 size)
      : HBUIItem(){
    this->name     = label;
    this->size     = size;
    this->drawType = drawType;
  };
  ~HBMenuItem(){

  };

  public:
  std::string name          =         "";
  ImVec2      size          =         {};
  ImVec2      pos           =         {};
  HBDrawType  drawType      =         Square;
  ImVec2      spacing       =         {-1,-1};
  ImVec2      getSpacing()  const;

  public:
  bool    draw(ImDrawList *drawList, ImColor color, bool drawFilled) override;
  void    update(float deltaTime)                   override;
};

struct HBMainMenuBar : HBRect {
  HBMainMenuBar (MainMenuBarFlags flags) :
                                                                flags(flags), HBRect(){

  }
  ~HBMainMenuBar(){};

  bool draw(ImDrawList *drawList, ImColor color, bool drawFilled) override;
  void update(float deltaTime)                            override;
  void append(std::shared_ptr<HBMenuItem> item);

  bool    isHorizontal     ()   const;
  bool    useCustomStyle   ()   const;
  ImColor getColor         ()   const;

  //flags
  MainMenuBarFlags                               flags        =   0;

  //properties
  float                                          height       =   50; //The height of the main menu bar. This is calculated based on the items. If This Is An Horizontal Bar Use The Height to offset to set the pos for the next window
  float                                          width        =   50; //The width of the main menu bar.  This is calculated based on the items. If This Is An Vertical Bar Use The Width to offset to set the pos for the next window

  //items
  std::vector<std::shared_ptr<HBMenuItem>>       items        =   {};
  ImVec2                                         nextItemPos  =   {};

  float                                           lowest      =   10;
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
  appendToCursor(const ImVec2 &size, const ImVec2 &spacing, bool addSpacing);

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
   * @brief create a main menu bar item
   * @param id - the id of the item
   * @param size - the size of the item
   * @return true if the item is clicked
   */
  IMGUI_API bool
  mainMenuBarItem(const std::string &id, ImVec2 size = {});

  /*
   * @brief create a main menu bar item
   * @param id - the id of the item
   * @param radius - the radius of the item
   * @return true if the item is clicked
   */
  IMGUI_API bool
  mainMenuBarItem(const std::string &id, float radius = HBUI::getStyle().mainMenuItemSize.x);

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


  //---------------------------------------------------------------------------------
  // [SECTION] Animations
  //---------------------------------------------------------------------------------

}// namespace HBUI
