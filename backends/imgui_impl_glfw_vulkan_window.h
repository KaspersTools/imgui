//
// Created by Kasper de Bruin on 19/01/2024.
//

#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#ifndef IMGUI_IMGUI_IMPL_GLFW_VULKAN_WINDOW_H
#define IMGUI_IMGUI_IMPL_GLFW_VULKAN_WINDOW_H
#include <imgui.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "imgui_internal.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>


struct ApplicationTitleBarSettings {
  bool CustomTitleBar = false;

  float Height = 58.0f;

  ImVec4 StartMaximized = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  ImVec4 StartWindowed = ImVec4(64.0f, 0.0f, 0.0f, 0.0f);

  ImVec2 LogoSize = ImVec2(32.0f, 32.0f);


  ///DONT CALL BEGIN MAIN MENU BAR IN YOUR CODE
  std::function<void()> *MainMenuBarCallback = nullptr;

  ImVec2 CurrentSize;
  ImVec2 CurrentMinScreen;
  ImVec2 CurrentMaxScreen;

  ImVec2 CurrentMinWindow;
  ImVec2 CurrentMaxWindow;

  void DrawDebug() {
    ImGui::Text("TitleBar");
    ImGui::Separator();
    ImGui::DragFloat4("Height", &Height, 0.0f, 100.0f);
    ImGui::SliderFloat4("StartMaximized", &StartMaximized.x, 0.0f, 100.0f);
    ImGui::SliderFloat4("StartWindowed", &StartWindowed.x, 0.0f, 100.0f);
    ImGui::Text("MainMenuBarCallback: %s", MainMenuBarCallback == nullptr ? "nullptr" : MainMenuBarCallback->target_type().name());
    ImGui::Separator();
    ImGui::Text("CurrentSize: %.2f, %.2f", CurrentSize.x, CurrentSize.y);
    ImGui::Text("CurrentMinScreen: %.2f, %.2f", CurrentMinScreen.x, CurrentMinScreen.y);
    ImGui::Text("CurrentMaxScreen: %.2f, %.2f", CurrentMaxScreen.x, CurrentMaxScreen.y);
    ImGui::Text("CurrentMinWindow: %.2f, %.2f", CurrentMinWindow.x, CurrentMinWindow.y);
    ImGui::Text("CurrentMaxWindow: %.2f, %.2f", CurrentMaxWindow.x, CurrentMaxWindow.y);


    ImGui::Separator();
  }
};

struct ApplicationWindowSettings {
  uint32_t Width = 1600;
  uint32_t Height = 900;

  bool WindowResizeable = true;
  bool WindowDecorated = true;
  bool CenterWindow = false;

  bool CreateDefaultDockSpace = true;

  void DrawDebug() {
    ImGui::Text("Window");
    ImGui::Separator();
    ImGui::Text("Width: %d", Width);
    ImGui::Text("Height: %d", Height);

    ImGui::BeginDisabled();
    ImGui::Checkbox("WindowResizeable", &WindowResizeable);
    ImGui::Checkbox("WindowDecorated", &WindowDecorated);
    ImGui::Checkbox("CenterWindow", &CenterWindow);
    ImGui::EndDisabled();

    ImGui::Checkbox("CreateDefaultDockSpace", &CreateDefaultDockSpace);
    ImGui::Separator();
  }
};

struct ApplicationSpecification {
  std::string Name = "Walnut App";
  std::filesystem::path IconPath;

  ApplicationWindowSettings WindowSettings;
  ApplicationTitleBarSettings TitleBarSettings;
};

struct RenderStats {
  float FrameTime = 0.0f;
  float TimeStep = 0.0f;
  float LastFrameTime = 0.0f;

