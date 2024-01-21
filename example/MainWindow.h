//
// Created by Kasper de Bruin on 19/01/2024.
//

#ifndef IMGUI_MAINWINDOW_H
#define IMGUI_MAINWINDOW_H

#include <imgui_impl_glfw_vulkan_window.h>
#include <functional>

class MainWindow {
public:
    MainWindow(){
      init();
    }
    ~MainWindow(){
      shutdown();
    }
    bool run();
private:
    void init();
    void render();
    void shutdown();

private:
};


#endif //IMGUI_MAINWINDOW_H
