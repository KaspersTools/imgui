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

namespace ImGui::GLFWVULKANIMPL {
  struct ApplicationSpecification {
    std::string Name = "Walnut App";
    uint32_t Width = 1600;
    uint32_t Height = 900;
    std::filesystem::path IconPath;

    bool WindowResizeable = true;
    bool WindowDecorated = true;

    bool CustomTitlebar = false;

    bool CenterWindow = false;
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

  IMGUI_IMPL_API float getTime();

  IMGUI_IMPL_API RenderStats &getRenderStats();
}// namespace ImGui::GLFWVULKANIMPL
#endif//IMGUI_IMGUI_IMPL_GLFW_VULKAN_WINDOW_H