  void showRenderStats(bool *p_open) {
    ImGuiIO &io = ImGui::GetIO();

    ImGui::Begin("Render Stats", p_open);

    {
      ImGui::Text("Application");
      ImGui::Separator();
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", FrameTime * 1000, 1000.0f / (FrameTime * 1000));

      ImGui::Separator();
    }
    {
      ImGui::Text("ImGui");
      ImGui::Separator();
      ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
      ImGui::Text("ImGui average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
      //        Text("%d visible windows, %d current allocations", io.MetricsRenderWindows, g.DebugAllocInfo.TotalAllocCount - g.DebugAllocInfo.TotalFreeCount);
      ImGui::Separator();
    }


    ImGui::End();
  }
};


IMGUI_IMPL_API void ImGui_ImplVKGlfw_init(ApplicationSpecification m_Specification);
IMGUI_IMPL_API void ImGui_ImplVKGlfw_startRender();
IMGUI_IMPL_API void ImGui_ImplVKGlfw_endRender();
IMGUI_IMPL_API void ImGui_ImplVKGlfw_shutdown();
IMGUI_IMPL_API GLFWwindow *ImGui_ImplVKGlfw_getMainWindowHandle();
IMGUI_IMPL_API const float ImGui_ImplVKGlfw_getTime();
IMGUI_IMPL_API RenderStats &ImGui_ImplVKGlfw_getRenderStats();

//LOGGING
IMGUI_IMPL_API void ImGui_ImplVKGlfw_setImplErrorCallback(const std::function<void(int error, const char *description)> &func);
IMGUI_IMPL_API void ImGui_ImplVKGlfw_addLog(const std::string &logMsg, const std::string &logType);
IMGUI_IMPL_API const std::map<std::string, std::string> ImGui_ImplVKGlfw_getLog();

//APPLICATION
IMGUI_IMPL_API ApplicationSpecification &ImGui_ImplVKGlfw_getApplicationSpecification();
IMGUI_IMPL_API const ApplicationWindowSettings &ImGui_ImplVKGlfw_getWindowSettings();
IMGUI_IMPL_API const ApplicationTitleBarSettings &ImGui_ImplVKGlfw_getTitleBarSpecification();

//application specification
//window specifiation

//titlebar specification
//titlebar specification

//VULKAN
IMGUI_IMPL_API VkInstance ImGui_ImplVKGlfw_getInstance();
IMGUI_IMPL_API VkPhysicalDevice ImGui_ImplVKGlfw_getPhysicalDevice();
IMGUI_IMPL_API VkDevice ImGui_ImplVKGlfw_getDevice();
IMGUI_IMPL_API VkCommandBuffer ImGui_ImplVKGlfw_getCommandBuffer(bool begin);

IMGUI_IMPL_API void ImGui_ImplVKGlfw_flushCommandBuffer(VkCommandBuffer commandBuffer);
IMGUI_IMPL_API void ImGui_ImplVKGlfw_submitResourceFree(std::function<void()> &&func);

//GLFW WINDOW
IMGUI_IMPL_API const bool ImGui_ImplVKGlfw_isWindowMaximized();
IMGUI_IMPL_API ImVec2 ImGui_ImplVKGlfw_getWindowSize();
IMGUI_IMPL_API ImVec2 ImGui_ImplVKGlfw_getWindowPos();

IMGUI_IMPL_API void ImGui_ImplVKGlfw_setWindowSize(const ImVec2 &size);
IMGUI_IMPL_API void ImGui_ImplVKGlfw_setWindowPos(const ImVec2 &pos);

IMGUI_IMPL_API void ImGui_ImplVKGlfw_setWindowTitle(const char *title);
IMGUI_IMPL_API void ImGui_ImplVKGlfw_setWindowIcon(const char *iconPath);

IMGUI_IMPL_API void ImGui_ImplVKGlfw_setWindowDecorated(bool decorated);
IMGUI_IMPL_API void ImGui_ImplVKGlfw_setWindowResizable(bool resizable);
IMGUI_IMPL_API void ImGui_ImpVKGlfw_setWindowMaximized(bool maximized);
IMGUI_IMPL_API void ImGui_ImplVKGlfw_setWindowFloating(bool floating);

IMGUI_IMPL_API float ImGui_ImplVKGlfw_getMonitorWidth();
IMGUI_IMPL_API float ImGui_ImplVKGlfw_getMonitorHeight();

IMGUI_IMPL_API ImVec2 ImGui_ImplVKGlfw_getWindowFrameSize();
IMGUI_IMPL_API ImVec2 ImGui_ImplVKGlfw_getWindowSize();


//TitleBar

namespace KDB_IMGUI_EXTENSION {
  // To experiment with editor theme live you can change these constexpr into static
  // members of a static "Theme" class and add a quick ImGui window to adjust the colour values
  namespace Colors::Theme {
    inline static ImColor titlebar = ImColor(255, 0, 255, 255);
    inline static ImColor text = ImColor(192, 192, 192, 255);
    inline static ImColor invalidPrefab = ImColor(255, 1, 1, 255);
    inline static ImColor invalidLogo = ImColor(255, 0, 255, 255);
    inline static ImColor mainMenuBarOutLine = ImColor(0, 255, 0, 255);

    inline static void DrawDebug() {
      ImGui::Text("Theme");
      ImGui::Separator();
      ImGui::ColorEdit4("Titlebar", &titlebar.Value.x);
      ImGui::ColorEdit4("Text", &text.Value.x);
      ImGui::ColorEdit4("InvalidPrefab", &invalidPrefab.Value.x);
      ImGui::ColorEdit4("InvalidLogo", &invalidLogo.Value.x);
      ImGui::Separator();
    }
  }// namespace Colors::Theme

  namespace Colors {
    inline ImU32 Normalized(const ImColor &color) {
      return (color.Value.x / 255.0f, color.Value.y / 255.0f, color.Value.z / 255.0f, color.Value.w / 255.0f);
    }
    inline ImU32 ColorWithMultipliedValue(const ImColor &color, float multiplier) {
      const ImVec4 &colRow = color.Value;
      float hue, sat, val;
      ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
      return ImColor::HSV(hue, sat, std::min(val * multiplier, 1.0f));
    }
  }// namespace Colors

  inline static float CurrentMenuXScreenStart = 0;
  inline static float CurrentMenuXScreenEnd = 0;

  void ShiftCursorY(float distance);

  ImRect GetItemRect();
  ImRect RectExpanded(const ImRect &rect, float x, float y);
  ImRect RectOffset(const ImRect &rect, float x, float y);

  ImVec2 GetScaleFactor();

  void RenderWindowOuterBorders(ImGuiWindow *window);
  bool UpdateWindowManualResize(ImGuiWindow *window, ImVec2 &newSize, ImVec2 &newPosition);

  // Menubar with custom rectangle
  bool BeginMenubar(const ImRect &barRectangle);
  void EndMenubar();

  bool BeginMenu(const char* label, bool enabled = true);
  void EndMenu();

  inline static ImVec2 screenToWindowSpace(ImVec2 screenPos) {
    ImVec2 currentPos = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(screenPos);
    ImVec2 windowPos = ImGui::GetCursorPos();
    ImGui::SetCursorScreenPos(currentPos);
    return windowPos;
  }

  inline static ImVec2 windowToScreenSpace(ImVec2 windowPos) {
    ImVec2 currentPos = ImGui::GetCursorPos();
    ImGui::SetCursorPos(windowPos);
    ImVec2 screenPos = ImGui::GetCursorScreenPos();
    ImGui::SetCursorPos(currentPos);
    return screenPos;
  }
}// namespace KDB_IMGUI_EXTENSION


#endif//IMGUI_IMGUI_IMPL_GLFW_VULKAN_WINDOW_H
