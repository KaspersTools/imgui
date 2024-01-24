#include <functional>
#include <imgui.h>
#include <imgui_internal.h>

#include <vulkan/vulkan.h>

namespace KDB::IMGUI {
  struct TempData {
    ImVec2 CurrentResetWindowPos;

    //Centered Title
    ImVec2 CenteredTitleStartWindow;
    ImVec2 CenteredTitleEndWindow;

    //Main Menu Bar
    ImVec2 MenuBarStartWindow;
    ImVec2 MenuBarSize;

    //Debug
    bool DrawDebug;
    ImColor DebugColor;

    float TitleBarHeight = 0.0f;

    ~TempData() {
    }

    void reset() {
      CurrentResetWindowPos = {};
      CenteredTitleStartWindow = {};
      CenteredTitleEndWindow = {};
      MenuBarStartWindow = {};
      MenuBarSize = {};

      TitleBarHeight = 0.0f;

      DrawDebug = false;
      DebugColor = {};
    }
  };

  void beginTitleBar(const bool drawTitleCentered,
                     const std::string &appTitle,
                     const bool hasLogo,
                     const VkDescriptorSet logoDescriptorSet,
                     const ImVec2 &logoSize,
                     const ImVec2 &startMaximized,
                     const ImVec2 &startWindowed,
                     const float inHeight,
                     const float menuBarExtraOffsetY,
                     const bool isMaximized,
                     const bool drawDebug,
                     const ImColor debugColor);
  void endTitleBar();

  void beginMenuBar(ImVec2 screenPos, float menuBarWidth, float menuBarHeight, bool drawDebug, ImColor debugColor);
  void endMenuBar();

  void beginMainMenuBar();

  void endMainMenuBar();

  ImVec2 screenToWindowSpace(const ImVec2 &screenPos);
  ImVec2 windowToScreenSpace(const ImVec2 &screenPos);

  void renderWindowOuterBorders(ImGuiWindow *window);

  class Extension {
public:
    static TempData *getTempData() {
      if (!s_tempData) {
        s_tempData = new TempData();
      }
      return s_tempData;
    }

    static void setTempData(TempData *tempData) {
      s_tempData = tempData;
    }

    ~Extension() {
      release();
    }

    static void release() {
      delete s_tempData;
      s_tempData = nullptr;
    }

    inline static TempData *s_tempData = {};
  };
}// namespace KDB::IMGUI