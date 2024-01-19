//
// Created by Kasper de Bruin on 19/01/2024.
//

#ifndef IMGUI_MAINWINDOW_H
#define IMGUI_MAINWINDOW_H

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
    bool m_demoWindow = false;
    bool m_showRenderStats = false;
};


#endif //IMGUI_MAINWINDOW_H
