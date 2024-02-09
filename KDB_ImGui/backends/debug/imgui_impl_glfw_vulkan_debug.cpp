#include "imgui_impl_glfw_vulkan_debug.h"
#include <vector>

#include "KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h"


void ImguiGlfwVulkanDebugger::init() {
}

void ImguiGlfwVulkanDebugger::render() {
  ImGui::Begin("ImguiGlfwVulkanDebugger");
  ImGui::Text("Hello from ImguiGlfwVulkanDebugger");

  ApplicationSpecification &applicationSetting = ImGui_ImplVKGlfw_getApplicationSpecification();
  ApplicationTitleBarSettings &titleBarSetting = applicationSetting.TitleBarSettings;
  ApplicationWindowSettings &windowSetting = applicationSetting.WindowSettings;

  ImGui::Separator();
  ImGui::Text("ApplicationSpecification");
  ImGui::Text("Name: %s", applicationSetting.Name.c_str());
  ImGui::Separator();
  ImGui::Text("Application titlebar debugging");
  std::shared_ptr<ApplicationTitleBarDebugInfo> dbInfo = applicationSetting.TitleBarSettings.DebugInfo;

  ImGui::Checkbox("DrawTitleBarDebugInfo", &dbInfo->isActive);
  ImGui::Checkbox("DrawFilled", &dbInfo->drawFilled);

  ImGui::ColorEdit4("RectColor", &dbInfo->rectColor.Value.x);
  ImGui::ColorEdit4("RectBGColor", &dbInfo->rectFilledColor.Value.x);
  ImGui::ColorEdit4("TextColor", &dbInfo->textColor.Value.x);
  ImGui::ColorEdit4("TextBGColor", &dbInfo->textBGColor.Value.x);

  ImGui::Checkbox("BackGroundBehindText", &dbInfo->backGroundBehindText);

  {
    const char *debugTextDirection = "None";
    switch (dbInfo->textDir) {
      case ImGuiDir_None:
        debugTextDirection = "None";
        break;
      case ImGuiDir_Left:
        debugTextDirection = "Left";
        break;
      case ImGuiDir_Right:
        debugTextDirection = "Right";
        break;
      case ImGuiDir_Up:
        debugTextDirection = "Up";
        break;
      case ImGuiDir_Down:
        debugTextDirection = "Down";
        break;
    }
    ImGui::Text("DebugTextDirection: %s", debugTextDirection);
    const std::vector<char *> debugTextDirectionOptions = {"None", "Left", "Right", "Up", "Down"};
    if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
      for (int n = -1; n < ImGuiDir_COUNT - 1; ++n) {
        const bool is_selected = (dbInfo->textDir == n);
        if (ImGui::Selectable(debugTextDirectionOptions[n + 1], is_selected)) {
          switch (n) {
            case -1:
              dbInfo->textDir = ImGuiDir_None;
              break;
            case 0:
              dbInfo->textDir = ImGuiDir_Left;
              break;
            case 1:
              dbInfo->textDir = ImGuiDir_Right;
              break;
            case 2:
              dbInfo->textDir = ImGuiDir_Up;
              break;
            case 3:
              dbInfo->textDir = ImGuiDir_Down;
              break;
          }
        }

        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndListBox();
    }
  }

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
  ImGui::Text("ImGui_ImplVKGlfw_getApplicationSpecification");
  ImGui::Text("Centered Title Bar: %f %f", applicationSetting.CenteredTitleStartScreenPos.x, applicationSetting.CenteredTitleStartScreenPos.y);

  ImGui::Separator();
  ImGui::Text("Style");
  ApplicationWindowStyleSettings &style = windowSetting.StyleSettings;
  ImGui::SliderFloat2("WindowPaddingNormal", &style.WindowPaddingNormal.x, -100.0f, 100.0f);
  ImGui::SliderFloat2("WindowPaddingMaximized", &style.WindowPaddingMaximized.x, -100.0f, 100.0f);
  ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, -100.0f, 100.0f);
  ImGui::ColorEdit4("MenuBarBG", &style.MenuBarBG.x);
  ImGui::End();
}

void ImguiGlfwVulkanDebugger::shutdown() {}
