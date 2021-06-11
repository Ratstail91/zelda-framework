#include "application.hpp"

#include "audio_mixer.hpp"
#include "imgui_impl_sdl.h"
#include "imgui_sdl.h"

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

	//init audio
	AudioMixer::GetSingleton().Init();

	//init dear imgui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.DisplaySize.x = screenWidth;
	io.DisplaySize.y = screenHeight;
	io.IniFilename = nullptr;

	io.Fonts->AddFontDefault();
	io.Fonts->Build();

	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE; //still in beta, huh?

	//SDL renderer
	ImGui_ImplSDL2_InitForMetal(window);
	ImGuiSDL::Initialize(renderer, screenWidth, screenHeight);
}

void Application::Proc() {
	//load the first scene
	ProcessSceneSignal(SceneSignal::FIRST);
	sceneList.front()->OnEnter();

	//fixed frame rate
	typedef std::chrono::steady_clock Clock;

	Clock::time_point simTime = Clock::now();
	Clock::time_point realTime;
	constexpr std::chrono::duration<int, std::milli> frameDelay(16); //~60FPS

	//the game loop continues until the scenes signal QUIT
	while(sceneList.size() > 0 && sceneList.front()->GetSceneSignal() != SceneSignal::QUIT) {
		//switch scenes if necessary
		if(sceneList.front()->GetSceneSignal() != SceneSignal::CONTINUE) {
			sceneList.front()->OnExit();

			ProcessSceneSignal(sceneList.front()->GetSceneSignal());

			if (sceneList.size() > 0) {
				sceneList.front()->OnEnter();
			}
			continue;
		}

		//update the current time
		realTime = Clock::now();

		//simulate the game or give the machine a break
		if (simTime < realTime) {
			while(simTime < realTime) {
				//call the user defined functions
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnFrameStart();
					if ((*it)->GetFreezing()) {
						break;
					}
				}

				ProcessEvents();

				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnUpdate();
					if ((*it)->GetFreezing()) {
						break;
					}
				}

				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnFrameEnd();
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

		//start the frame
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		//actually render the scenes (from the highest hiding member forward)
		std::list<BaseScene*>::iterator it = sceneList.begin();
		while(std::next(it, 1) != sceneList.end() && !(*it)->GetHiding()) {
			it++; //search up the first hiding member
		}
		do {
			(*it)->OnRenderFrame(renderer);
			it--;
		} while(std::next(it, 1) != sceneList.begin()); //while still pointing to the list

		//end the frame
		ImGui::EndFrame();
		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);
	}
}

void Application::Quit() {
	//clean up after the program
	for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
		(*it)->OnExit();
		delete *it;
	}
	sceneList.clear();

	ImGuiSDL::Deinitialize();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	AudioMixer::GetSingleton().Quit();
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
		//ImGUI hook
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch(event.type) {
			//default
			case SDL_QUIT:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnQuitEvent();
				}
			break;

			//mouse events
			case SDL_MOUSEMOTION:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnMouseMotion(event.motion);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnMouseButtonDown(event.button);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_MOUSEBUTTONUP:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnMouseButtonUp(event.button);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_MOUSEWHEEL:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnMouseWheel(event.wheel);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			//keyboard events
			case SDL_KEYDOWN:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnKeyDown(event.key);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_KEYUP:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnKeyUp(event.key);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			//TODO: joystick events

			//controller events
			case SDL_CONTROLLERAXISMOTION:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnControllerAxisMotion(event.caxis);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_CONTROLLERBUTTONDOWN:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnControllerButtonDown(event.cbutton);
					if ((*it)->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_CONTROLLERBUTTONUP:
				for (std::list<BaseScene*>::iterator it = sceneList.begin(); it != sceneList.end(); it++) {
					(*it)->OnControllerButtonUp(event.cbutton);
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