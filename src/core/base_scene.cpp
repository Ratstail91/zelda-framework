#include "base_scene.hpp"

SDL_Renderer* BaseScene::rendererHandle = nullptr;

BaseScene::BaseScene() {
	//EMPTY
}

BaseScene::~BaseScene() {
	//EMPTY
}

void BaseScene::RenderFrame(SDL_Renderer* renderer) {
	//EMPTY
}

void BaseScene::SetRenderer(SDL_Renderer* r) {
	rendererHandle = r;
}

SDL_Renderer* BaseScene::GetRenderer() {
	return rendererHandle;
}

void BaseScene::SetSceneSignal(SceneSignal signal) {
	sceneSignal = signal;
}

SceneSignal BaseScene::GetSceneSignal() {
	return sceneSignal;
}

//-------------------------
//frame phases
//-------------------------

void BaseScene::FrameStart() {
	//EMPTY
}

void BaseScene::Update() {
	//EMPTY
}

void BaseScene::FrameEnd() {
	//EMPTY
}

//-------------------------
//input events
//-------------------------

void BaseScene::QuitEvent() {
	sceneSignal = SceneSignal::QUIT;
}

void BaseScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	//EMPTY
}

void BaseScene::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	//EMPTY
}

void BaseScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	//EMPTY
}

void BaseScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	//EMPTY
}

void BaseScene::KeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
		break;
	}
}

void BaseScene::KeyUp(SDL_KeyboardEvent const& event) {
	//EMPTY
}

void BaseScene::ControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//EMPTY
}

void BaseScene::ControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//EMPTY
}

void BaseScene::ControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//EMPTY
}
