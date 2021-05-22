#include "application.hpp"

//Add the custom scene headers here
#include "example_scene.hpp"

#include <sstream>
#include <stdexcept>

static void error(std::string str) {
	std::ostringstream msg;
	msg << str << ": " << SDL_GetError();
	throw(std::runtime_error(msg.str()));
}

//TODO: code generation to do this?
void Application::ProcessSceneSignal(SceneSignal signal) {
	switch(signal) {
		case SceneSignal::FIRST: //for the first scene to be loaded
		case SceneSignal::EXAMPLE_SCENE:
			sceneList.push_front(new ExampleScene());
		break;

		//add more scenes here

		case SceneSignal::POP:
			PopScene();
		break;

		default: {
			error("Failed to recognize the scene signal");
		}
	}
}
