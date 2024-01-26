#include <functional>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace KDB_ImGui {
  struct TempData {
    /**
     * @brief The current reset window position.
     */
    ImVec2 CurrentResetWindowPos;

    /**
     * @brief The start position of the centered title.
     */
    ImVec2 CenteredTitleStartWindow;
    /**
     * @brief The end position of the centered title.
     */
    ImVec2 CenteredTitleEndWindow;

    /**
     * @brief The start position of the menu bar.
     */
    ImVec2 MenuBarStartWindow;
    /**
     * @brief The size of the menu bar.
     */
    ImVec2 MenuBarSize;

    /**
     * @brief Should debug information/lines be drawn?
     */
    bool DrawDebug;

    /**
     * @brief The debug color of the title bar.
     */
    ImColor DebugColor;

    /**
     * @brief The height of the title bar.
     */
    float TitleBarHeight = 0.0f;


    ~TempData() {
    }

    /**
     * @brief reset resets the TempData object.
     * @return void
     */
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

  /**
   * @brief beginTitleBar handles the beginning of rendering a custom title bar.
   * @param drawTitleCentered Should the centered title be drawn?
   * @param appTitle The title of the application.
   * @param hasLogo Does the application have a logo?
   * @param logoDescriptorSet The descriptor set of the logo.
   * @param logoSize The size of the logo.
   * @param startMaximized The start position of the titlebar when maximized.
   * @param startWindowed The start position of the titlebar when windowed.
   * @param inHeight The height of the title bar.
   * @param menuBarExtraOffsetY The extra y offset of the menu bar.
   * @param isMaximized Is the window maximized?
   * @param drawDebug Should debug information be drawn?
   * @param debugColor The color of the debug information.
   * @return void
   */
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

  /**
   * @brief endTitleBar handles the ending of rendering a custom title bar.
   * @return void
   */
  void endTitleBar();

    /**
     * @brief beginMenuBar handles the beginning of rendering a custom menu bar.
     * @param screenPos The screen position of the menu bar.
     * @param menuBarWidth The width of the menu bar.
     * @param menuBarHeight The height of the menu bar.
     * @param drawDebug Should debug information be drawn?
     * @param debugColor The color of the debug information.
     * @return void
     */
  void beginMenuBar(ImVec2 screenPos, float menuBarWidth, float menuBarHeight, bool drawDebug, ImColor debugColor);

    /**
     * @brief endMenuBar handles the ending of rendering a custom menu bar.
     * @return void
     */
  void endMenuBar();

    /**
     * @brief beginMainMenuBar handles the beginning of rendering a custom main menu bar.
     * @return void
     */
  void beginMainMenuBar();

    /**
     * @brief endMainMenuBar handles the ending of rendering a custom main menu bar.
     * @return void
     */
  void endMainMenuBar();

    /**
     * @brief screenToWindowSpace converts a screen position to a window position.
     * @param screenPos The screen position.
     * @return ImVec2 The window position.
     */
  ImVec2 screenToWindowSpace(const ImVec2 &screenPos);

    /**
     * @brief windowToScreenSpace converts a window position to a screen position.
     * @param screenPos The window position.
     * @return ImVec2 The screen position.
     */
  ImVec2 windowToScreenSpace(const ImVec2 &screenPos);

    /**
     * @brief renderWindowOuterBorders renders the outer borders of a window.
     * @param window The window.
     * @return void
     */
  void renderWindowOuterBorders(ImGuiWindow *window);


  class Extension {
public:
    /**
     * @brief getTempData gets the TempData object.
     * @return TempData* The TempData object.
     */
    static TempData *getTempData() {
      if (!s_tempData) {
        s_tempData = new TempData();
      }
      return s_tempData;
    }

    /**
     * @brief setTempData sets the TempData object.
     * @param tempData The TempData object.
     * @return void
     */
    static void setTempData(TempData *tempData) {
      s_tempData = tempData;
    }

    /**
     * @brief release releases the TempData object.
     * @return void
     */
    ~Extension() {
      release();
    }

private:
    /**
     * @brief release releases the TempData object.
     * @return void
     */
    static void release() {
      delete s_tempData;
      s_tempData = nullptr;
    }

private:
    /**
     * @brief s_tempData The TempData object.
     */
    inline static TempData *s_tempData = {};
  };
}// namespace KDB::IMGUI