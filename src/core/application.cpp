#include "application.hpp"

#include "audio_mixer.hpp"
#include "imgui_impl_sdl.h"
#include "imgui_sdl.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>

static void error(const std::string& str) {
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
				for (auto scenePtr : sceneList) {
					scenePtr->OnFrameStart();
					if (scenePtr->GetFreezing()) {
						break;
					}
				}

				ProcessEvents();

				for (auto scenePtr : sceneList) {
					scenePtr->OnUpdate();
					if (scenePtr->GetFreezing()) {
						break;
					}
				}

				for (auto scenePtr : sceneList) {
					scenePtr->OnFrameEnd();
					if (scenePtr->GetFreezing()) {
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
		auto it = sceneList.begin();
		while(std::next(it, 1) != sceneList.end() && !(*it)->GetHiding()) {
			it++; //search up the top-most hiding member
		}

		do {
			(*it)->OnRenderFrame(renderer);
		} while(std::next(--it, 1) != sceneList.begin()); //while still pointing to the list

		//end the frame
		ImGui::EndFrame();
		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);
	}
}

void Application::Quit() {
	//clean up after the program
	for (auto scenePtr : sceneList) {
		scenePtr->OnExit();
		delete scenePtr;
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
				for (auto scenePtr : sceneList) {
					scenePtr->OnQuitEvent();
				}
			break;

			//mouse events
			case SDL_MOUSEMOTION:
				for (auto scenePtr : sceneList) {
					scenePtr->OnMouseMotion(event.motion);
					if (scenePtr->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				for (auto scenePtr : sceneList) {
					scenePtr->OnMouseButtonDown(event.button);
					if (scenePtr->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_MOUSEBUTTONUP:
				for (auto scenePtr : sceneList) {
					scenePtr->OnMouseButtonUp(event.button);
					if (scenePtr->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_MOUSEWHEEL:
				for (auto scenePtr : sceneList) {
					scenePtr->OnMouseWheel(event.wheel);
					if (scenePtr->GetBlocking()) {
						break;
					}
				}
			break;

			//keyboard events
			case SDL_KEYDOWN:
				for (auto scenePtr : sceneList) {
					scenePtr->OnKeyDown(event.key);
					if (scenePtr->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_KEYUP:
				for (auto scenePtr : sceneList) {
					scenePtr->OnKeyUp(event.key);
					if (scenePtr->GetBlocking()) {
						break;
					}
				}
			break;

			//TODO: joystick events

			//controller events
			case SDL_CONTROLLERAXISMOTION:
				for (auto scenePtr : sceneList) {
					scenePtr->OnControllerAxisMotion(event.caxis);
					if (scenePtr->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_CONTROLLERBUTTONDOWN:
				for (auto scenePtr : sceneList) {
					scenePtr->OnControllerButtonDown(event.cbutton);
					if (scenePtr->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_CONTROLLERBUTTONUP:
				for (auto scenePtr : sceneList) {
					scenePtr->OnControllerButtonUp(event.cbutton);
					if (scenePtr->GetBlocking()) {
						break;
					}
				}
			break;

			case SDL_CONTROLLERDEVICEADDED: {
				SDL_GameController* controller = SDL_GameControllerOpen(event.cdevice.which);
				if (!controller) {
					error("Could not open game controller");
				}
				gameControllers[event.cdevice.which] = controller;
			}
			break;

			case SDL_CONTROLLERDEVICEREMOVED:
				SDL_GameControllerClose(gameControllers[event.cdevice.which]);
				gameControllers.erase(event.cdevice.which);
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