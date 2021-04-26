#include "example_scene.hpp"

ExampleScene::ExampleScene() {
	//EMPTY
}

ExampleScene::~ExampleScene() {
	//EMPTY
}

//frame phases
void ExampleScene::FrameStart() {
	//
}

void ExampleScene::Update() {
	//
}

void ExampleScene::FrameEnd() {
	//
}

void ExampleScene::RenderFrame(SDL_Renderer* renderer) {
	//
}

//input events
void ExampleScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	//
}

void ExampleScene::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	//
}

void ExampleScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	//
}

void ExampleScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	//
}

void ExampleScene::KeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
		break;
	}
}

void ExampleScene::KeyUp(SDL_KeyboardEvent const& event) {
	//
}

void ExampleScene::ControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//
}

void ExampleScene::ControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//
}

void ExampleScene::ControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//
}