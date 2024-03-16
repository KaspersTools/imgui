#include "MainWindow.h"

// Main code
int main(int, char **) {
	auto *window = new MainWindow();
	window->run();
	delete window;
}
