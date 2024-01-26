#include "imgui_impl_glfw_vulkan_debug.h"

#include "KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h"

void ImguiGlfwVulkanDebugger::init(){

}

void ImguiGlfwVulkanDebugger::render(){
    ImGui::Begin("ImguiGlfwVulkanDebugger");
    ImGui::Text("Hello from ImguiGlfwVulkanDebugger");

    ApplicationSpecification& applicationSetting = ImGui_ImplVKGlfw_getApplicationSpecification();
    ApplicationTitleBarSettings& titleBarSetting = applicationSetting.TitleBarSettings;
    ApplicationWindowSettings& windowSetting = applicationSetting.WindowSettings;

    ImGui::Separator();
    ImGui::Text("ApplicationSpecification");
    ImGui::Text("Name: %s", applicationSetting.Name.c_str());
    ImGui::Checkbox("DrawDebugOutlines", &applicationSetting.DrawDebugOutlines);
    ImGui::Separator();

    ImGui::Text("WindowSettings");
    ImGui::Text("Width: %d", windowSetting.Width);
    ImGui::Text("Height: %d", windowSetting.Height);
    ImGui::Text("WindowResizeable: %d", windowSetting.WindowResizeable);
    ImGui::Text("WindowDecorated: %d", windowSetting.WindowDecorated);
    ImGui::Text("CenterWindow: %d", windowSetting.CenterWindow);
    ImGui::Text("CreateDefaultDockSpace: %d", windowSetting.CreateDefaultDockSpace);
    ImGui::Separator();

    ImGui::Text("TitleBarSettings");
    ImGui::Checkbox("CustomTitleBar", &titleBarSetting.CustomTitleBar);
    ImGui::SliderFloat("Height", &titleBarSetting.Height, 0.0f, 100.0f);
    ImGui::SliderFloat4("StartMaximized", &titleBarSetting.StartMaximized.x, 0.0f, 100.0f);
    ImGui::SliderFloat4("StartWindowed", &titleBarSetting.StartWindowed.x, 0.0f, 100.0f);
    ImGui::Checkbox("HasLogo", &titleBarSetting.HasLogo);
    ImGui::SliderFloat2("LogoDrawSize", &titleBarSetting.LogoDrawSize.x, 0.0f, 100.0f);
    ImGui::SliderFloat("ImageZoom", &titleBarSetting.ImageZoom, 0.0f, 100.0f);
    ImGui::Checkbox("DrawTitleCentered", &titleBarSetting.DrawTitleCentered);
    ImGui::Text("MainMenuBarCallback: %s", titleBarSetting.MainMenuBarCallback == nullptr ? "nullptr" : titleBarSetting.MainMenuBarCallback->target_type().name());
    ImGui::SliderFloat("MainMenuBarExtraHeightOffset", &titleBarSetting.MainMenuBarExtraHeight, 0.0f, 100.0f);
    ImGui::Separator();

    ImGui::Text("ApplicationTitleBarDebugInfo");
    ImGui::Text("TitleBarLastScreenPos: %f, %f", titleBarSetting.DebugInfo.TitleBarLastScreenPos.x, titleBarSetting.DebugInfo.TitleBarLastScreenPos.y);
    ImGui::Text("TitleBarLastSize: %f, %f", titleBarSetting.DebugInfo.TitleBarLastSize.x, titleBarSetting.DebugInfo.TitleBarLastSize.y);

    ImGui::Separator();
    ImGui::Text("ImGui_ImplVKGlfw_getApplicationSpecification");
    ImGui::Text("Centered Title Bar: %f %f", applicationSetting.CenteredTitleStartScreenPos.x, applicationSetting.CenteredTitleStartScreenPos.y);
    ImGui::Separator();
    ImGui::End();
}

void ImguiGlfwVulkanDebugger::shutdown(){}
