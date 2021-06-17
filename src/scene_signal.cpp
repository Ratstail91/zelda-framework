#include "application.hpp"

//Add the custom scene headers here
#include "root_scene.hpp"
#include "example_scene.hpp"
#include "editor_scene.hpp"
#include "pong_scene.hpp"

#include <sstream>
#include <stdexcept>

using namespace std::string_literals;

static void error(std::string const& str) {
	std::ostringstream msg;
	msg << str << ": " << SDL_GetError();
	throw(std::runtime_error(msg.str()));
}

void Application::ProcessSceneSignal(SceneSignal signal) {
	switch(signal) {
		case SceneSignal::FIRST: //for the first scene to be loaded
		case SceneSignal::ROOT_SCENE:
			sceneList.push_front(new RootScene());
		break;

		case SceneSignal::EXAMPLE_SCENE:
			sceneList.push_front(new ExampleScene());
		break;

		case SceneSignal::EDITOR_SCENE:
			sceneList.push_front(new EditorScene());
		break;

		case SceneSignal::PONG_SCENE:
			sceneList.push_front(new PongScene());
		break;

		//add more scenes here

		case SceneSignal::POP:
			PopScene();
		break;

		default: {
			error("Failed to recognize the scene signal"s);
		}
	}
}
