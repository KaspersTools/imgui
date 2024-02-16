#include "MainWindow.h"

// Main code
int main(int, char **) {
	MainWindow *window = new MainWindow();
	window->run();

	delete window;
}
