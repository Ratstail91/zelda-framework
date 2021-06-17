#include "root_scene.hpp"

RootScene::RootScene() {
	//
}

RootScene::~RootScene() {
	removeDescendantsOfNode(&root);
}

//frame phases
void RootScene::OnFrameStart() {
	//
}

void RootScene::OnUpdate() {
	//
}

void RootScene::OnFrameEnd() {
	//
}

void RootScene::OnRenderFrame(SDL_Renderer* renderer) {
	//
}

//input events
void RootScene::OnMouseMotion(SDL_MouseMotionEvent const& event) {
	//
}

void RootScene::OnMouseButtonDown(SDL_MouseButtonEvent const& event) {
	//
}

void RootScene::OnMouseButtonUp(SDL_MouseButtonEvent const& event) {
	//
}

void RootScene::OnMouseWheel(SDL_MouseWheelEvent const& event) {
	//
}

void RootScene::OnKeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			SetSceneSignal(SceneSignal::POP);
		break;

		//utility scenes
		case SDLK_SPACE:
			SetSceneSignal(SceneSignal::EXAMPLE_SCENE);
		break;

		case SDLK_TAB:
			SetSceneSignal(SceneSignal::EDITOR_SCENE);
		break;

		//game scenes
		case SDLK_1:
			SetSceneSignal(SceneSignal::PONG_SCENE);
		break;
	}
}

void RootScene::OnKeyUp(SDL_KeyboardEvent const& event) {
	//
}

void RootScene::OnControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//
}

void RootScene::OnControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//
}

void RootScene::OnControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//
}