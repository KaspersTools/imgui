//#include <imgui.h>
//#include <KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h>
#pragma once

//#ifdef IMGUI_USER_CONFIG_KDBIMGUI
#include <KDB_ImGui/KDBConfig.h>
//#endif
#include <functional>
#include <vulkan/vulkan.h>

struct ApplicationTitleBarDebugInfo;
struct ImGuiWindow;

namespace KDB_IMGUI {
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
     * @brief The height of the title bar.
     */
    float TitleBarHeight = 0.0f;

    /**
     * @brief The debug information of the title bar.
     */
    std::shared_ptr<ApplicationTitleBarDebugInfo> DebugInfo = nullptr;

    /**
     * @brief TempData The constructor of the TempData object.
        */
    TempData() {
    }

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
    }
  };
  /**
     * @brief beginTitleBar handles the beginning of rendering a custom title bar.
     * @param drawTitleCentered Whether to draw the title centered.
     * @param appTitle The application title.
     * @param hasLogo Whether the application has a logo.
     * @param logoDescriptorSet The logo descriptor set.
     * @param logoSize The logo size.
     * @param startMaximized The start position of the maximized window.
     * @param startWindowed The start position of the windowed window.
     * @param inHeight The height of the title bar.
     * @param menuBarExtraOffsetY The extra offset of the menu bar.
     * @param isMaximized Whether the window is maximized.
     * @param debugInfo The debug information of the title bar.
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
                     std::shared_ptr<ApplicationTitleBarDebugInfo> debugInfo);

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
     * @return void
     */
  void beginMenuBar(ImVec2 screenPos, float menuBarWidth, float menuBarHeight, bool drawDebug);

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

  /**
   * @brief calculate center of window/childwindow
   */
  ImVec2 calculateCenteredWindowPos();

  /**
   * @brief render centered (colored text)
   */
  void textCentered(const std::string &text, const ImVec4 &color);

  /**
   * @brief render centered multiline (colored text)
   */
  void textCenteredMultiline(const std::string &text, const ImVec4 &color);

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
}// namespace HBUI