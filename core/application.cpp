#include "application.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>

void error(std::string str) {
	std::ostringstream msg;
	msg << str << ": " << SDL_GetError();
	throw(std::runtime_error(msg.str()));
}

void Application::Init(int argc, char* argv[]) {
	//default screen size
	const int screenWidth = 800;
	const int screenHeight = 600;

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

	//setup the game controllers
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
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
	while(activeScene->GetSceneSignal() != SceneSignal::QUIT) {
		//switch scenes if necessary
		if(activeScene->GetSceneSignal() != SceneSignal::CONTINUE) {
			ProcessSceneSignal(activeScene->GetSceneSignal());
			continue;
		}

		//update the current time
		realTime = Clock::now();

		//simulate the game or give the machine a break
		if (simTime < realTime) {
			while(simTime < realTime) {
				//call the user defined functions
				activeScene->FrameStart();
				ProcessEvents();
				activeScene->Update();
				activeScene->FrameEnd();

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

		//actually render
		activeScene->RenderFrame(renderer);
		SDL_RenderPresent(renderer);
	}

	//cleanup
	ClearScene();
}

void Application::Quit() {
	//clean up after the program
	for (auto it : gameControllers) {
		SDL_GameControllerClose(it.second);
	}
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	BaseScene::SetRenderer(nullptr);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

//Scene management
void Application::ProcessEvents() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			//default
			case SDL_QUIT:
				activeScene->QuitEvent();
			break;

			//mouse events
			case SDL_MOUSEMOTION:
				activeScene->MouseMotion(event.motion);
			break;

			case SDL_MOUSEBUTTONDOWN:
				activeScene->MouseButtonDown(event.button);
			break;

			case SDL_MOUSEBUTTONUP:
				activeScene->MouseButtonUp(event.button);
			break;

			case SDL_MOUSEWHEEL:
				activeScene->MouseWheel(event.wheel);
			break;

			//keyboard events
			case SDL_KEYDOWN:
				activeScene->KeyDown(event.key);
			break;

			case SDL_KEYUP:
				activeScene->KeyUp(event.key);
			break;

			//TODO: joystick events

			//controller events
			case SDL_CONTROLLERAXISMOTION:
				activeScene->ControllerAxisMotion(event.caxis);
			break;

			case SDL_CONTROLLERBUTTONDOWN:
				activeScene->ControllerButtonDown(event.cbutton);
			break;

			case SDL_CONTROLLERBUTTONUP:
				activeScene->ControllerButtonUp(event.cbutton);
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

//Add the custom scene headers here
#include "example_scene.hpp"

//TODO: code generation to do this?
void Application::ProcessSceneSignal(SceneSignal signal) {
	ClearScene();

	switch(signal) {
		case SceneSignal::FIRST: //for the first scene to be loaded
		case SceneSignal::EXAMPLE_SCENE:
			activeScene = new ExampleScene();
		break;

		default: {
			error("Failed to recognize the scene signal");
		}
	}
}

void Application::ClearScene() {
	delete activeScene;
	activeScene = nullptr;
}