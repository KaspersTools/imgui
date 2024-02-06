//
// Created by Kasper de Bruin on 19/01/2024.
//


#include "MainWindow.h"
#include <KDB_ImGui/backends/debug/imgui_impl_glfw_vulkan_debug.h>
#include <KDB_ImGui/backends/imgui_impl_glfw_vulkan_window.h>
#include <KDB_ImGui/themes/Themes.h>
#include <KDB_ImGui/fonts/FontManager.h>
#include <KDB_ImGui/Extension.h>

#include <iostream>

void MainWindow::init() {

    ApplicationSpecification m_Specification;
    //App settings
    m_Specification.Name = "Imgui Vulkan Example";

    //Window settings
    m_Specification.WindowSettings.Width = 800;
    m_Specification.WindowSettings.Height = 600;
    m_Specification.WindowSettings.WindowResizeable = true;
    m_Specification.WindowSettings.WindowDecorated = true;
    m_Specification.WindowSettings.CenterWindow = true;
    m_Specification.WindowSettings.CreateDefaultDockSpace = true;

    //Titlebar settings
    m_Specification.TitleBarSettings.CustomTitleBar = true;
    m_Specification.TitleBarSettings.DrawTitleCentered = true;

    m_Specification.TitleBarSettings.HasLogo = true;
    m_Specification.TitleBarSettings.LogoPath = "Assets/Textures/LogoSqaured.jpeg";
    m_Specification.TitleBarSettings.LogoDrawSize = ImVec2(45., 45.);
//    m_Specification.TitleBarSettings.LogoRemoveBackground = true;
//    m_Specification.TitleBarSettings.LogoBackgroundColor = ImVec4(255.f, 255.f, 255.f, 255.f);

    //Make it so that the main menu bar is 10 pixels lower then by default.
    // This because the menubar is so big that it overlaps the titlebar
    //TODO: Make this automatic
    m_Specification.TitleBarSettings.MainMenuBarExtraHeight = 10.546f;
    m_Specification.TitleBarSettings.Height = 60.132f;

    //Set the main menu bar callback
    {
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

            if (ImGui::BeginMenu("ImGui Widgets")) {
                if (ImGui::MenuItem("showDemoWindow")) showDemoWindow = true;
                if (ImGui::MenuItem("showAboutWindow")) showAboutWindow = true;
                if (ImGui::MenuItem("showMetricsWindow")) showMetricsWindow = true;
                if (ImGui::MenuItem("showDebugLogWindow")) showDebugLogWindow = true;
                if (ImGui::MenuItem("showIDStackToolWindow")) showIDStackToolWindow = true;
                if (ImGui::MenuItem("showFontSelector")) showFontSelector = true;
                if (ImGui::MenuItem("showUserGuide")) showUserGuide = true;
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Themes")) {
              if (ImGui::MenuItem("Show Style Editor"))
                showStyleEditor = true;
              if(ImGui::MenuItem("ImGuiTheme_ImGuiColorsClassic"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_ImGuiColorsClassic);
              if(ImGui::MenuItem("ImGuiTheme_ImGuiColorsDark"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_ImGuiColorsDark);
              if(ImGui::MenuItem("ImGuiTheme_ImGuiColorsLight"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_ImGuiColorsLight);
              if(ImGui::MenuItem("ImGuiTheme_MaterialFlat"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_MaterialFlat);
              if(ImGui::MenuItem("ImGuiTheme_PhotoshopStyle"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_PhotoshopStyle);
              if(ImGui::MenuItem("ImGuiTheme_GrayVariations"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_GrayVariations);
              if(ImGui::MenuItem("ImGuiTheme_GrayVariations_Darker"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_GrayVariations_Darker);
              if(ImGui::MenuItem("ImGuiTheme_MicrosoftStyle"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_MicrosoftStyle);
              if(ImGui::MenuItem("ImGuiTheme_Cherry"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_Cherry);
              if(ImGui::MenuItem("ImGuiTheme_Darcula"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_Darcula);
              if(ImGui::MenuItem("ImGuiTheme_DarculaDarker"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_DarculaDarker);
              if(ImGui::MenuItem("ImGuiTheme_LightRounded"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_LightRounded);
              if(ImGui::MenuItem("ImGuiTheme_SoDark_AccentBlue"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_SoDark_AccentBlue);
              if(ImGui::MenuItem("ImGuiTheme_SoDark_AccentYellow"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_SoDark_AccentYellow);
              if(ImGui::MenuItem("ImGuiTheme_SoDark_AccentRed"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_SoDark_AccentRed);
              if(ImGui::MenuItem("ImGuiTheme_BlackIsBlack"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_BlackIsBlack);
              if(ImGui::MenuItem("ImGuiTheme_WhiteIsWhite"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_WhiteIsWhite);
              if(ImGui::MenuItem("ImGuiTheme_Count"))
                KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_Count);
              ImGui::EndMenu();
            }
        });
    }

    ImGui_ImplVKGlfw_setImplErrorCallback([&](int error, const char *description) {
        std::cout << "GLFWVULKANIMPL::errorCallback() error: " << error << " description: " << description << std::endl;
    });

    ImGui_ImplVKGlfw_init(m_Specification);

    //log
    {
    }

    //Set theme
    {
      KDB_IMGUI::Themes::ThemeManager::applyTheme(KDB_IMGUI::Themes::ImGuiTheme_SoDark_AccentBlue);
    }

    //load font(s)
    {
        ImFontConfig fontConfig;
        fontConfig.RasterizerDensity = 3.0f;

        KDB_IMGUI::FontManager::addFont("Assets/Fonts/JetBrainsMono/JetBrainsMonoNerdFontPropo-Regular.ttf",
                                        "JetBrainsMonoNerdFontPropo-Regular", 16.0f, fontConfig);
    }

    run();
}

bool MainWindow::run() {
    while (!ImGui_ImplVKGlfw_shouldClose()) {
        render();
    }

    return true;
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
    if (showStyleEditor) {
      KDB_IMGUI::Themes::ThemeManager::showThemeManager(&showStyleEditor);
    }
    if (showFontSelector) {
        ImGui::Begin("Font Selector");
        ImGui::ShowFontSelector("Font Selector");
        ImGui::End();
    }
    if (showUserGuide) {
        ImGui::Begin("User Guide");
        ImGui::ShowUserGuide();
        ImGui::End();
    }

    ImGui::Begin("Centered text");
    KDB_IMGUI::textCentered("This is centered text", ImVec4(1, 0, 0, 1));
    ImGui::End();


    ImguiGlfwVulkanDebugger::render();
    auto scale = ImGui_ImplVKGlfw_getCurrentMonitorContentScale();
    ImGui_ImplVKGlfw_endRender();
}

void MainWindow::shutdown() {
    ImGui_ImplVKGlfw_shutdown();
}
