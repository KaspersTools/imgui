//
// Created by Kasper de Bruin on 19/01/2024.
//

#pragma once

#include "backends/imgui_impl_vulkan_image.h"

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <map>

#include <imgui.h>
#include <imgui_internal.h>
#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>

struct ApplicationTitleBarSettings {
  bool CustomTitleBar = false;

  float Height = 58.0f;

  ImVec4 StartMaximized = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  ImVec4 StartWindowed = ImVec4(64.0f, 0.0f, 0.0f, 0.0f);

  bool   HasLogo = false;
  ImVec2 LogoDrawSize = ImVec2(50.0f, 45.0f);
  std::shared_ptr<Image> Logo = nullptr;
  std::filesystem::path LogoPath = "";
  float ImageZoom = 1.5f;

  bool DrawTitleCentered = false;

  ///DONT CALL BEGIN MAIN MENU BAR IN YOUR CODE
  std::function<void()> *MainMenuBarCallback = nullptr;
};

struct ApplicationWindowSettings {
  uint32_t Width = 1600;
  uint32_t Height = 900;

  bool WindowResizeable = true;
  bool WindowDecorated = true;
  bool CenterWindow = false;

  bool CreateDefaultDockSpace = true;

};

struct ApplicationSpecification {
  std::string Name = "Walnut App";
  ApplicationWindowSettings WindowSettings;
  ApplicationTitleBarSettings TitleBarSettings;
  bool DrawDebugOutlines = false;
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
IMGUI_IMPL_API void ImGui_ImplVKGlfw_check_vk_result(VkResult err);

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

  bool BeginMenu(const char *label, bool enabled = true);
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