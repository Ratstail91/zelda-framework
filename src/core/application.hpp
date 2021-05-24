#pragma once

#include "base_scene.hpp"
#include "scene_signal.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#include <list>
#include <map>

//DOCS: The Application class handles scene switching, utilizing only one window
class Application {
public:
	Application() = default;
	~Application() = default;

	void Init(int argc, char* argv[]);
	void Proc();
	void Quit();

private:
	//scene management
	void ProcessEvents();
	void ProcessSceneSignal(SceneSignal);
	void PopScene();

	std::list<BaseScene*> sceneList;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	std::map<int, SDL_GameController*> gameControllers;
};