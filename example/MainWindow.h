//
// Created by Kasper de Bruin on 19/01/2024.
//

#ifndef IMGUI_MAINWINDOW_H
#define IMGUI_MAINWINDOW_H

class MainWindow {
public:
    MainWindow(){
        run();
    }
    ~MainWindow(){

    }
    bool run();
private:
    void init();
    void render();
};


#endif //IMGUI_MAINWINDOW_H
