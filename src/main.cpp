#include "application.hpp"

#include "SDL2/SDL.h"

#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {
	std::cout << "Beginning " << argv[0] << std::endl;
	try {
		Application app;

		app.Init(argc, argv);
		app.Proc();
		app.Quit();
	}
	catch(std::exception& e) {
		std::cerr << "Fatal Error: " << e.what() << std::endl;
		return 1;
	}
	std::cout << "Clean exit from " << argv[0] << std::endl;
	return 0;
}