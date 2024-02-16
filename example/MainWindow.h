//
// Created by Kasper de Bruin on 19/01/2024.
//

#ifndef IMGUI_MAINWINDOW_H
#define IMGUI_MAINWINDOW_H

#include <HBUI/HBUI.h>

class MainWindow {
public:
		MainWindow() {
			init();
		}

		~MainWindow() {
			shutdown();
		}

		bool run();

private:
		void init();

		void createMainMenuBars();

		void createDockSpace();

		void render();

		void shutdown();

private:
		bool showDebugWindow   = true;
		bool showDemoWindow    = true;
		bool withMenuBar       = true;
		bool horizontalMenuBar = false;
		bool verticalMenuBar   = true;
		bool withDockSpace     = true;
};


#endif//IMGUI_MAINWINDOW_H
