//
// Created by Kasper de Bruin on 19/01/2024.
//
#pragma once

#include <imconfig.h>

#include <imgui_internal.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include <KDB_ImGui/backends/imgui_impl_vulkan_image.h>

struct ApplicationTitleBarDebugInfo {
  std::string label;
  bool isActive;
  ImVec2 start;
  ImVec2 end;
  bool drawFilled;

  ImColor rectColor = {255, 0, 0, 255};
  ImColor rectFilledColor = {0, 145, 111, 255};
  ImColor textColor = {0, 255, 255, 255};
  ImColor textBGColor = {255, 0, 0, 165};

  bool backGroundBehindText;
  ImGuiDir_ textDir = ImGuiDir_Right;
};

struct ApplicationTitleBarSettings {
    bool CustomTitleBar = false;

    float Height = 50.f;

    ImVec4 StartMaximized = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    ImVec4 StartWindowed = ImVec4(64.0f, 0.0f, 0.0f, 0.0f);

    bool HasLogo = false;
    ImVec2 LogoDrawSize = ImVec2(45.0f, 45.0f);
    std::shared_ptr<Image> Logo = nullptr;
    std::filesystem::path LogoPath = "";

    float ImageZoom = 1.5f;
    float MainMenuBarExtraHeight = 8.558f;

    bool DrawTitleCentered = false;

    ///DONT CALL BEGIN MAIN MENU BAR IN YOUR CODE
    std::function<void()> *MainMenuBarCallback = nullptr;

    std::shared_ptr<ApplicationTitleBarDebugInfo> DebugInfo = std::make_shared<ApplicationTitleBarDebugInfo>();
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

    ImVec2 MenuBarStartWindow;
    ImVec2 CurrentResetWindowPos;

    ImVec2 CenteredTitleStartScreenPos;
    ImVec2 CenteredTitleEndScreenPos;

    float MenuBarWidth;
    float MenuBarHeight;
};

IMGUI_IMPL_API void ImGui_ImplVKGlfw_init(ApplicationSpecification m_Specification);

//Application and window settings
IMGUI_IMPL_API ApplicationSpecification &ImGui_ImplVKGlfw_getApplicationSpecification();

IMGUI_IMPL_API const ApplicationWindowSettings &ImGui_ImplVKGlfw_getWindowSettings();

IMGUI_IMPL_API const ApplicationTitleBarSettings &ImGui_ImplVKGlfw_getTitleBarSpecification();

//Glfw
IMGUI_IMPL_API GLFWmonitor *ImGui_ImplVKGlfw_getCurrentMonitor();
IMGUI_IMPL_API ImVec2 ImGui_ImplVKGlfw_getCurrentMonitorContentScale();

IMGUI_IMPL_API ImVec2 ImGui_ImplVKGlfw_getWindowSize();

IMGUI_IMPL_API float ImGui_ImplVKGlfw_getMonitorWidth();

IMGUI_IMPL_API float ImGui_ImplVKGlfw_getMonitorHeight();

IMGUI_IMPL_API ImVec2 ImGui_ImplVKGlfw_getWindowFrameSize();

IMGUI_IMPL_API bool ImGui_ImplVKGlfw_isWindowMaximized();

IMGUI_IMPL_API bool ImGui_ImplVKGlfw_shouldClose();

IMGUI_IMPL_API void ImGui_ImplVKGlfw_setShouldClose(const bool value);





//Vulkan
IMGUI_IMPL_API void
ImGui_ImplVKGlfw_setImplErrorCallback(const std::function<void(int error, const char *description)> &func);

IMGUI_IMPL_API VkPhysicalDevice ImGui_ImplVKGlfw_getPhysicalDevice();

IMGUI_IMPL_API VkDevice ImGui_ImplVKGlfw_getDevice();

IMGUI_IMPL_API VkCommandBuffer ImGui_ImplVKGlfw_getCommandBuffer();

IMGUI_IMPL_API void ImGui_ImplVKGlfw_flushCommandBuffer(VkCommandBuffer commandBuffer);

IMGUI_IMPL_API void ImGui_ImplVKGlfw_check_vk_result(VkResult err);

IMGUI_IMPL_API void ImGui_ImplVKGlfw_submitResourceFree(std::function<void()> &&func);


//KDB_ImGui
IMGUI_IMPL_API void ImGui_ImplVKGlfw_startRender();
IMGUI_IMPL_API void ImGui_ImplVKGlfw_endRender();
IMGUI_IMPL_API void ImGui_ImplVKGlfw_shutdown();

IMGUI_IMPL_API ImGuiContext* ImGui_ImplVKGlfw_getCurrentContext();

