//
// Created by Kasper de Bruin on 19/01/2024.
//

#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "vulkan/vulkan.h"
#include <imgui.h>
#include <KDB_ImGui/backends/imgui_impl_vulkan_image.h>
#include <filesystem>
#include <imgui_internal.h>

struct ApplicationTitleBarDebugInfo {
    //Title bar
    ImVec2 TitleBarLastScreenPos = ImVec2(0.0f, 0.0f);
    ImVec2 TitleBarLastSize = ImVec2(0.0f, 0.0f);
};

struct ApplicationTitleBarSettings {
    bool CustomTitleBar = false;

    float Height = 50.f;

    ImVec4 StartMaximized = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    ImVec4 StartWindowed = ImVec4(64.0f, 0.0f, 0.0f, 0.0f);

    bool HasLogo = false;
    ImVec2 LogoDrawSize = ImVec2(45.021f, 39.628f);
    std::shared_ptr<Image> Logo = nullptr;
    std::__fs::filesystem::path LogoPath = "";
    float ImageZoom = 1.5f;
    float MainMenuBarExtraHeight = 20.0f;

    bool DrawTitleCentered = false;

    ///DONT CALL BEGIN MAIN MENU BAR IN YOUR CODE
    std::function<void()> *MainMenuBarCallback = nullptr;

    ApplicationTitleBarDebugInfo DebugInfo;
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

    bool DrawDebugOutlines = false;
};

IMGUI_IMPL_API void ImGui_ImplVKGlfw_init(ApplicationSpecification m_Specification);

//Application and window settings
IMGUI_IMPL_API ApplicationSpecification &ImGui_ImplVKGlfw_getApplicationSpecification();

IMGUI_IMPL_API const ApplicationWindowSettings &ImGui_ImplVKGlfw_getWindowSettings();

IMGUI_IMPL_API const ApplicationTitleBarSettings &ImGui_ImplVKGlfw_getTitleBarSpecification();

//Glfw
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
[[maybe_unused]]IMGUI_IMPL_API bool
ImGui_ImplVKGlfw_addFont(const std::filesystem::path &path, const std::string &name, const float &size,
                         const ImFontConfig &config, const bool &defaultFont = false);


IMGUI_IMPL_API void ImGui_ImplVKGlfw_startRender();

IMGUI_IMPL_API void ImGui_ImplVKGlfw_endRender();

IMGUI_IMPL_API void ImGui_ImplVKGlfw_shutdown();

