#include "application.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>

static void error(std::string str) {
	std::ostringstream msg;
	msg << str << ": " << SDL_GetError();
	throw(std::runtime_error(msg.str()));
}

void Application::Init(int argc, char* argv[]) {
	//default screen size
	const int screenWidth = 800;
	const int screenHeight = 600;

	//initialize SDL
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) {
		error("Failed to initialize SDL");
	}

	//create and check the window
	window = SDL_CreateWindow(
		"Example Caption",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth,
		screenHeight,
		SDL_WINDOW_RESIZABLE);

	if (!window) {
		error("Failed to create the window");
	}

	//create and check the renderer
	if (!(renderer = SDL_CreateRenderer(window, -1, 0))) {
		error("Failed to create the renderer");
	}

	//screen scaling
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);

	//set the hook for the renderer
	BaseScene::SetRenderer(renderer);

	//Initialize SDL_mixer
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
		error("Failed to open audio");
	}

	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) {
		error("Failed to initialize OGG format");
	}
}

void Application::Proc() {
	//load the first scene
	ProcessSceneSignal(SceneSignal::FIRST);

	//fixed frame rate
	typedef std::chrono::steady_clock Clock;

	Clock::time_point simTime = Clock::now();
	Clock::time_point realTime;
	constexpr std::chrono::duration<int, std::milli> frameDelay(16); //~60FPS

	//the game loop continues until the scenes signal QUIT
	while(sceneList.size() > 0 && sceneList.front()->GetSceneSignal() != SceneSignal::QUIT) {
		//switch scenes if necessary
		if(sceneList.front()->GetSceneSignal() != SceneSignal::CONTINUE) {
			ProcessSceneSignal(sceneList.front()->GetSceneSignal());
			continue;
		}

		//update the current time
		realTime = Clock::now();

		//simulate the game or give the machine a break
		if (simTime < realTime) {
			while(simTime < realTime) {
				//call the user defined functions
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->FrameStart();
					if ((*it)->GetFreezing()) {
						break;
					}
				}

				ProcessEvents();

				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->Update();
					if ((*it)->GetFreezing()) {
						break;
					}
				}

				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->FrameEnd();
					if ((*it)->GetFreezing()) {
						break;
					}
				}

				//step to the next frame
				simTime += frameDelay;
			}
		}
		else {
			SDL_Delay(1); //give the machine a break
		}

		//blank the screen (can be disabled for performance)
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//actually render (from the highest hiding member forward)
		std::list<BaseScene*>::iterator it = sceneList.begin();
		while(std::next(it, 1) != sceneList.end() && !(*it)->GetHiding()) {
			it++; //search up the first hiding member
		}
		do {
			(*it)->RenderFrame(renderer);
			it--;
		} while(std::next(it, 1) != sceneList.begin()); //while still pointing to the list
		SDL_RenderPresent(renderer);
	}
}

void Application::Quit() {
	//clean up after the program
	for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
		delete *it;
	}

	Mix_Quit();
	Mix_CloseAudio();
	for (auto it : gameControllers) {
		SDL_GameControllerClose(it.second);
	}
	BaseScene::SetRenderer(nullptr);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//Scene management
void Application::ProcessEvents() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			//default
			case SDL_QUIT:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->QuitEvent();
				}
			break;

			//mouse events
			case SDL_MOUSEMOTION:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->MouseMotion(event.motion);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->MouseButtonDown(event.button);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_MOUSEBUTTONUP:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->MouseButtonUp(event.button);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_MOUSEWHEEL:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->MouseWheel(event.wheel);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			//keyboard events
			case SDL_KEYDOWN:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->KeyDown(event.key);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_KEYUP:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->KeyUp(event.key);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			//TODO: joystick events

			//controller events
			case SDL_CONTROLLERAXISMOTION:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->ControllerAxisMotion(event.caxis);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_CONTROLLERBUTTONDOWN:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->ControllerButtonDown(event.cbutton);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_CONTROLLERBUTTONUP:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->ControllerButtonUp(event.cbutton);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_CONTROLLERDEVICEADDED: {
				SDL_GameController* controller = SDL_GameControllerOpen(event.cdevice.which);
				if (!controller) {
					std::cerr << "Could not open game controller " << event.cdevice.which << std::endl;
				}
				std::cout << "Controller added: " << event.cdevice.which << std::endl;
				gameControllers[event.cdevice.which] = controller;
			}
			break;

			case SDL_CONTROLLERDEVICEREMOVED:
				SDL_GameControllerClose(gameControllers[event.cdevice.which]);
				gameControllers.erase(event.cdevice.which);
				std::cout << "Controller removed: " << event.cdevice.which << std::endl;
			break;

//			case SDL_CONTROLLERDEVICEREMAPPED:
//				//TODO: what is this?
//			break;

			//TODO: text editing

			//window events are handled internally
			case SDL_WINDOWEVENT:
				switch(event.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						SDL_RenderSetLogicalSize(renderer, event.window.data1, event.window.data2);
					break;
				}
			break;
		}
	}
}

void Application::PopScene() {
	delete sceneList.front();
	sceneList.pop_front();

	//reset the scene signal when there's nothing on top of that scene
	if (sceneList.size() > 0) {
		sceneList.front()->SetSceneSignal(SceneSignal::CONTINUE);
	}
}