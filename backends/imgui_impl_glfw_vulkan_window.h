//
// Created by Kasper de Bruin on 19/01/2024.
//

#ifndef IMGUI_IMGUI_IMPL_GLFW_VULKAN_WINDOW_H
#define IMGUI_IMGUI_IMPL_GLFW_VULKAN_WINDOW_H
#include <imgui.h>

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

IMGUI_IMPL_API void generateWindow(ImGuiConfigFlags imGuiConfigFlags);
IMGUI_IMPL_API void startRender();
IMGUI_IMPL_API void endRender();
IMGUI_IMPL_API void cleanupWindow();
#endif //IMGUI_IMGUI_IMPL_GLFW_VULKAN_WINDOW_H
