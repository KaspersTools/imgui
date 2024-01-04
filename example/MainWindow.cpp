//
// Created by Kasper de Bruin on 03/01/2024.
//

#include "MainWindow.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include <imgui_notify.h>

#include <SDL_opengl.h>

namespace ImGuiExamples {

  void MainWindow::initSDL() {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
      printf("Error: %s\n", SDL_GetError());
      exit(-1);
    }


    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(// required on Apple
            SDL_GL_CONTEXT_FLAGS,
            SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    auto mainWindowFlags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    s_window = SDL_CreateWindow(
            "Humming Bird",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280,
            720,
            mainWindowFlags);

    // limit to which minimum size user can resize the window
    SDL_SetWindowMinimumSize(s_window, 800, 600);

    m_gl_context = SDL_GL_CreateContext(s_window);
    if (m_gl_context == nullptr) {
      printf("Error: %s\n", SDL_GetError());
      exit(-1);
    }
    SDL_GL_MakeCurrent(s_window, m_gl_context);

    // enable VSync
    SDL_GL_SetSwapInterval(1);
    int sdlOpenGLmajor = 0, sdlOpenGLminor = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &sdlOpenGLmajor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &sdlOpenGLminor);
  }

  void MainWindow::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // setup platform/renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(s_window, m_gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");
  }

  void MainWindow::run() {
    while (!m_exit) {
      render();
    }
  }

  void MainWindow::render() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        m_exit = true;
      if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(s_window))
        m_exit = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    renderUI();

    //Render toast notifications
    {
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
      ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
      ImGui::RenderNotifications();
      ImGui::PopStyleVar(1);// Don't forget to Pop()
      ImGui::PopStyleColor(1);
    }

    ImGui::Render();

    glViewport(0, 0, (int) getWindowWidth(), (int) getWindowHeight());
    glClearColor(m_clearColor.x * m_clearColor.w,
                 m_clearColor.y * m_clearColor.w,
                 m_clearColor.z * m_clearColor.w,
                 m_clearColor.w);

    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(s_window);
  }

  void MainWindow::renderUI() {
    beginDockSpace();
    renderTaskBar();
  }

  void MainWindow::beginDockSpace() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  }

  void MainWindow::renderTaskBar() {
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New")) {
        }
        if (ImGui::MenuItem("Open")) {
        }
        if (ImGui::MenuItem("Save")) {
        }
        if (ImGui::MenuItem("Save As")) {
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit")) {
          m_exit = true;
        }

        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

  void MainWindow::renderWindows() {
    ImGui::Begin("Welcome to Humming Bird", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::Text("Welcome to Humming Bird");
    ImGui::End();

    ImGui::ShowDemoWindow();
  }

  void MainWindow::endDockSpace() {

  }

  // Decide GL+GLSL versions
}// namespace ImGuiExamples