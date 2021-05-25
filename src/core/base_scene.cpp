#include "base_scene.hpp"

SDL_Renderer* BaseScene::rendererHandle = nullptr;

BaseScene::BaseScene() {
	//EMPTY
}

BaseScene::~BaseScene() {
	//EMPTY
}

void BaseScene::OnRenderFrame(SDL_Renderer* renderer) {
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

void BaseScene::OnEnter() {
	//EMPTY
}

void BaseScene::OnExit() {
	//EMPTY
}

//-------------------------
//frame phases
//-------------------------

void BaseScene::OnFrameStart() {
	//EMPTY
}

void BaseScene::OnUpdate() {
	//EMPTY
}

void BaseScene::OnFrameEnd() {
	//EMPTY
}

//-------------------------
//input events
//-------------------------

void BaseScene::OnQuitEvent() {
	sceneSignal = SceneSignal::QUIT;
}

void BaseScene::OnMouseMotion(SDL_MouseMotionEvent const& event) {
	//EMPTY
}

void BaseScene::OnMouseButtonDown(SDL_MouseButtonEvent const& event) {
	//EMPTY
}

void BaseScene::OnMouseButtonUp(SDL_MouseButtonEvent const& event) {
	//EMPTY
}

void BaseScene::OnMouseWheel(SDL_MouseWheelEvent const& event) {
	//EMPTY
}

void BaseScene::OnKeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			OnQuitEvent();
		break;
	}
}

void BaseScene::OnKeyUp(SDL_KeyboardEvent const& event) {
	//EMPTY
}

void BaseScene::OnControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//EMPTY
}

void BaseScene::OnControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//EMPTY
}

void BaseScene::OnControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//EMPTY
}
