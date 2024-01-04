//
// Created by Kasper de Bruin on 03/01/2024.
//

#ifndef IMGUI_MAINWINDOW_H
#define IMGUI_MAINWINDOW_H

#include <SDL.h>
#include <imgui.h>

namespace ImGuiExamples {

  class MainWindow {
public:
    MainWindow() {
      initSDL();
      initImGui();
      run();
    }
    ~MainWindow() = default;

public:
    const float getWindowWidth() const {
      ImGuiIO &io = ImGui::GetIO();
      return io.DisplaySize.x;
    }
    const int getWindowHeight() const {
      ImGuiIO &io = ImGui::GetIO();
      return io.DisplaySize.y;
    }

    static SDL_Window *getWindow() { return s_window; }


private:
    void initSDL();
    void initImGui();
    void run();

    void render();
    void renderUI();
    void beginDockSpace();
    void renderTaskBar();
    void renderWindows();
    void endDockSpace();

private:
    void loadFonts();
    void enableFont();
    void disableFont();

private:
    inline static SDL_Window *s_window;
    SDL_GLContext m_gl_context{};

    bool m_exit = false;
    ImVec4 m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  };

}// namespace ImGuiExamples

#endif//IMGUI_MAINWINDOW_H
