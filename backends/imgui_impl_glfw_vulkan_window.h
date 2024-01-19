//
// Created by Kasper de Bruin on 19/01/2024.
//

#include <filesystem>
#include <iostream>
#include <string>

#ifndef IMGUI_IMGUI_IMPL_GLFW_VULKAN_WINDOW_H
#define IMGUI_IMGUI_IMPL_GLFW_VULKAN_WINDOW_H
#include <imgui.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>

namespace ImGui::GLFWVULKANIMPL {
  struct ApplicationTitleBarSettings{
    bool WindowNoDefaultTitleBar = false;

    float TitleBarHeight = 0.0f;
    float TitleBarPadding = 0.0f;
    float TitleBarIconSize = 0.0f;
    float TitleBarIconPadding = 0.0f;

    float TitleBarOffsetX = 60.0f;
    float TitleBarOffsetY = 0.0f;
  };

  struct WindowSettings{
    uint32_t Width = 1600;
    uint32_t Height = 900;

    bool WindowResizeable = true;
    bool WindowDecorated = true;
    bool CenterWindow = false;
  };

  struct ApplicationSpecification {
    std::string Name = "Walnut App";

    std::filesystem::path IconPath;
    WindowSettings WindowSettings;

    ApplicationTitleBarSettings TitleBarSettings;
  };

  struct RenderStats {
    float FrameTime = 0.0f;
    float TimeStep = 0.0f;
    float LastFrameTime = 0.0f;

    void showRenderStats(bool* p_open) {
      ImGuiIO &io = ImGui::GetIO();

      ImGui::Begin("Render Stats", p_open);

      {
        ImGui::Text("Application");
        ImGui::Separator();
        Text("Application average %.3f ms/frame (%.1f FPS)", FrameTime*1000, 1000.0f / (FrameTime*1000));

        ImGui::Separator();
      }
      {
        ImGui::Text("ImGui");
        ImGui::Separator();
        Text("Dear ImGui %s", GetVersion());
        Text("ImGui average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
//        Text("%d visible windows, %d current allocations", io.MetricsRenderWindows, g.DebugAllocInfo.TotalAllocCount - g.DebugAllocInfo.TotalFreeCount);
        ImGui::Separator();
      }




      ImGui::End();
    }
  };


  IMGUI_IMPL_API void init(ApplicationSpecification m_Specification);
  IMGUI_IMPL_API void startRender();
  IMGUI_IMPL_API void endRender();
  IMGUI_IMPL_API void shutdown();

  IMGUI_IMPL_API GLFWwindow *getMainWindowHandle();

  IMGUI_IMPL_API const float getTime();

  IMGUI_IMPL_API RenderStats &getRenderStats();

  //LOGGING
  IMGUI_IMPL_API void setImplErrorCallback(std::function<void(int error, const char *description)> &&func);

  IMGUI_IMPL_API ApplicationSpecification &getApplicationSpecification();
  IMGUI_IMPL_API const ApplicationTitleBarSettings &getTitleBarSpecification();
  IMGUI_IMPL_API const WindowSettings &getWindowSettings();
  IMGUI_IMPL_API const bool isWindowMaximized();

  IMGUI_IMPL_API VkInstance getInstance();
  IMGUI_IMPL_API VkPhysicalDevice getPhysicalDevice();
  IMGUI_IMPL_API VkDevice getDevice();
  IMGUI_IMPL_API VkCommandBuffer getCommandBuffer(bool begin);
  IMGUI_IMPL_API void flushCommandBuffer(VkCommandBuffer commandBuffer);
  IMGUI_IMPL_API void submitResourceFree(std::function<void()> &&func);
}// namespace ImGui::GLFWVULKANIMPL
#endif//IMGUI_IMGUI_IMPL_GLFW_VULKAN_WINDOW_H
