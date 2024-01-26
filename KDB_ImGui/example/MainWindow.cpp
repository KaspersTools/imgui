//
// Created by Kasper de Bruin on 19/01/2024.
//


#include "MainWindow.h"
#include <KDB_ImGui/backends/debug/imgui_impl_glfw_vulkan_debug.h>
#include <KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h>

#ifdef KDB_IMGUI_ADDONS_CODE_EDITOR
#include <imguicodeeditor.h>
#endif

#ifdef KDB_IMGUI_ADDONS_NOTIFY
#include <imgui_notify.h>
#endif

#ifdef KDB_IMGUI_ADDONS_DATE_CHOOSER
#include <imguidatechooser.h>
#endif

#ifdef KDB_IMGUI_ADDONS_FILE_DIALOG
#include <ImGuiFileDialog.h>
#endif

#ifdef KDB_IMGUI_ADDONS_STYLE_SERIALIZER
#include <imguistyleserializer.h>
#endif


void MainWindow::init() {

  ApplicationSpecification m_Specification;
  //App settings
  m_Specification.Name = "Imgui Vulkan Example";
  m_Specification.WindowSettings.Width = 800;
  m_Specification.WindowSettings.Height = 600;
  m_Specification.WindowSettings.WindowResizeable = true;
  m_Specification.WindowSettings.WindowDecorated = true;
  m_Specification.WindowSettings.CenterWindow = true;
  m_Specification.WindowSettings.CreateDefaultDockSpace = true;

  //Titlebar settings
  m_Specification.TitleBarSettings.CustomTitleBar = true;
  m_Specification.TitleBarSettings.HasLogo = true;
  m_Specification.TitleBarSettings.LogoPath = "Assets/Icons/logo.png";
  m_Specification.TitleBarSettings.DrawTitleCentered = true;

  //Make it so that the main menu bar is 20 pixels lower then by default.
  // This because the menubar is so big that it overlaps the titlebar
  m_Specification.TitleBarSettings.MainMenuBarExtraHeight = 19.0f;
  m_Specification.TitleBarSettings.Height = 60.132f;

  //Set the main menu bar callback
  m_Specification.TitleBarSettings.MainMenuBarCallback = new std::function<void()>([&]() {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New")) {
        std::cout << "New" << std::endl;
      }
      if (ImGui::MenuItem("Open")) {
        std::cout << "Open" << std::endl;
      }
      if (ImGui::MenuItem("Save")) {
        std::cout << "Save" << std::endl;
      }
      if (ImGui::MenuItem("Exit")) {
        //Close the window
        ImGui_ImplVKGlfw_setShouldClose(true);
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Debug")) {

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Theme")) {

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window")) {
      if (ImGui::MenuItem("Center")) {
      }
      if (ImGui::MenuItem("Resize")) {
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window Decorations")) {
      if (ImGui::MenuItem("Decorated")) {
      }
      if (ImGui::MenuItem("Undecorated")) {
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window Resizeable")) {
      if (ImGui::MenuItem("Resizeable")) {
      }
      if (ImGui::MenuItem("Unresizeable")) {
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window Floating")) {
      if (ImGui::MenuItem("Floating")) {
      }
      if (ImGui::MenuItem("Unfloating")) {
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("ImGui Widgets")) {
      if (ImGui::MenuItem("showDemoWindow")) showDemoWindow = true;
      if (ImGui::MenuItem("showAboutWindow")) showAboutWindow = true;
      if (ImGui::MenuItem("showStyleEditor")) showStyleEditor = true;
      if (ImGui::MenuItem("showMetricsWindow")) showMetricsWindow = true;
      if (ImGui::MenuItem("showDebugLogWindow")) showDebugLogWindow = true;
      if (ImGui::MenuItem("showIDStackToolWindow")) showIDStackToolWindow = true;
      if (ImGui::MenuItem("showFontSelector")) showFontSelector = true;
      if (ImGui::MenuItem("showStyleSelector")) showStyleSelector = true;
      if (ImGui::MenuItem("showUserGuide")) showUserGuide = true;
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Addons")) {

#ifdef KDB_IMGUI_ADDONS_CODE_EDITOR
      if (ImGui::MenuItem("KDB_IMGUI_ADDONS_CODE_EDITOR")) {
      }
#endif

#ifdef KDB_IMGUI_ADDONS_NOTIFY
      if (ImGui::MenuItem("KDB_IMGUI_ADDONS_NOTIFY")) {
      }
#endif

#ifdef KDB_IMGUI_ADDONS_DATE_CHOOSER
      if (ImGui::MenuItem("KDB_IMGUI_ADDONS_DATE_CHOOSER")) {
      }
#endif

#ifdef KDB_IMGUI_ADDONS_FILE_DIALOG
      if (ImGui::MenuItem("KDB_IMGUI_ADDONS_FILE_DIALOG")) {
      }
#endif

#ifdef KDB_IMGUI_ADDONS_STYLE_SERIALIZER
      if (ImGui::MenuItem("KDB_IMGUI_ADDONS_STYLE_SERIALIZER")) {
      }
#endif
      ImGui::EndMenu();
    }
  });

  ImGui_ImplVKGlfw_setImplErrorCallback([&](int error, const char *description) {
    std::cout << "GLFWVULKANIMPL::errorCallback() error: " << error << " description: " << description << std::endl;
  });


  ImGui_ImplVKGlfw_init(m_Specification);

  //log
  std::cout << "----------Application Specification---------------------------------------------------------------------" << std::endl;
  std::cout << "GLFWVULKANIMPL::init() finished" << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().Name: " << ImGui_ImplVKGlfw_getApplicationSpecification().Name << std::endl;
  std::cout << "----------Window Specification--------------------------------------------------------------------------" << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().Width: " << ImGui_ImplVKGlfw_getWindowSettings().Width << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().Height: " << ImGui_ImplVKGlfw_getWindowSettings().Height << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().WindowResizeable: " << ImGui_ImplVKGlfw_getWindowSettings().WindowResizeable << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().WindowDecorated: " << ImGui_ImplVKGlfw_getWindowSettings().WindowDecorated << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().CenterWindow: " << ImGui_ImplVKGlfw_getWindowSettings().CenterWindow << std::endl;
  std::cout << "---------Title bar Specification------------------------------------------------------------------------" << std::endl;
  std::cout << "GLFWVULKANIMPL::getApplicationSpecification().TitleBarSettings.WindowNoDefaultTitleBar: " << ImGui_ImplVKGlfw_getApplicationSpecification().TitleBarSettings.CustomTitleBar << std::endl;
  std::cout << "--------------------------------------------------------------------------------------------------------" << std::endl;
  ///////////

  //load fonts
  //  ImGui_ImplVKGlfw_addFont("JetBrainsMonoNerdFontPropo-Regular.ttf")
  ImFontConfig fontConfig;
  fontConfig.OversampleH = 4;
  fontConfig.OversampleV = 4;

  ImGui_ImplVKGlfw_addFont(
          "Assets/Fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf",
          "JetBrainsMonoNerdFontPropo-Regular",
          14.0f,
          fontConfig, true);
  run();
}

bool MainWindow::run() {
  while (!ImGui_ImplVKGlfw_shouldClose()) {
    render();
  }
}

void MainWindow::render() {
  ImGui_ImplVKGlfw_startRender();
  if (showDemoWindow)
    ImGui::ShowDemoWindow(&showDemoWindow);
  if (showMetricsWindow)
    ImGui::ShowMetricsWindow(&showMetricsWindow);
  if (showDebugLogWindow)
    ImGui::ShowDebugLogWindow(&showDebugLogWindow);
  if (showIDStackToolWindow)
    ImGui::ShowIDStackToolWindow(&showIDStackToolWindow);
  if (showAboutWindow)
    ImGui::ShowAboutWindow(&showAboutWindow);
  if (showStyleEditor)
    ImGui::ShowStyleEditor(&ImGui::GetStyle());
  if (showFontSelector) {
    ImGui::Begin("Font Selector");
    ImGui::ShowFontSelector("Font Selector");
    ImGui::End();
  }
  if (showStyleSelector) {
    ImGui_ImplVKGlfw_ShowStyleSelector("Style Selector", &showStyleSelector);
  }
  if (showUserGuide) {
    ImGui::Begin("User Guide");
    ImGui::ShowUserGuide();
    ImGui::End();
  }

  ImguiGlfwVulkanDebugger::render();
  ImGui_ImplVKGlfw_endRender();
}

void MainWindow::shutdown() {
  ImGui_ImplVKGlfw_shutdown();
}
