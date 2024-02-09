//
// Created by Kasper de Bruin on 06/02/2024.
//
#include <HBUI/HBUI.h>
#include <KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h>

namespace HBUI {
    ImVec2 getMainScreenSize() {
      return ImGui_ImplVKGlfw_getWindowSize();
    }

    ImVec2 getMainScreenPos() {
      return ImGui_ImplVKGlfw_getWindowPos();
    }
}// namespace HBUI
